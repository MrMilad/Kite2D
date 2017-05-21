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
#ifndef KCORETYPES_H
#define KCORETYPES_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "kcoretypes.khgen.h"

KMETA
namespace Kite{

	/* Power Sourece State */
	enum class PowerState : U8{
		ACON = 0,
		ACOFF,
		UNKNOWN
	};

	/* Battery State */
	/*enum KBatteryStateTypes{
		KBS_USING,
		KBS_CHARGING,
		KBS_NOBATTERY,
		KBS_UNKNOWN
	};*/

	KM_ENUM()
	enum class Primitive : U8 {
		I8 = 0,
		U8,
		I16,
		U16,
		I32,
		U32,
		I64,
		U64,
		F32,
		F64,
		BOOL,
		STR,
		VPTR,
		UNKNOWN
	};
	PRIMITIVE_BODY();


	enum class VectorComponent : U8{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};

	enum class CoreInstance : U8{
		IMAGE = 0,
		TEXTURE,
		AUDIOBUFFER,
		AUDIOSOURCE,
		VAO,
		VBO,
		IBO,
		PBO,
		FBO,
		SHADER,
		SHPROGRAM,
		WINDOW
	};

	/*enum class KRestypes : U8 {
		TEXTURE = 0,
		IMAGE,
		AUDIOBUFFER,
		AUDIOSOURCE,
		SHADER,
		SHPROGRAM,
		ATLAS,
		ANIMCLIP,
		SCRIPT,
		SCENE
	};*/

	enum class GridMapTrigger : U8 {
		ENTER,
		LEAVE
	};

	typedef void(*KCallGridMapTrigger)(void *Data, GridMapTrigger State);

	/*typedef U64(*KCallInputStreamRead)(void *Data, U64 DataSize);
	typedef I32(*KCallInputStreamSeek)(I64 Offset, I32 Origin);
	typedef I64(*KCallInputStreamTell)();
	typedef bool(*KCallInputStreamIsOpen)();
	typedef I32(*KCallInputStreamEOF)();
	typedef I32(*KCallInputStreamClose)();*/

}
#endif // KCORETYPES_H
