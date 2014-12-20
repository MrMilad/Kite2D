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
#ifndef KSYSTEMUTIL_H
#define KSYSTEMUTIL_H

#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/system/ksystemtypes.h"
#include "Kite/core/system/ksystemstructs.h"

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
	KITE_FUNC_EXPORT extern const KCPUInfo *getCPUInfo();

	/// unique identifier generator (1 to 18,446,744,073,709,551,615)
	/// not thread-safe
	KITE_FUNC_EXPORT extern U64 getUniqueNumber();

}

#endif // KSYSTEMUTIL_H
