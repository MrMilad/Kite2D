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
#ifndef WAVEIO_H
#define WAVEIO_H

#include "Kite/core/system/ksystemdef.h"
#include <string>

class CWaves;
namespace Kite{
namespace Internal{
    class WaveIO{
    public:
        WaveIO();
        ~WaveIO();

        // open .wav file
        // max number of opened file handle = 1024
        void openFile(const std::string &FileName);

        // read data from opened file
        void readFile(void *Data, UL32 DataSize, UL32 *BytesWritten);

        UL32 getReadOffset();
        void setReadOffset(UL32 Offset);

        inline UL32 getSize() const {return _ksize;}
        inline UL32 getSampleRate() const {return _ksampleRate;}
        inline UL32 getByteRate() const {return _kbyteRate;}
        inline U16 getChannelCount() const {return _kchannel;}
        inline U16 getBitPerSample() const {return _kbitPerSample;}
        inline U16 getBlockAllign() const {return _kblockAlign;}

    private:
        CWaves *_kcwaves; // CWaves pointer
        I32 _kID;
        UL32 _ksize;
        UL32 _ksampleRate;
        UL32 _kbyteRate;
        U16 _kchannel;
        U16 _kbitPerSample;
        U16 _kblockAlign;
    };
}
}

#endif // WAVEIO_H
