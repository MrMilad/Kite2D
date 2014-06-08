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
#ifndef KAUDIOSTRUCTS_H
#define KAUDIOSTRUCTS_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
    struct KAudioInfo{
        U8 channel, bitsPerSample;
        I32 size;
        I64 bitRate, sampleRate;
        F64 totalTime;
        bool isSeekable;

        KAudioInfo(U8 Channel = 0, U8 BitsPerSample = 0,
                   I32 Size = 0, I64 BitRate = 0, I64 SampleRate = 0,
                   F64 TotalTime = 0.0, bool IsSeekable = false):
            channel(Channel), bitsPerSample(BitsPerSample),
            size(Size), bitRate(BitRate), sampleRate(SampleRate),
            totalTime(TotalTime), isSeekable(IsSeekable)
        {}
    };
}

#endif // KAUDIOSTRUCTS_H