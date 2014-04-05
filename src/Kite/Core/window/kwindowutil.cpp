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
#include "Kite/Core/window/kwindowutil.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    std::vector<KEnumDisplay> getFullscreenStates(){
        Internal::initeGLFW();

        I32 count = 0;
        std::vector<KEnumDisplay> vdis;
        const GLFWvidmode *vid = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

        if (vid){
            KEnumDisplay dis;
            for (U16 i = 0; i < count; i++){
                dis.width = vid[i].width;
                dis.height = vid[i].height;
                dis.refreshRate = vid[i].refreshRate;
                dis.colorDepth = vid[i].redBits + vid[i].greenBits + vid[i].blueBits;
                vdis.push_back(dis);
            }
        }else{
            KDEBUG_PRINT("glfwGetVideoModes failed.");
        }
        return vdis;
    }

    KEnumDisplay getDesktopState(){
        Internal::initeGLFW();

        KEnumDisplay dis;
        const GLFWvidmode *vid = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (vid){
            dis.width = vid->width;
            dis.height = vid->height;
            dis.refreshRate = vid->refreshRate;
            dis.colorDepth = vid->redBits + vid->greenBits + vid->blueBits;
        }else{
            KDEBUG_PRINT("glfwGetVideoMode failed.");
        }

        return dis;
    }
}
