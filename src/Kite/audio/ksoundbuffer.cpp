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
#include "Kite/audio/ksoundbuffer.h"
#include "src/Kite/audio/alcall.h"
#include <vector>
#include <cstdlib>

#if defined (KITE_PLATFORM_WINDOWS)

    #include "src/Kite/audio/win32/waveio.h"

#elif defined (KITE_PLATFORM_LINUX)


#elif defined (KITE_PLATFORM_MACOS)


#endif


namespace Kite{
    KSoundBuffer::KSoundBuffer():
        _ksampleRate(0),
        _kchannelCount(0),
        _kbitsPerSample(0),
        _ksize(0),
        _kID(0)
    {
        DAL_CALL(alGenBuffers(1, &_kID));
    }

    KSoundBuffer::~KSoundBuffer(){
        DAL_CALL(alDeleteBuffers(1, &_kID));
    }

    void KSoundBuffer::load(const std::string &FileName){
        Internal::WaveIO reader;
        void *Data;

        // open file for reading data
        reader.openFile(FileName);

        // first get file properties
        _ksize = reader.getSize();
        _ksampleRate = reader.getSampleRate();
        _kchannelCount = reader.getChannelCount();
        _kbitsPerSample = reader.getBitPerSample();

        // read data from file
        UL32 read;
        Data = malloc(_ksize);
        reader.setReadOffset(0);
        reader.readFile(Data, _ksize, &read);
        KDEBUG_ASSERT_T(read == _ksize);

        // find best format
        ALenum format = Internal::getFormat(_kchannelCount, _kbitsPerSample);
        KDEBUG_ASSERT_T(format != 0);

        // fill buffer
        DAL_CALL(alBufferData(_kID, format, Data, _ksize, _ksampleRate));

        // cleanup
        free(Data);
        Data = NULL;
    }

}
