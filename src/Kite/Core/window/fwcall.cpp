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
