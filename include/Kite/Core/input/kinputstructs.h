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
#ifndef KINPUTSTRUCTS_H
#define KINPUTSTRUCTS_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
namespace Internal{
    struct KEnumInputDevice{
        bool mouse;
        bool keyboard;
        bool joystick;
        U8 joystickCount;
        KEnumInputDevice():
            mouse(false),
            keyboard(false),
            joystick(false),
            joystickCount(0)
        {}
    };
}

    // standard joystick state
    struct KJoystickInput{
        I64 xAxisAnalog;
        I64 yAxisAnalog;
        I64 zAxisAnalog;
        I64 rotXAxis;
        I64 rotYAxis;
        I64 rotZAxis;
        const IL32 *slider; // pointer to array[2]. two additional axes, formerly called the u-axis and v-axis.
        const DWORD *pov; // pointer to array[4].
        const U8 *buttons; // pointer to array[32].
        bool isChanged;
    };

}
#endif // KINPUTSTRUCTS_H
