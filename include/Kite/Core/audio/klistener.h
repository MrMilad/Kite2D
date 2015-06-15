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
#ifndef KLISTENER_H
#define KLISTENER_H

/*! \file alcall.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KListener class is a wrapper around OpenAL listener for easy accessing and manipulating.
	/*!
		There is only one listener which represents where 'you' are, the user.
		The listener properties along with the source properties determine how the audio sample will be heard.
		by controlling the listener, the application controls the way the user experiences the virtual world,
		as the listener defines the position point and orientation, and other parameters that affect the output stream.
	*/
    class KITE_FUNC_EXPORT KListener{
    public:

		//! Set master global gain
		/*!
			Range of master gain [1, 100]
			by default 1

			\param Gain Pointer to image data.
		*/
        static void setMasterGain(F32 Gain);

		//! Get master global gain
		/*!
			\return Master gain
		*/
        static F32 getMasterGain();

		//! Set listener position
		/*!
			Pass (-1, 0, 0) or (1, 0, 0) for 2D panning
			by default (0, 0, 0)

			\param Position Listener position
		*/
        static void setPosition(const KVector3F32 &Position);

		//! Get listener position
		/*!
			\return Listener position
		*/
        static KVector3F32 getPosition();

		//! Set listener Orientation
		/*!
			by default (0, 0, 0)

			\param Orientation Listener orientation
		*/
        static void setOrientation(const KVector3F32 &Orientation);

		//! Get listener orientation
		/*!
			\return Listener orientation
		*/
        static KVector3F32 getOrientation();

    };
}

#endif // KLISTENER_H
