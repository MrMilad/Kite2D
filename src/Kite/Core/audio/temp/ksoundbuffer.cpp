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
#include "src/Kite/audio/soundio.h"
#include <vector>

namespace Kite{
    KSoundBuffer::KSoundBuffer():
        _ksRate(0),
        _kchCount(0),
        _ksize(0),
        _kID(0)
    {
        DAL_CALL(alGenBuffers(1, &_kID));
    }

    KSoundBuffer::~KSoundBuffer(){
        DAL_CALL(alDeleteBuffers(1, &_kID));
    }

    void KSoundBuffer::load(const std::string &FileName){
        Internal::SoundIO reader;
        std::vector<I16> Data;

        // open file for reading data
        Data.resize(reader.getSampleCount());
        reader.openReadFromFile(FileName);

        // first get file properties
        _ksCount = reader.getSampleCount();
        _ksRate = reader.getSampleRate();
        _kchCount = reader.getChannelCount();

        // read data from file
        KDEBUG_ASSERT((reader.read(&Data[0], _ksCount) == _ksCount));

        // find best format from ChannelCount
        ALenum format = Internal::getFormatChCount(_kchCount);
        KDEBUG_ASSERT(format != 0);

        // fill buffer
        ALsizei size = (ALsizei)(Data.size() * sizeof(I16));
        _ksize = size;
        DAL_CALL(alBufferData(_kID, format, &Data[0], size, _ksRate));
    }

}
