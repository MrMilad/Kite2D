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
#ifndef KANIMATIONBASE_H
#define KANIMATIONBASE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/animation/kanimationtypes.h"

namespace Kite {
	class KAnimationBase {
	public:
		virtual ~KAnimationBase();
		//! Set animation loop
		/*!
		loop apply at any direction (foreward and backwrad)

		\param Loop Animation loop
		*/
		inline void setLoop(bool Loop) { _kloop = Loop; }

		//! Get animation loop
		inline bool getLoop() const { return _kloop; }

		//! Set animation playback direction
		/*!
		\param Direction Animation playback direction
		*/
		inline void setPlayDirection(KAnimeDirectionTypes Direction) { _kdtype = Direction; }

		//! Get animation playback direction
		inline KAnimeDirectionTypes getPlayDirection() const { return _kdtype; }

		//! Set the animation playback state
		/*!
		\param State Animation playback state
		*/
		inline void setState(KAnimeStateTypes State) { _kstype = State; }

		//! Get the animation playback state
		inline KAnimeStateTypes getState() const { return _kstype; }

		//! Update animation state
		/*!
		Positive and negative values as Delta, are ineffective in play direction (use setPlayDirection())

		\param Delta Delta time
		*/
		virtual void update(F32 Delta) = 0;

	private:
		bool _kloop;
		KAnimeStateTypes _kstype;
		KAnimeDirectionTypes _kdtype;
	};
}

#endif // KANIMATIONBASE_H


