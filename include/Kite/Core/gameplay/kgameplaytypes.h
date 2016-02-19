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
#ifndef KGAMEPLAYTYPES_H
#define KGAMEPLAYTYPES_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite {

	enum class KComponentTypes : U8 {
		KCT_GRAPHIC = 0,
		KCT_AUDIO,
		KCT_INPUT,
		KCT_TRANSFORM,
		KCT_MAX_COMP_SIZE,
	};
	// KCT_LOGIC // logic (scripts) components stored in another place

	enum KMessageScopeTypes {
		KMS_SELF,
		KMS_CHILDREN,
		KMS_ALL
	};
}

#endif // KGAMEPLAYTYPES_H