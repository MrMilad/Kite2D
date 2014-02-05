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
#ifndef INPUT_H
#define INPUT_H

// we using DirectInput only for joystick(s)
#include "Kite/input/win32/dicall.h"
#include <windows.h>
#include "Kite/system/knoncopyable.h"
#include "Kite/system/kvector2.h"
#include "Kite/input/kinputstructs.h"
#include "Kite/input/kinputtypes.h"

namespace Kite{
namespace Internal{
    class Input : KNonCopyable{
    public:

        // enumerates installed devices (mouse and keyboard)
        static const KEnumInputDevice *getEnumDevices();

        // mouse
        static bool getMouseButton(KMouseButtonTypes Button);
        static KVector2I32 getMousePosition(KMousePositionTypes Position, HWND Window);

        // keyboard
        static bool getKeyboardButton(KKeyboardButtonTypes Button);

        // joystick
        // initialize DirectInput 8
        // enumerates installed devices (joystick(s))
        static void activeJoysticks(bool Exclusive, HWND Window);
        static const KJoystickInput *getJoystickInput(U8 JoyID);
        static void releaseJoysticks();

    private:

        Input();
        ~Input();

        static KEnumInputDevice _kenumDevice;          // enum input device (only joystick(s))

        // we use DirectInput only for joystick(s)
        #ifdef KDINPUT_ALLOW
        // callback function for enumerating objects (axes, buttons, POVs, Keys)
        // set axes min/max values of Joystick.
        static BOOL CALLBACK _kenumObjCallb(const DIDEVICEOBJECTINSTANCE* pdidOInstance, VOID* pContext);

        // Callback function for enumerating input device (mouse, keyboard, joystick)
        static BOOL CALLBACK _kenumDevCallb(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext);

        static LPDIRECTINPUT8 _kdinput;                // direct input 8 device
        static LPDIRECTINPUTDEVICE8 _kdjoysticks[4];   // DirectInput device (joystick - support 4 joysticks)

        // joystick
        static DIJOYSTATE _kdjoysticState;
        #endif
        static KJoystickInput _kjoystickInput;
    };
}
}
#endif // INPUT_H
