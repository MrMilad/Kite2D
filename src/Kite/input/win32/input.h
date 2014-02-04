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

#include <vector>
#include "Kite/input/win32/dicall.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/input/kinputstructs.h"

namespace Kite{
namespace Internal{
    class Input : KNonCopyable{
    public:

        static Input *CreateInstance();
        static void DestroyInstance();

        // initialize DirectInput 8
        // enumerates installed devices
        void setup(HWND WindowHandle);
        inline const KEnumInputDevice *getEnumDevices() const {return &_kenumDevice;}

        // mouse
        void activeMouse(bool Exclusive);
        const KMouseInput *getMouseInput();
        void releaseMouse();

        // keyboard
        void activeKeyboard(bool Exclusive);
        const KKeyboardInput *getKeyboardInput();
        void releaseKeyboard();

        // joystick
        void activeJoysticks(bool Exclusive);
        const KJoystickInput *getJoystickInput(U8 JoyID);
        void releaseJoysticks();

    private:

        // callback function for enumerating objects (axes, buttons, POVs, Keys)
        // set axes min/max values of Joystick.
        static BOOL CALLBACK _kenumObjCallb(const DIDEVICEOBJECTINSTANCE* pdidOInstance, VOID* pContext);

        // Callback function for enumerating input device (mouse, keyboard, joystick)
        BOOL _kenumDevCallbMember(const DIDEVICEINSTANCE *pdidInstance);
        static BOOL CALLBACK _kenumDevCallb(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext);

        Input();
        ~Input();

        static Input *_kinstance;           // class instance
        LPDIRECTINPUT8 _kdinput;            // direct input 8 device
        KEnumInputDevice _kenumDevice;      // enum input device
        HWND _kwhwnd;

        LPDIRECTINPUTDEVICE8 _kdmouse;      // DirectInput device (mouse)
        LPDIRECTINPUTDEVICE8 _kdkeyboard;   // DirectInput device (keyboard)
        LPDIRECTINPUTDEVICE8 _kdjoysticks[4];// DirectInput device (joystick - support 4 joysticks)

        // mouse
        KMouseInput _kmouseInput;
        DIMOUSESTATE _kdmouseState;

        // keyboard
        KKeyboardInput _kkeyboardInput;
        U8 _kdkeyboardState[256];

        // joystick
        KJoystickInput _kjoystickInput;
        DIJOYSTATE _kdjoysticState;
    };
}
}
#endif // INPUT_H
