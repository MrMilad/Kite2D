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
#ifndef SDLCALL_H
#define SDLCALL_H

#include "Kite/core/kcoredef.h"
#include "sdl/SDL.h"

namespace Kite{
namespace Internal{
	/// inite sdl video/input/event
    extern void initeSDL();

	/// inite sdl sub-systems
	extern void initeSubSDL(U32 Flag);

	/// shutdown sdl
	extern void destroySDL();

	/// check sdl errors if any
    extern bool checkLastSDLErr();

/*! \def DSDL_CALL
\brief SDL debug macro.
\param call Any SDL function in debug mode
*/
#if defined(KITE_DEV_DEBUG)
#define DSDL_CALL(call) call; KDEBUG_ASSERT_T(Kite::Internal::checkLastSDLErr())
#else
#define DSDL_CALL(call) call;
#endif
}
}

#endif // SDLCALL_H
