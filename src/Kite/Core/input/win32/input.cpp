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
#include "Kite/core/input/win32/input.h"

namespace Kite{
namespace Internal{

#ifdef KDINPUT_ALLOW
    BOOL CALLBACK Input::_kenumDevCallb(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext){
        // we use DirectInput only for joystick(s)
        switch( 0xff & pdidInstance->dwDevType ){
        case DI8DEVTYPE_GAMEPAD:
        case DI8DEVTYPE_JOYSTICK:
            // we have to create joystick(s) device here.
            // because we need GUID of joystick(s).
            DDI_CALL(_kdinput->CreateDevice(pdidInstance->guidInstance,
                                            &_kdjoysticks[_kenumDevice.joystickCount],
                                            NULL));
            _kenumDevice.joystick = true;
            ++_kenumDevice.joystickCount;
            break;
        }
        return DIENUM_CONTINUE;
    }

    BOOL CALLBACK Input::_kenumObjCallb( const DIDEVICEOBJECTINSTANCE* pdidOInstance, VOID* pContext ){
        // for axes that are returned, set the DIPROP_RANGE property for the
        // enumerated axis in order to scale min/max values.
        if( pdidOInstance->dwType & DIDFT_AXIS ){
            DIPROPRANGE diprg;
            diprg.diph.dwSize       = sizeof(DIPROPRANGE);
            diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            diprg.diph.dwHow        = DIPH_BYID;
            diprg.diph.dwObj        = pdidOInstance->dwType; // Specify the enumerated axis
            diprg.lMin              = - 1000;
            diprg.lMax              = 1000;

            // Set the range for the axis
            LPDIRECTINPUTDEVICE8 joystick = (LPDIRECTINPUTDEVICE8)pContext;
            DDI_CALL(joystick->SetProperty(DIPROP_RANGE, &diprg.diph));
        }

        // Set the UI to reflect what objects the device supports
//      if (pdidOInstance->guidType == GUID_XAxis)  {rDev->xAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_YAxis)  {rDev->yAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_ZAxis)  {rDev->zAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_RxAxis) {rDev->rotXAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_RyAxis) {rDev->rotYAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_RzAxis) {rDev->rotZAxis = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_Slider) {++rDev->slider; rDev->slider = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_POV)    {++rDev->pov; rDev->pov = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_Button) {++rDev->buttonCount; rDev->button = true; return DIENUM_CONTINUE;}
//      if (pdidOInstance->guidType == GUID_Key)    {++rDev->keyCount; rDev->key = true; return DIENUM_CONTINUE;}
        return DIENUM_CONTINUE;
    }
#endif

    // initialize static objects
    KEnumInputDevice Input::_kenumDevice;
    #ifdef KDINPUT_ALLOW
    LPDIRECTINPUT8 Input::_kdinput = 0;
    LPDIRECTINPUTDEVICE8 Input::_kdjoysticks[4] = {0};
    DIJOYSTATE Input::_kdjoysticState;
    #endif
    KJoystickInput Input::_kjoystickInput;

    const KEnumInputDevice *Input::getEnumDevices(){
        UINT nDevices;
        PRAWINPUTDEVICELIST pRawDevList;

        // retrieve the number of device(s)
        if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) {KDEBUG_BREAK;}

        // allocate enough size
        pRawDevList = new RAWINPUTDEVICELIST[ sizeof( RAWINPUTDEVICELIST ) * nDevices ];

        // get device list
        if (GetRawInputDeviceList(pRawDevList, &nDevices, sizeof(RAWINPUTDEVICELIST)) < 0) {KDEBUG_BREAK;}

        // enumrates device(s)
        for (unsigned int i = 0; i < nDevices; ++i){
            switch (pRawDevList[i].dwType) {
            case RIM_TYPEMOUSE:
                _kenumDevice.mouse = true;
                break;
            case RIM_TYPEKEYBOARD:
                _kenumDevice.keyboard = true;
                break;
            default: // joystick(s) enumrate with DirectInput
                break;
            }
        }

