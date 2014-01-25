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
