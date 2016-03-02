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
#include "Kite/core/input/kjoystick.h"
#include "src/Kite/window/fwcall.h"

namespace Kite{

    const std::vector<F32> &KJoystick::getAxes(KJoystcikIDTypes JoystickID){
        int count = 0;
        static std::vector<F32> axesArr;
        axesArr.clear();

        const F32 *axes = glfwGetJoystickAxes(JoystickID, &count);

        if (count)
            axesArr.assign(axes, axes + count);

        return axesArr;
    }

    const std::vector<U8> &KJoystick::getButtonsState(KJoystcikIDTypes JoystickID){
        int count = 0;
        static std::vector<U8> buttArr;
        buttArr.clear();

        const U8 *buttons = glfwGetJoystickButtons(JoystickID, &count);

        if (count)
            buttArr.assign(buttons, buttons + count);

        return buttArr;
    }

    bool KJoystick::isInstalled(KJoystcikIDTypes JoystickID){
		if (glfwJoystickPresent(JoystickID) == GL_TRUE)
			return true;
		return false;
    }

    std::string KJoystick::getName(KJoystcikIDTypes JoystickID){
        std::string strName;
        const char *name = glfwGetJoystickName(JoystickID);
        strName.assign(name);
        return strName;
    }

}