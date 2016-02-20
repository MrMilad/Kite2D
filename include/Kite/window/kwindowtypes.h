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
#ifndef KWINDOWTYPES_H
#define KWINDOWTYPES_H

#include "Kite/core/kcoredef.h"

namespace Kite{

enum KAspectRatioTypes{
    KAR_4X3,
    KAR_16X9,
    KAR_16X10,
    KAR_UNKNOWN
};

/* Callbacks */
enum KWindowEventType{
	KWE_SHOWN = 0, // window has been shown
	KWE_HIDDEN,
	KWE_EXPOSED,
	KWE_MOVED,
	KWE_RESIZED,
	KWE_SIZE_CHANGED,
	KWE_MINIMIZED,
	KWE_MAXIMIZED,
	KWE_RESTORED,
	KWE_ENTER,
	KWE_LEAVE,
	KWE_FOCUS_GAINED,
	KWE_FOCUS_LOST,
	KWE_CLOSE,
	KWE_UNKNOWN
};

typedef void (*KCallWindowEvent)(KWindowEventType EventType);

}

#endif // KWINDOWTYPES_H
