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
#ifndef KCOREUTIL_H
#define KCOREUTIL_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcorestructs.h"

namespace Kite{

	/// Sleep
	//KITE_FUNC_EXPORT extern void sleep(UL32 MicroSec);

	/// Time
	KITE_FUNC_EXPORT extern U32 getCurrentTimeSec();

	KITE_FUNC_EXPORT extern U32 getCurrentTimeMin();

	KITE_FUNC_EXPORT extern U32 getCurrentTimeHours();

	/// Power
	//KITE_FUNC_EXPORT extern KPowerState getPowerState();

	/// CPU
	//KITE_FUNC_EXPORT extern const KCPUInfo *getCPUInfo();

	/// unique identifier generator (1 to 18,446,744,073,709,551,615)
	/// not thread-safe
	KITE_FUNC_EXPORT extern U64 getUniqueNumber();

	KITE_FUNC_EXPORT extern F64 convertStrToDouble(const char *Value);

	KITE_FUNC_EXPORT extern bool isBigEndian();

	KITE_FUNC_EXPORT extern U64 hashPair(I32 First, I32 Second);

	KITE_FUNC_EXPORT extern U32 getHash32(const void *Data, U32 Lenght, U32 Seed);

	KITE_FUNC_EXPORT extern void getHash128(void *Data, U32 Lenght, U32 Seed, void *Out);

}

#endif // KCOREUTIL_H
