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
#include "Kite/input/kmouse.h"
#include "src/Kite/window/sdlcall.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	KVector2I32 KMouse::_kwheelVal;
	U16 KMouse::_kcount = 0;
	void KMouse::initeMouse() {
		Internal::initeSDL();
		// add our watcher for handling mouse wheel
		DSDL_CALL(SDL_AddEventWatch(KMouse::_eventWatcher, NULL));
	}

	bool KMouse::isAnyKeyDown() {
		if (_kcount > 0)
			return true;

		return false;
	}

	bool KMouse::isButtonPressed(MouseButton Button) {
		auto button = DSDL_CALL(SDL_GetMouseState(NULL, NULL));
		switch (Button) {
		case MouseButton::LEFT:
			if (button & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				return true;
			}
			break;
		case MouseButton::RIGHT:
			if (button & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				return true;
			}
			break;
		case MouseButton::MIDDLE:
			if (button & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
				return true;
			}
			break;
		case MouseButton::X1:
			if (button & SDL_BUTTON(SDL_BUTTON_X1)) {
				return true;
			}
			break;
		case MouseButton::X2:
			if (button & SDL_BUTTON(SDL_BUTTON_X2)) {
				return true;
			}
			break;
		}

		return false;
	}


    KVector2I32 KMouse::getPosition(){
		KVector2I32 pos;
		DSDL_CALL(SDL_GetMouseState(&pos.x, &pos.y));
        return pos;
    }

	KVector2I32 KMouse::getWheelValue() {
		KVector2I32 temp = _kwheelVal;
		_kwheelVal.x = 0;
		_kwheelVal.y = 0;
		return temp;
	}

    void KMouse::setWindowPosition(const KVector2I32 &Position, KWindowHandle Handle){
		DSDL_CALL(SDL_WarpMouseInWindow((SDL_Window *) Handle,Position.x, Position.y));
    }

	void KMouse::setGlobalPosition(const KVector2I32 &Position) {
		DSDL_CALL(SDL_WarpMouseGlobal(Position.x, Position.y));
	}

	int KMouse::_eventWatcher(void *Data, SDL_Event *Event) {
		if (Event->type == SDL_MOUSEWHEEL) {
			_kwheelVal.x = Event->wheel.x;
			_kwheelVal.y = Event->wheel.y;
		}

		if (Event->type == SDL_MOUSEBUTTONDOWN) {
			++_kcount;
		} else if (Event->type == SDL_MOUSEBUTTONUP) {
			--_kcount;
		}
		return 0;
	}

	KMETA_KMOUSE_SOURCE();
}
