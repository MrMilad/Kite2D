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
    KCALL_WIN_ALL // unregister callback only
};

typedef void (KCallWinSizeChange)(const U32 Width, const U32 Height);
typedef void (KCallWinPositionChange)(const I32 Top, const I32 Left);
typedef void (KCallWinFocusChange)(const bool Focus);
typedef bool (KCallWinClose)(); // return false = close window

}

#endif // KWINDOWTYPES_H
