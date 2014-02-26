/*
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Kite/audio/kstreamsource.h"
#include "Kite/audio/alcall.h"
#include "Kite/system/ksystemutil.h"
#include <cstdlib>
#if defined (KITE_PLATFORM_WINDOWS)

    #include "src/Kite/audio/win32/waveio.h"

#elif defined (KITE_PLATFORM_LINUX)


#elif defined (KITE_PLATFORM_MACOS)


#endif

namespace Kite{
    KStreamSource::KStreamSource():
        _kreader(new Internal::WaveIO),
        _kthread(new KThread(&Kite::KStreamSource::loader, this)),
        _kloop(false),
        _KuserStop(false)
    {
        DAL_CALL(alGenBuffers(4, _kbuffers));
    }

    KStreamSource::~KStreamSource(){
        stop();
        delete _kthread;
        delete _kreader;
        DAL_CALL(alSourcei(_kID, AL_BUFFER, 0));
        DAL_CALL(alDeleteBuffers(4, _kbuffers));
        // source will be deleted in base class destructure
    }

    void KStreamSource::play(){
        if (getState() != KSS_PLAYING){
            DAL_CALL(alSourcePlay(_kID));
            _KuserStop = false;
            _kthread->lunch();
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
        ALuint format = Internal::getFormat(_kreader->getChannelCount(), _kreader->getBitPerSample());
        KDEBUG_ASSERT_T(format != 0);

        // queue 250ms of audio data
        UL32 bufferSize = _kreader->getBlockAllign();
        bufferSize = _kreader->getByteRate() >> 2;

        // IMPORTANT : the buffer size must be an exact multiple of the BlockAlignment
        bufferSize -= (bufferSize % _kreader->getBlockAllign());

        // reading data
        // allocate enough size for storing data
        void *data = malloc(bufferSize);
        KDEBUG_ASSERT_T(data);

        // set read position to start of audio data
        _kreader->setReadOffset(0);

        // finally fill all the Buffers with audio data from the wavefile
        // our buffers are ready for streaming now
        UL32 bytes;
        for (U16 i = 0; i < 4; ++i){
            _kreader->readFile(data, bufferSize, &bytes);
            DAL_CALL(alBufferData(_kbuffers[i], format, data, bytes, _kreader->getSampleRate()));
            DAL_CALL(alSourceQueueBuffers(_kID, 1, &_kbuffers[i]));
        }

        // cleanup
        free(data);
    }

    void KStreamSource::loadFile(const std::string &FileName){
        // first we stop the source
        stop();

        // open file for reading
        _kreader->openFile(FileName);

        // fill buffers
        fillFirst4Buffer();

    }

    void KStreamSource::loader(){
        // queue 250ms of audio data
        UL32 bufferSize = _kreader->getBlockAllign();

        // IMPORTANT : the buffer size must be an exact multiple of the BlockAlignment
        bufferSize -= (bufferSize % _kreader->getBlockAllign());
        bufferSize = _kreader->getByteRate() >> 2;

        // allocate enough size for storing data
        void *data = malloc(bufferSize);
        KDEBUG_ASSERT_T(data);

        ALuint format = Internal::getFormat(_kreader->getChannelCount(), _kreader->getBitPerSample());
        KDEBUG_ASSERT_T(format != 0);

        while(true){
            // reduse extra CPU usage
            Kite::sleep(20);

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
                UL32 bytes;
                _kreader->readFile(data, bufferSize, &bytes);
                if (bytes > 0){

                    // Copy audio data to Buffer
                    DAL_CALL(alBufferData(bufID, format, data, bytes, _kreader->getSampleRate()));
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

        // cleanup
        free(data);
        data = NULL;
    }
}
