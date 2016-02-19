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
#include "Kite/Core/window/kwindowutil.h"
#include "src/Kite/Core/window/sdlcall.h"

namespace Kite{
    void getFullscreenStates(std::vector<KEnumDisplay> &DisplayList){
        Internal::initeSDL();
		DisplayList.clear();

		// Declare display mode structure to be filled in.
		SDL_DisplayMode current;
		DisplayList.reserve(SDL_GetNumVideoDisplays());

		// Get current display mode of all displays.
		for (I32 i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
			DSDL_CALL(SDL_GetCurrentDisplayMode(i, &current));
			KEnumDisplay edis;
			edis.width = current.w;
			edis.height = current.h;
			edis.refreshRate = current.refresh_rate;
			edis.colorDepth = current.format;
			DisplayList.push_back(edis);
		}
    }

    KEnumDisplay getDesktopState(){
		Internal::initeSDL();

		SDL_DisplayMode dm;
		DSDL_CALL(SDL_GetDesktopDisplayMode(0, &dm));

		KEnumDisplay edis;
		edis.width = dm.w;
		edis.height = dm.h;
		edis.refreshRate = dm.refresh_rate;
		edis.colorDepth = dm.format;

        return edis;
    }
}
