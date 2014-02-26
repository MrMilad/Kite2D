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
#ifndef DICALL_H
#define DICALL_H

#include "Kite/core/system/ksystemdef.h"

#ifdef KDINPUT_ALLOW // using DirectInput
#define DIRECTINPUT_VERSION 0x0800
#define INITGUID
#include <dinput8/dinput.h>

// DirectInput debug macro
#if defined(KITE_DEV_DEBUG)
    #define DDI_CALL(call) if (call != DI_OK) {KDEBUG_PRINT(call); KDEBUG_BREAK}
#else
    #define DDI_CALL(call) call;
#endif

#endif

#endif // DICALL_H