        // free the RAWINPUTDEVICELIST
        delete[] pRawDevList;

        return &_kenumDevice;
    }

    bool Input::getMouseButton(KMouseButtonTypes Button){
        int vkey = 0;
        switch (Button){
        case KMB_LEFT:
            vkey = VK_LBUTTON;
            break;
        case KMB_RIGHT:
            vkey = VK_RBUTTON;
            break;
        case KMB_MIDDLE:
            vkey = VK_MBUTTON;
            break;
        case KMB_X1:
            vkey = VK_XBUTTON1;
            break;
        case KMB_X2:
            vkey = VK_XBUTTON2;
            break;
        default:
            vkey = 0;
            break;
        }

        return (GetAsyncKeyState(vkey) & 0x8000) != 0;
    }

    KVector2I32 Input::getMousePosition(KMousePositionTypes Position, HWND Window){
        POINT mPoint;
        KVector2I32 mPos;

        // get screen position
        GetCursorPos(&mPoint);
        mPos.x = mPoint.x;
        mPos.y = mPoint.y;
        if (Position == KMP_SCREEN)
            return mPos;

        // get window position
        ScreenToClient(Window, &mPoint);
        mPos.x = mPoint.x;
        mPos.y = mPoint.y;
        return mPos;
    }

    bool Input::getKeyboardButton(KKeyboardButtonTypes Button){
        int vKey = 0;
        switch (Button) {
        case KKB_1:             vKey = '1';    break;
        case KKB_2:             vKey = '2';    break;
        case KKB_3:             vKey = '3';    break;
        case KKB_4:             vKey = '4';    break;
        case KKB_5:             vKey = '5';    break;
        case KKB_6:             vKey = '6';    break;
        case KKB_7:             vKey = '7';    break;
        case KKB_8:             vKey = '8';    break;
        case KKB_9:             vKey = '9';    break;
        case KKB_0:             vKey = '0';    break;
        case KKB_A:             vKey = 'A';    break;
        case KKB_B:             vKey = 'B';    break;
        case KKB_C:             vKey = 'C';    break;
        case KKB_D:             vKey = 'D';    break;
        case KKB_E:             vKey = 'E';    break;
        case KKB_F:             vKey = 'F';    break;
        case KKB_G:             vKey = 'G';    break;
        case KKB_H:             vKey = 'H';    break;
        case KKB_I:             vKey = 'I';    break;
        case KKB_J:             vKey = 'G';    break;
        case KKB_K:             vKey = 'K';    break;
        case KKB_L:             vKey = 'L';    break;
        case KKB_M:             vKey = 'M';    break;
        case KKB_N:             vKey = 'N';    break;
        case KKB_O:             vKey = 'O';    break;
        case KKB_P:             vKey = 'P';    break;
        case KKB_Q:             vKey = 'Q';    break;
        case KKB_R:             vKey = 'R';    break;
        case KKB_S:             vKey = 'S';    break;
        case KKB_T:             vKey = 'T';    break;
        case KKB_U:             vKey = 'U';    break;
        case KKB_V:             vKey = 'V';    break;
        case KKB_W:             vKey = 'W';    break;
        case KKB_X:             vKey = 'X';    break;
        case KKB_Y:             vKey = 'Y';    break;
        case KKB_Z:             vKey = 'Z';    break;
        case KKB_F1:            vKey = VK_F1;    break;
        case KKB_F2:            vKey = VK_F2;    break;
        case KKB_F3:            vKey = VK_F3;    break;
        case KKB_F4:            vKey = VK_F4;    break;
        case KKB_F5:            vKey = VK_F5;    break;
        case KKB_F6:            vKey = VK_F6;    break;
        case KKB_F7:            vKey = VK_F7;    break;
        case KKB_F8:            vKey = VK_F8;    break;
        case KKB_F9:            vKey = VK_F9;    break;
        case KKB_F10:           vKey = VK_F10;    break;
        case KKB_F11:           vKey = VK_F11;    break;
        case KKB_F12:           vKey = VK_F12;    break;
        case KKB_F13:           vKey = VK_F13;    break;
        case KKB_F14:           vKey = VK_F14;    break;
        case KKB_F15:           vKey = VK_F15;    break;
        case KKB_NUMPAD0:       vKey = VK_NUMPAD0;    break;
        case KKB_NUMPAD1:       vKey = VK_NUMPAD1;    break;
        case KKB_NUMPAD2:       vKey = VK_NUMPAD2;    break;
        case KKB_NUMPAD3:       vKey = VK_NUMPAD3;    break;
        case KKB_NUMPAD4:       vKey = VK_NUMPAD4;    break;
        case KKB_NUMPAD5:       vKey = VK_NUMPAD5;    break;
        case KKB_NUMPAD6:       vKey = VK_NUMPAD6;    break;
        case KKB_NUMPAD7:       vKey = VK_NUMPAD7;    break;
        case KKB_NUMPAD8:       vKey = VK_NUMPAD8;    break;
        case KKB_NUMPAD9:       vKey = VK_NUMPAD9;    break;
        case KKB_ADD:           vKey = VK_ADD;    break;
        case KKB_BACK:          vKey = VK_BACK;    break;
        case KKB_BACKSLASH:     vKey = VK_OEM_5;    break;
        case KKB_COMMA:         vKey = VK_OEM_COMMA;    break;
        case KKB_DASH:          vKey = VK_OEM_MINUS;    break;
        case KKB_DELETE:        vKey = VK_DELETE;    break;
        case KKB_DIVIDE:        vKey = VK_DIVIDE;    break;
        case KKB_DOWN:          vKey = VK_DOWN;    break;
        case KKB_END:           vKey = VK_END;    break;
        case KKB_EQUAL:         vKey = VK_OEM_PLUS;    break;
        case KKB_ESCAPE:        vKey = VK_ESCAPE;    break;
        case KKB_HOME:          vKey = VK_HOME;    break;
        case KKB_INSERT:        vKey = VK_INSERT;    break;
        case KKB_LALT:          vKey = VK_LMENU;    break;
        case KKB_LBRACKET:      vKey = VK_OEM_4;    break;
        case KKB_LCONTROL:      vKey = VK_LCONTROL;    break;
        case KKB_LEFT:          vKey = VK_LEFT;    break;
        case KKB_LSHIFT:        vKey = VK_LSHIFT;    break;
        case KKB_LSYSTEM:       vKey = VK_LWIN;    break;
        case KKB_MENU:          vKey = VK_MENU;    break;
        case KKB_MULTIPLY:      vKey = VK_MULTIPLY;    break;
        case KKB_PAGEDOWN:      vKey = VK_NEXT;    break;
        case KKB_PAGEUP:        vKey = VK_PRIOR;    break;
        case KKB_PAUSE:         vKey = VK_PAUSE;    break;
        case KKB_PERIOD:        vKey = VK_OEM_PERIOD;    break;
        case KKB_QUOTE:         vKey = VK_OEM_7;    break;
        case KKB_RALT:          vKey = VK_RMENU;    break;
        case KKB_RBRACKET:      vKey = VK_OEM_6;    break;
        case KKB_RCONTROL:      vKey = VK_RCONTROL;    break;
        case KKB_RETURN:        vKey = VK_RETURN;    break;
        case KKB_RIGHT:         vKey = VK_RIGHT;    break;
        case KKB_RSHIFT:        vKey = VK_RSHIFT;    break;
        case KKB_RSYSTEM:       vKey = VK_RWIN;    break;
        case KKB_SEMICOLON:     vKey = VK_OEM_1;    break;
        case KKB_SLASH:         vKey = VK_OEM_2;    break;
        case KKB_SPACE:         vKey = VK_SPACE;    break;
        case KKB_SUBTRACT:      vKey = VK_SUBTRACT;    break;
        case KKB_TAB:           vKey = VK_TAB;    break;
        case KKB_TILDE:         vKey = VK_OEM_3;    break;
        case KKB_UP:            vKey = VK_UP;    break;
        default:
            vKey = 0;
            break;
        }
        return (GetAsyncKeyState(vKey) & 0x8000) != 0;
    }

