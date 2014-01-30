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

// unique identifier generator (1 to 18,446,744,073,709,551,615)
// not thread-safe
extern "C" { KITE_FUNC_EXPORT U64 getUniqueNumber(); }

}

#endif // KSYSTEMUTIL_H
