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
#include "Kite/core/input/kmouse.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    KWindowHandle KMouse::_kwinHandle = 0;

	void KMouse::setWindowHandle(KWindowHandle Window){
		_kwinHandle = Window; 
	}

    KButtonStateTypes KMouse::getButtonState(KMouseButtonTypes Button){
        return (KButtonStateTypes)glfwGetMouseButton((GLFWwindow *)_kwinHandle, Button);
    }

    KVector2F64 KMouse::getPosition(){
        KVector2F64 pos;
        glfwGetCursorPos((GLFWwindow *)_kwinHandle, &pos.x, &pos.y);
        return pos;
    }

    void KMouse::setPosition(const KVector2F64 &Position){
        glfwSetCursorPos((GLFWwindow *)_kwinHandle, Position.x, Position.y);
    }

    void KMouse::registerCallback(void *Callback, KMouseCallbackTypes CallbackType){
        switch (CallbackType){
        case KMC_MBUTTON:
            glfwSetMouseButtonCallback((GLFWwindow *)_kwinHandle, (GLFWmousebuttonfun) Callback);
            break;
        case KMC_MENTER:
            glfwSetCursorEnterCallback((GLFWwindow *)_kwinHandle, (GLFWcursorenterfun) Callback);
            break;
        case KMC_MPOSITION:
            glfwSetCursorPosCallback((GLFWwindow *)_kwinHandle, (GLFWcursorposfun) Callback);
            break;
        case KMC_MSCROLL:
            glfwSetScrollCallback((GLFWwindow *)_kwinHandle, (GLFWscrollfun) Callback);
            break;
        default:
            KDEBUG_PRINT("invalid mouse callback type");
            break;
        }
    }

    void KMouse::unregisterCallback(Kite::KMouseCallbackTypes CallbackType){
        switch (CallbackType){
        case KMC_ALL:
            glfwSetMouseButtonCallback((GLFWwindow *)_kwinHandle, 0);
            glfwSetCursorEnterCallback((GLFWwindow *)_kwinHandle, 0);
            glfwSetCursorPosCallback((GLFWwindow *)_kwinHandle, 0);
            glfwSetScrollCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KMC_MBUTTON:
            glfwSetMouseButtonCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KMC_MENTER:
            glfwSetCursorEnterCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KMC_MPOSITION:
            glfwSetCursorPosCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KMC_MSCROLL:
            glfwSetScrollCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        default:
            KDEBUG_PRINT("invalid mouse callback type");
            break;
        }
    }
}
