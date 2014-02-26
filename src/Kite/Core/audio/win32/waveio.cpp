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
#include "src/Kite/audio/tp/CWaves.h"
#include "src/Kite/audio/win32/waveio.h"

namespace Kite{
namespace Internal{
    WaveIO::WaveIO():
        _kcwaves(new CWaves()),
        _kID(0),
        _ksize(0),
        _ksampleRate(0),
        _kbyteRate(0),
        _kchannel(0),
        _kbitPerSample(0),
        _kblockAlign(0)
    {}

    WaveIO::~WaveIO(){
        _kcwaves->DeleteWaveFile(_kID);
        delete _kcwaves;
    }

    void WaveIO::openFile(const std::string &FileName){
        // open file
        KDEBUG_ASSERT_EQ(_kcwaves->OpenWaveFile(FileName.c_str(), &_kID), 0);

        // get information
        WAVEFORMATEX format;
        KDEBUG_ASSERT_EQ(_kcwaves->GetWaveSize(_kID, &_ksize), 0);
        KDEBUG_ASSERT_EQ(_kcwaves->GetWaveFormatExHeader(_kID, &format), 0);
        _ksampleRate = format.nSamplesPerSec;
        _kbyteRate = format.nAvgBytesPerSec;
        _kchannel = format.nChannels;
        _kbitPerSample = format.wBitsPerSample;
        _kblockAlign = format.nBlockAlign;
    }

    void WaveIO::readFile(void *Data, UL32 DataSize, UL32 *BytesWritten){
        KDEBUG_ASSERT_EQ(_kcwaves->ReadWaveData(_kID, Data, DataSize, BytesWritten), 0);
    }

    UL32 WaveIO::getReadOffset(){
        UL32 offset = 0;
        KDEBUG_ASSERT_EQ(_kcwaves->GetWaveDataOffset(_kID, &offset), 0);
        return offset;
    }

    void WaveIO::setReadOffset(UL32 Offset){
        KDEBUG_ASSERT_EQ(_kcwaves->SetWaveDataOffset(_kID, Offset), 0);
    }
}
}
