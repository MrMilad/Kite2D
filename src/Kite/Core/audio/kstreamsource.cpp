/*
    Kite2D Game Framework.
    Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/
#include "Kite/core/audio/kstreamsource.h"
#include "Kite/core/system/ksystemutil.h"
#include "src/Kite/core/audio/alcall.h"
#include "src/Kite/Core/audio/soundio.h"
#include <cstdlib>
#include <thread>
#include <chrono>

namespace Kite{
    KStreamSource::KStreamSource():
        _kreader(new Internal::SoundIO),
        _kthread(0),
        _kdata(0),
        _kloop(false),
        _KuserStop(false),
		_kmemstream(0)
    {
        // create openal contextand set as current context
		Internal::initeAL();

        // create 4 buffer
        DAL_CALL(alGenBuffers(4, _kbuffers));

        // temporary buffer
        _kdata = new I8[KOGG_BUFF_SIZE * 4];
    }

    KStreamSource::~KStreamSource(){
        stop();
		if (_kthread){
			_kthread->join();
			delete _kthread;
		}
        delete _kreader;
        delete[] _kdata;
        DAL_CALL(alSourcei(_kID, AL_BUFFER, 0));
        DAL_CALL(alDeleteBuffers(4, _kbuffers));
        // source will be deleted in base class destructure

		if (_kmemstream)
			delete _kmemstream;
    }

    void KStreamSource::play(){
        if (getState() != KSS_PLAYING){
            DAL_CALL(alSourcePlay(_kID));
            _KuserStop = false;
			if (_kthread){
				_kthread->detach();
				delete _kthread;
			}
			_kthread = new std::thread(&Kite::KStreamSource::loader, this);
        }
    }

    void KStreamSource::pause(){
        DAL_CALL(alSourcePause(_kID));
    }

    void KStreamSource::stop(){
        _KuserStop = true;
        DAL_CALL(alSourceStop(_kID));
    }

    void KStreamSource::fillFirst4Buffer(){
        // clear queue
        DAL_CALL(alSourcei(_kID, AL_BUFFER, 0));

        // check file format
        ALuint format = Internal::getFormat(_kreader->getInfo().channel, _kreader->getInfo().bitsPerSample);
        KDEBUG_ASSERT_T(format != 0);

        // set read position to start of audio data
        _kreader->setReadOffset(0);

        // finally fill all the Buffers with audio data
        // our buffers are ready for streaming now
        UL32 read = 0;
        for (U16 i = 0; i < 4; ++i){
            read = _kreader->readData((void *)_kdata, KOGG_BUFF_SIZE * 4);
            DAL_CALL(alBufferData(_kbuffers[i], format, _kdata, read, (ALsizei)_kreader->getInfo().sampleRate));
            DAL_CALL(alSourceQueueBuffers(_kID, 1, &_kbuffers[i]));
        }
    }

	void KStreamSource::setLoop(bool Loop) {
		_kloop = Loop; 
	}

	bool KStreamSource::getLoop() {
		return _kloop; 
	}

	bool KStreamSource::loadFile(const std::string &FileName, U32 FileType) {
        // first we stop the source
        stop();

        // open file for reading
		bool ret = _kreader->openFile(FileName.c_str(), KAF_OGG);

        // fill buffers
        fillFirst4Buffer();

		return ret;
    }

	bool KStreamSource::loadStream(KInputStream &InputStream, U32 FileType) {
		// first we stop the source
		stop();

		// open file for reading
		bool ret = _kreader->openFile(InputStream, KAF_OGG);

		// fill buffers
		fillFirst4Buffer();

		return ret;
	}

	bool KStreamSource::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		// first we stop the source
		stop();

		if (_kmemstream)
			delete _kmemstream;

		_kmemstream = new KMemInputStream(Data, Size);

		return this->loadStream(*_kmemstream);
	}

    void KStreamSource::loader(){
        ALuint format = Internal::getFormat(_kreader->getInfo().channel, _kreader->getInfo().bitsPerSample);
        KDEBUG_ASSERT_T(format != 0);

        while(true){
            // reduse extra CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Request the number of OpenAL Buffers have been processed (played) on the Source
            ALint bufProcCount = 0;
            DAL_CALL(alGetSourcei(_kID, AL_BUFFERS_PROCESSED, &bufProcCount));

            // for each processed buffer, remove it from the Source Queue, read next chunk of audio
            // data from disk, fill buffer with new data, and add it to the Source Queue
            while (bufProcCount){

                // remove the Buffer from the Queue.  (bufID contains the Buffer ID for the unqueued Buffer)
                ALuint bufID = 0;
                DAL_CALL(alSourceUnqueueBuffers(_kID, 1, &bufID));

                // read more audio data (if there is any)
                I32 read;
                read = _kreader->readData((void *)_kdata, KOGG_BUFF_SIZE * 4);
                if (read > 0){

                    // Copy audio data to Buffer
                    DAL_CALL(alBufferData(bufID, format, _kdata, read, (ALsizei)_kreader->getInfo().sampleRate));
                    // Queue Buffer on the Source
                    DAL_CALL(alSourceQueueBuffers(_kID, 1, &bufID));
                }

                --bufProcCount;
            }

            // Check the status of the Source.  If it is not playing, then playback was completed,
            // or user stoped it
            // or the Source was starved of audio data, and needs to be restarted.
            ALint state;
            DAL_CALL(alGetSourcei(_kID, AL_SOURCE_STATE, &state));

            // AL_PLAYING
            if (state == AL_PLAYING){
                continue;

            // AL_STOPED
            }else if(state == AL_STOPPED){

                // if user stoped it
                if (_KuserStop){
                    // we inite buffer for next play() call
                    fillFirst4Buffer();
                    break;
                }

                // if there are Buffers in the Source Queue then the Source was starved of audio
                // data, so needs to be restarted (because there is more audio data to play)
                ALint QueuedBuffers;
                DAL_CALL(alGetSourcei(_kID, AL_BUFFERS_QUEUED, &QueuedBuffers));

                // starved buffer
                if (QueuedBuffers){
                    DAL_CALL(alSourcePlay(_kID));

                // playback was completed
                }else{
                    // check loop
                    if (_kloop){
                        fillFirst4Buffer();
                        continue;
                    }

                    // end of story!
                    break;
                }

            // AL_PAUSED or ...
            }else{
                break;
            }
        }
    }
}