    void Input::activeJoysticks(bool Exclusive, HWND Window){
#ifdef KDINPUT_ALLOW

        if (_kdinput == 0){
            // create DirectInput 8 object
            DDI_CALL(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                        IID_IDirectInput8, (void**)&_kdinput, NULL));

            // enumrate currently installed device (only joystick(s))
            DDI_CALL(_kdinput->EnumDevices(DI8DEVCLASS_ALL, _kenumDevCallb,
                                           NULL, DIEDFL_ATTACHEDONLY));

            // we find attached joystick(s)
            if (_kenumDevice.joystick){

                for(unsigned int i = 0; i < _kenumDevice.joystickCount; ++i){
                    if (_kdjoysticks[i] != 0){

                        // joystick(s) objects created. (in enum function)

                        // set joystick(s) exclusive option
                        DWORD mExc;
                        if (Exclusive){
                            mExc = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
                        }else{
                            mExc = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
                        }
                        DDI_CALL(_kdjoysticks[i]->SetCooperativeLevel(Window, mExc));

                        // set joystick(s) data format
                        DDI_CALL(_kdjoysticks[i]->SetDataFormat(&c_dfDIJoystick));


                        // enum joystick(s) capabilities
                        // currently we dont need get capabilities (maybe later!)
                        // but we must set min/max range for axis
                        // this job will done in "EnumObjectsCallback()"
                        DDI_CALL(_kdjoysticks[i]->EnumObjects(_kenumObjCallb, (void *)_kdjoysticks, DIDFT_ALL));
                    }
                }
            }
            }
#endif
    }

    const KJoystickInput *Input::getJoystickInput(Kite::U8 JoyID){
#ifdef KDINPUT_ALLOW
        if (_kenumDevice.joystick && JoyID <= _kenumDevice.joystickCount){
            if (_kdjoysticks[JoyID] != 0){

                // clear joystick-state structure
                ZeroMemory(&_kdjoysticState, sizeof(_kdjoysticState));
                _kjoystickInput.isChanged = false;

                HRESULT hRes;
                hRes = _kdjoysticks[JoyID]->Poll();
                if (hRes == DI_OK || hRes == DI_NOEFFECT){
                    if (_kdjoysticks[JoyID]->GetDeviceState(sizeof(DIJOYSTATE),(LPVOID)&_kdjoysticState) == DI_OK){
                        _kjoystickInput.xAxisAnalog = _kdjoysticState.lX;
                        _kjoystickInput.yAxisAnalog = _kdjoysticState.lY;
                        _kjoystickInput.zAxisAnalog = _kdjoysticState.lZ;
                        _kjoystickInput.rotXAxis = _kdjoysticState.lRx;
                        _kjoystickInput.rotYAxis = _kdjoysticState.lRy;
                        _kjoystickInput.rotZAxis = _kdjoysticState.lRz;
                        _kjoystickInput.buttons = _kdjoysticState.rgbButtons;
                        _kjoystickInput.pov = _kdjoysticState.rgdwPOV;
                        _kjoystickInput.slider = _kdjoysticState.rglSlider;
                        _kjoystickInput.isChanged = true;
                    }
                    _kjoystickInput.isChanged = false;

                // reacquire device (if device lost)
                }else if (hRes == DIERR_INPUTLOST || hRes == DIERR_NOTACQUIRED){
                    _kdjoysticks[JoyID]->Acquire();
                }
            }
        }
#endif
        return &_kjoystickInput;
    }

    void Input::releaseJoysticks(){
#ifdef KDINPUT_ALLOW
        for (int i = 0; i < _kenumDevice.joystickCount; ++i){
            if (_kdjoysticks[i] != 0){
                _kdjoysticks[i]->Unacquire();
                _kdjoysticks[i]->Release();
                _kdjoysticks[i] = 0;
            }
        }
#endif
    }

}
}
