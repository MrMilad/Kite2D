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
#include "src/Kite/window/sdlcall.h"

namespace Kite{
namespace Internal{

    void initeSDL(){
		static bool inite = false;
        if (!inite){
			DSDL_CALL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS));
			inite = true;
        }
    }

	void initeSubSDL(U32 Flag) {
		if ((SDL_WasInit(Flag) == 0)) {
			DSDL_CALL(SDL_InitSubSystem(Flag));
		}
	}

	void destroySDL() {
		DSDL_CALL(SDL_Quit());
	}

	bool checkLastSDLErr() {
		if ((*SDL_GetError()) != '\0') {
			KDEBUG_PRINT(SDL_GetError());
			SDL_ClearError();
			return false;
		}
		return true;
	}
}
}
