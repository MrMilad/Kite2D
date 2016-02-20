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

#include <vector>
#include <utility>
#include "Kite/core/system/krefvariant.h"

namespace Kite {
	typedef std::pair<F32, KRefVariant> KAnimKey;
	typedef std::vector<KAnimKey> KAnimKeys;


	/*! \enum KAnimeStateTypes
	\brief Current state of a animation controller
	*/
	enum KAnimeStateTypes {
		KAS_PLAY,	//!< Play
		KAS_PAUSE,	//!< Pause
		KAS_STOP	//!< Stop
	};

	/*! \enum KAnimeDirectionTypes
	\brief Direction of the animation
	*/
	enum KAnimeDirectionTypes {
		KAD_FOREWARD,
		KAD_BACKWARD
	};

	/*! \enum KAnimeValueChangeTypes
	\brief Property animation methodes
	*/
	enum KAnimeValueChangeTypes {
		KAV_SET,	//!< override current value(s)
		KAV_APPEND	//!< append to current value(s) (recomended: use + and - value(s) when using KAV_APPEND)
	};

	/*enum KAnimeCallbackTypes{
	KAC_ATTACHED,	//!< When attach to AnimeController list
	KAC_DETACHED	//!< When detach from AnimeController list
	};*/

	enum KAnimeClipFileTypes {
		KAF_ANIMEKEY,	//!< Default KAtlas format (compact format in speed and size)
		KAF_XML	//!< Adobe Flash created JSON 
	};

	/*! \typedef KCallAnimeTrigger
	\brief Function handle for animation triggers
	*/
	typedef void(*KCallAnimeTrigger)(void *Parameter);

	/*! \typedef KCallKeyChange
	\brief Function handle for animation key change
	*/
	typedef void(*KCallKeyChange)(void *Parameter, U32 KeyNumber);

	/*! \typedef KCallAnimeStatechange
	\brief Function handle for animation states
	*/
	typedef void(*KCallAnimeStatechange)(void *Parameter);
}