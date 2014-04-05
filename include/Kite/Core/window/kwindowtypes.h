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
#ifndef KWINDOWTYPES_H
#define KWINDOWTYPES_H

#include "Kite/core/system/ksystemdef.h"

namespace Kite{

enum KAspectRatioTypes{
    KAR_4X3,
    KAR_16X9,
    KAR_16X10,
    KAR_UNKNOWN
};

/* Callbacks */
enum KWindowCallbackTypes{
    KWC_ALL = 0, // use only for unregistering callback
    KWC_SIZECHANGE,
    KWC_POSCHANGE,
    KWC_FOCUSCHANGE,
    KWC_CLOSE,
    KWC_FBCHANGE, // frame buffer
};

typedef void (*KCallWinSizeChange)(KWindowHandle WinHandle, I32 Width, I32 Height);
typedef void (*KCallWinPosChange)(KWindowHandle WinHandle, I32 Top, I32 Left);
typedef void (*KCallWinFBChange)(KWindowHandle WinHandle, I32 Width, I32 Height);
typedef void (*KCallWinFocusChange)(KWindowHandle WinHandle, bool Focus);
typedef void (*KCallWinClose)(KWindowHandle WinHandle); // return false = close window

}

#endif // KWINDOWTYPES_H
