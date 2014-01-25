#ifndef KWINDOWUTIL_H
#define KWINDOWUTIL_H

#include "Kite/window/kwindowstructs.h"

namespace Kite{

extern "C" { KITE_FUNC_EXPORT const KEnumDisplayList *getFullscreenStates(); }
extern "C" { KITE_FUNC_EXPORT const KEnumDisplay *getDesktopState(); }
extern "C" { KITE_FUNC_EXPORT bool isValidDisplay(const KEnumDisplayList &DisplayList,
                                                  const KEnumDisplay &Display); }

// fill a KWindowState from KEnumDisplay
extern "C" { KITE_FUNC_EXPORT void convertEnumToWin(const KEnumDisplay &EnumDisplay,
                                                    KWindowState &WindowState); }
}

#endif // KWINDOWUTIL_H
