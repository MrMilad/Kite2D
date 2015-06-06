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
#include "Kite/core/audio/ksoundbuffer.h"
#include "src/Kite/core/audio/alcall.h"
#include "src/Kite/Core/audio/soundio.h"
#include <vector>
#include <cstdlib>


namespace Kite{
    KSoundBuffer::KSoundBuffer():
		KResources(KRT_AUDIO),
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

    void KSoundBuffer::loadFile(const std::string &FileName, Kite::KAudioFileTypes Format){
        Internal::SoundIO sound;
		char *Data;

        // open file for reading data
        sound.openFile(FileName.c_str(), Format);

        // first get file properties
        _ksize = sound.getInfo().size;
        _ksampleRate = sound.getInfo().sampleRate;
        _kchannelCount = sound.getInfo().channel;
        _kbitsPerSample = sound.getInfo().bitsPerSample;

        // read data from file
        I64 read;
		Data = new char[_ksize];
        read = sound.readData((void *)Data, (I32)_ksize);
        KDEBUG_ASSERT_T(read == _ksize);

        // find best format
        ALenum format = Internal::getFormat(_kchannelCount, _kbitsPerSample);
        KDEBUG_ASSERT_T(format != 0);

        // fill buffer
		DAL_CALL(alBufferData(_kID, format, (ALvoid *)Data, (ALsizei)_ksize, (ALsizei)_ksampleRate));

        // cleanup
		delete[] Data;
        Data = NULL;
    }

	U64 KSoundBuffer::resGetSize() const{
		if (_ksize > 0)
			return _ksize;

		return 0;
	}

}
