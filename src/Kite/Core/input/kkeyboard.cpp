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
#include "Kite/core/input/kkeyboard.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    KWindowHandle KKeyboard::_kwinHandle = 0;

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

