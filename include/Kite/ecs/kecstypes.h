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
#ifndef KECSTYPES_H
#define KECSTYPES_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "kecstypes.khgen.h"

KMETA
namespace Kite{

	KM_ENUM()
	enum class RecieveTypes : U8 {
		RECEIVED = 0,
		IGNORED
	};
	KMETA_RECIEVETYPES_BODY();

	/*enum class KComTypes : U8 {
		KCT_GRAPHIC = 0,
		KCT_AUDIO,
		KCT_INPUT,
		KCT_TRANSFORM,
		KCT_LOGIC,
		KCT_MAX_COMP_SIZE,
	};*/

	KM_ENUM()
	enum class MessageScope : U8 {
		SELF = 0,
		CHILDREN,
		ALL
	};
	KMETA_MESSAGESCOPE_BODY();

	/*typedef U64(*KCallInputStreamRead)(void *Data, U64 DataSize);
	typedef I32(*KCallInputStreamSeek)(I64 Offset, I32 Origin);
	typedef I64(*KCallInputStreamTell)();
	typedef bool(*KCallInputStreamIsOpen)();
	typedef I32(*KCallInputStreamEOF)();
	typedef I32(*KCallInputStreamClose)();*/

}
#endif // KCORETYPES_H
