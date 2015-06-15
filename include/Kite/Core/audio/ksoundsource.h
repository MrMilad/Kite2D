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
#ifndef KSOUNDSOURCE_H
#define KSOUNDSOURCE_H

/*! \file ksoundsource.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/audio/kaudiotypes.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KSoundSource class provide abstract base class for diffrent types of sound source.
	/*!
		Sources specify attributes like position, velocity, and a buffer with sample data.
		by controlling a source's attributes the application can modify and parameterize
		the static sample data provided by the buffer referenced by the source
	*/
    class KITE_FUNC_EXPORT KSoundSource{
    public:

		//! Default constructors
        KSoundSource();

		//! Destructors
        virtual ~KSoundSource();
        
		//! Play sound
        virtual void play() = 0;

		//! Pause sound
        virtual void pause() = 0;

		//! Stop sound
        virtual void stop() = 0;

		//! Set loop
		/*!
			Specifies what should happen when the end of a buffer queue is reached.
			false by default

			\param Loop Loop
		*/
        virtual void setLoop(bool Loop) = 0;

		//! Get loop
		/*!
			\return Loop
		*/
        virtual bool getLoop() = 0;

		//! Set Releative
		/*!
			Make the sound's position relative to the listener or absolute
			when set to true indicates that the position, velocity,
			cone, and direction properties of a source are to be interpreted relative to
			the listener position.
			false by default

			\Releative Releative Releative
		*/
        void setReleative(bool Releative);

		//! Get Releative
		/*!
			\return Releative
		*/
        bool getReleative();

        /// pitch
        /// default 1

		//! Set pitch
		/*!
			When pitch is set higher than original,
			openal speeds up the sample by the multiplier
			using some sort of interpolation when the multiplier is not whole.
			1 by default

			\param Pitch Pitch
		*/
        void setPitch(F32 Pitch);

		//! Get pitch
		/*!
			\return Pitch
		*/
        F32 getPitch();

		//! Set volume
		/*!
			100 by default. range [0, 100]

			\param Volume Volume
		*/
        void setVolume(F32 Volume);

		//! Get volume
		/*!
			\return Volume
		*/
        F32 getVolume();

		//! Set rollof factor
		/*!
			This is used for distance attenuation calculations based on inverse distance with rolloff.
			pass 0 for simulate 2D panning.
			1 by default.

			\param Factor Rollof factor
		*/
        void setRolloffFactor(F32 Factor);

		//! Get rollof factor
		/*!
			\return Rollof factor
		*/
        F32 getRolloffFactor();

		//! Set min distance
		/*!
			This is used for distance attenuation calculations based on inverse distance with rolloff.
			depending on the distance model it will also act as a distance threshold below which gain is clamped.
			always positive value. 1 by default.

			\param Distance Distance
		*/
        void setMinDistance(F32 Distance);

		//! Get min distance
		/*!
			\return Distance
		*/
        F32 getMinDistance();

		//! Set position
		/*!
			Specifies the current location of the object in the world coordinate system.
			(0, 0, 0) by default

			\param Position Position
		*/
        void setPosition(KVector3F32 Position);

		//! Get position
		/*!
			\return Position
		*/
        KVector3F32 getPosition();

		//! Get current state of the source
        KSourceStateTypes getState();

		//! Get OpenAL ID of the buffer
		/*!
			\return OpenAL ID of the buffer
		*/
        inline U32 getID() const {return _kID;}

    protected:
        U32 _kID;	//!< Name of the source
    };
}

#endif // KSOUNDSOURCE_H
