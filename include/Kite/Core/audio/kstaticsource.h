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
#ifndef KSTATICSOURCE_H
#define KSTATICSOURCE_H

/*! \file kstaticsource.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/audio/ksoundsource.h"
#include "Kite/Core/audio/ksoundbuffer.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KStaticSource class provide the ability to play sound buffers with a variety of options.
	/*!
		KStaticSource is perfect for playing short sounds that can fit in memory and require no latency,
		like foot steps or gun shots.
	*/
    class KITE_FUNC_EXPORT KStaticSource : public KSoundSource{
    public:
        void play();
        void pause();
        void stop();

        void setLoop(bool Loop);
        bool getLoop();

		//! Attach a buffer to the source
		/*!
			For a source in the KSS_PLAYING or KSS_PAUSED state, attaching buffer will
			result in a invalid state. sound buffers can be applied only to sources in the KSS_INTIAL and KSS_STOPPED states.

			\param Buffer Sound buffer
		*/
        void attachBuffer(const KSoundBuffer &Buffer);

		//! Deattach all buffers from the source
		/*!
			
		*/
        void deattachBuffer();

        /// set play offset
        void setPlayOffset(F32 Second);
    };
}

#endif // KSTATICSOURCE_H
