#ifndef KWINDOWTYPES_H
#define KWINDOWTYPES_H

#include "Kite/system/ksystemdef.h"

namespace Kite{

/* ColorDepth */
enum KColorDepthTypes{
    KCOLOR_DEPTH_4 = 4,
    KCOLOR_DEPTH_8 = 8,
    KCOLOR_DEPTH_16 = 16,
    KCOLOR_DEPTH_24 = 24,
    KCOLOR_DEPTH_32 = 32,
    KCOLOR_DEPTH_OUT = 0
};

enum KAspectRatioTypes{
    KASPRAT_4X3,
    KASPRAT_16X9,
    KASPRAT_16X10,
    KASPRAT_UNKNOWN
};

/* Callbacks */
enum KWindowCallbackTypes{
    KCALL_WIN_SIZECHANGE,
    KCALL_WIN_POSCHANGE,
    KCALL_WIN_FOCUSCHANGE,
    KCALL_WIN_CLOSE,
    KCALL_WIN_ALL // use only for unregistering callback
};

typedef void (KCallWinSizeChange)(const U32 Width, const U32 Height);
typedef void (KCallWinPositionChange)(const I32 Top, const I32 Left);
typedef void (KCallWinFocusChange)(const bool Focus);
typedef bool (KCallWinClose)(); // return false = close window

}

#endif // KWINDOWTYPES_H
