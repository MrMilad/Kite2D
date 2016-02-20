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
#include "Kite/core/audio/ksoundbuffer.h"
#include "Kite/core/utility/kmeminputstream.h"
#include "src/Kite/core/audio/alcall.h"
#include "src/Kite/Core/audio/soundio.h"
#include <vector>
#include <cstdlib>


namespace Kite{
    KSoundBuffer::KSoundBuffer():
		KCoreInstance(KCI_AUDIOBUFFER),
		KResource(KRT_AUDIOBUFFER),
        _ksampleRate(0),
        _kchannelCount(0),
        _kbitsPerSample(0),
        _ksize(0),
        _kID(0)
    {
		Internal::initeAL();
        DAL_CALL(alGenBuffers(1, &_kID));
    }

    KSoundBuffer::~KSoundBuffer(){
        DAL_CALL(alDeleteBuffers(1, &_kID));
    }

	bool KSoundBuffer::loadFile(const std::string &FileName, U32 FileType ) {
        Internal::SoundIO sound;
		char *Data;
		bool ret = false;

        // open file for reading data
        sound.openFile(FileName.c_str(), KAF_OGG);

        // first get file properties
        _ksize = sound.getInfo().size;
        _ksampleRate = sound.getInfo().sampleRate;
        _kchannelCount = sound.getInfo().channel;
        _kbitsPerSample = sound.getInfo().bitsPerSample;

        // read data from file
        I64 read;
		Data = new char[_ksize];
        read = sound.readData((void *)Data, (I32)_ksize);
		if (read == _ksize){

			// find best format
			ALenum format = Internal::getFormat(_kchannelCount, _kbitsPerSample);
			if (format != 0){

				// fill buffer
				DAL_CALL(alBufferData(_kID, format, (ALvoid *)Data, (ALsizei)_ksize, (ALsizei)_ksampleRate));
				ret = true;
			}
			else{
				KDEBUG_PRINT("Proper audio format not found");
			}
		}
		else{
			KDEBUG_PRINT("Error in reading sound file");
		}

        // cleanup
		delete[] Data;
        Data = NULL;
		return ret;
    }

	bool KSoundBuffer::loadStream(KIStream &InputStream, U32 FileType) {
		Internal::SoundIO sound;
		char *Data;
		bool ret = false;

		// open stream
		sound.openFile(InputStream, KAF_OGG);

		// first get file properties
		_ksize = sound.getInfo().size;
		_ksampleRate = sound.getInfo().sampleRate;
		_kchannelCount = sound.getInfo().channel;
		_kbitsPerSample = sound.getInfo().bitsPerSample;

		// read data from file
		I64 read;
		Data = new char[_ksize];
		read = sound.readData((void *)Data, (I32)_ksize);
		if (read == _ksize){

			// find best format
			ALenum format = Internal::getFormat(_kchannelCount, _kbitsPerSample);
			if (format != 0){

				// fill buffer
				DAL_CALL(alBufferData(_kID, format, (ALvoid *)Data, (ALsizei)_ksize, (ALsizei)_ksampleRate));
				ret = true;
			}
			else{
				KDEBUG_PRINT("Proper audio format not found");
			}
		}
		else{
			KDEBUG_PRINT("Error in reading sound file");
		}

		// cleanup
		delete[] Data;
		Data = NULL;
		return ret;
	}

	bool KSoundBuffer::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		KMemInputStream temp(Data, Size);
		return loadStream(temp);
	}

	U64 KSoundBuffer::getInstanceSize() const{
		if (_ksize > 0)
			return _ksize;

		return 0;
	}

}
