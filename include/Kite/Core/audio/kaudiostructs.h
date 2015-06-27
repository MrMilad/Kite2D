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
#ifndef KAUDIOSTRUCTS_H
#define KAUDIOSTRUCTS_H

/*! \file kaudiostructs.h */

#include "Kite/Core/system/ksystemdef.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! Utility struct for storing information of sounds data. (Internally use)
    struct KAudioInfo{
		U8 channel;					//!< How many channels are contained in the audio data. For example, Stereo 2 channels
		U8 bitsPerSample;			//!< How many bits per sample
        I32 size;					//!< Total size of sound track (in bytes)
		I64 bitRate;				//!< The bit rate of PCM audio data (32, 96, 192, 320, ...)
		I64 sampleRate;				//!< The sample rate is in samples per second
        F64 totalTime;				//!< Total time of sound track 
        bool isSeekable;			//!< Seek is possible

		//! Default constructors
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
