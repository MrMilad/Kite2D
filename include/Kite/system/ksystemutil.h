/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KSYSTEMUTIL_H
#define KSYSTEMUTIL_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/ksystemtypes.h"
#include "Kite/system/ksystemstructs.h"

namespace Kite{

// Sleep
extern "C" { KITE_FUNC_EXPORT void sleep(U64 MicroSec); }

// Time
extern "C" { KITE_FUNC_EXPORT U32 getCurrentTimeSec(); }
extern "C" { KITE_FUNC_EXPORT U32 getCurrentTimeMin(); }
extern "C" { KITE_FUNC_EXPORT U32 getCurrentTimeHours(); }

// Power
extern "C" { KITE_FUNC_EXPORT KPowerState getPowerState(); }

// CPU
extern "C" { KITE_FUNC_EXPORT const KCPUInfo *getCPUInfo(); }

}

#endif // KSYSTEMUTIL_H
