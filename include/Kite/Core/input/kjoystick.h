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
#ifndef KJOYSTICK_H
#define KJOYSTICK_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/input/kinputtypes.h"
#include <string>
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KJoystick{
    public:

        /// returns the values of all axes
        static const std::vector<F32> &getAxes(KJoystcikIDTypes JoystickID);

        /// returns the state of all buttons
        static const std::vector<U8> &getButtonsState(KJoystcikIDTypes JoystickID);

        /// returns whether the specified joystick is present
        static bool isInstalled(KJoystcikIDTypes JoystickID);

        /// returns the name of the specified joystick
        static std::string getName(KJoystcikIDTypes JoystickID);
    };
}

#endif // KJOYSTICK_H
