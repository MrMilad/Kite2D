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
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
namespace Internal{

    bool initeGLFW(){
        static bool isInit = false;
        if (isInit == false){
            KDEBUG_TEMP(glfwSetErrorCallback(glfwErrCallb));
            if (glfwInit()){
                isInit = true;
                return false;
            }else{
                KDEBUG_PRINT("glfwInite failed.");
            }
        }
        return isInit;
    }

    void glfwErrCallb(int error, const char* description){
        KDEBUG_TEMP(printf("glfw an error occurred.\nerror cod: %i\ndescription: %s",
                           error, description));
    }
}
}
