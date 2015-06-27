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
#include "Kite/core/input/kmouse.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    KWindowHandle KMouse::_kwinHandle = 0;

	void KMouse::setWindowHandle(KWindowHandle Window){
		_kwinHandle = Window; 
	}

	void KMouse::setMouseVisible(bool Visible){
		KDEBUG_ASSERT_T(_kwinHandle);
		if (Visible){
			glfwSetInputMode((GLFWwindow *)_kwinHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else{
			glfwSetInputMode((GLFWwindow *)_kwinHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

    KButtonStateTypes KMouse::getButtonState(KMouseButtonTypes Button){
		KDEBUG_ASSERT_T(_kwinHandle);
        return (KButtonStateTypes)glfwGetMouseButton((GLFWwindow *)_kwinHandle, Button);
    }

    KVector2F64 KMouse::getPosition(){
		KDEBUG_ASSERT_T(_kwinHandle);
        KVector2F64 pos;
		glfwGetCursorPos((GLFWwindow *)_kwinHandle, &pos.x, &pos.y);
        return pos;
    }

    void KMouse::setPosition(const KVector2F64 &Position){
		KDEBUG_ASSERT_T(_kwinHandle);
        glfwSetCursorPos((GLFWwindow *)_kwinHandle, Position.x, Position.y);
    }

    void KMouse::registerCallback(void *Callback, KMouseCallbackTypes CallbackType){
		KDEBUG_ASSERT_T(_kwinHandle);
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
		KDEBUG_ASSERT_T(_kwinHandle);
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
