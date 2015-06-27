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
#include "Kite/core/input/kkeyboard.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    KWindowHandle KKeyboard::_kwinHandle = 0;

	void KKeyboard::setWindowHandle(KWindowHandle Window){
		_kwinHandle = Window; 
	}

    KButtonStateTypes KKeyboard::getButtonState(Kite::KKeyboardKeyTypes Button){
        return (KButtonStateTypes)glfwGetKey((GLFWwindow *)_kwinHandle, Button);
    }

    void KKeyboard::registerCallback(void *Callback, KKeyboardCallbackTypes CallbackType){
        switch (CallbackType){
        case KKC_KEY:
            glfwSetKeyCallback((GLFWwindow *)_kwinHandle, (GLFWkeyfun) Callback);
            break;
        case KKC_UNICODE:
            glfwSetCharCallback((GLFWwindow *)_kwinHandle, (GLFWcharfun) Callback);
            break;
        default:
            KDEBUG_PRINT("invalid keyboard callback type");
            break;
        }
    }

    void KKeyboard::unregisterCallback(KKeyboardCallbackTypes CallbackType){
        switch (CallbackType){
        case KKC_ALL:
            glfwSetKeyCallback((GLFWwindow *)_kwinHandle, 0);
            glfwSetCharCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KKC_KEY:
            glfwSetKeyCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        case KKC_UNICODE:
            glfwSetCharCallback((GLFWwindow *)_kwinHandle, 0);
            break;
        default:
            KDEBUG_PRINT("invalid keyboard callback type");
            break;
        }
    }
}

