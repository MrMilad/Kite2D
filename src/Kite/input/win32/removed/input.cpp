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
#include "Kite/input/win32/input.h"

namespace Kite{
namespace Internal{
    Input *Input::_kinstance = 0;

    BOOL CALLBACK Input::_kenumDevCallb(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext){
        // call member function callback
        // because we need access to private member variable
        Input *classPtr = (Input *)pContext;
        return classPtr->_kenumDevCallbMember(pdidInstance);
    }

    BOOL Input::_kenumDevCallbMember(const DIDEVICEINSTANCE *pdidInstance){
        switch( 0xff & pdidInstance->dwDevType ){
        case DI8DEVTYPE_KEYBOARD:
            _kenumDevice.keyboard = true;
            break;
        case DI8DEVTYPE_MOUSE:
            _kenumDevice.mouse = true;
            break;
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

    Input *Input::CreateInstance(){
        if (_kinstance == 0)
            _kinstance = new Input();

        return _kinstance;
    }

    void Input::DestroyInstance(){
        delete _kinstance;
        _kinstance = 0;
    }

    Input::Input():
        _kdinput(0),
        //_kdmouse(0),
        //_kdkeyboard(0),
        _kdjoysticks() // array
    {}

    Input::~Input(){

        // release keyboard
        //this->releaseKeyboard();

        // release mouse
        //this->releaseMouse();

        // release joystick(s)
        this->releaseJoysticks();

        // release DirectInput
        if (_kdinput != 0){
            _kdinput->Release();
            _kdinput = 0;
        }
    }

    void Input::setup(HWND WindowHandle){
        if (_kdinput == 0){
            // create DirectInput 8 object
            DDI_CALL(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                        IID_IDirectInput8, (void**)&_kdinput, NULL));

            _kwhwnd = WindowHandle;

            // enumrate currently installed device
            DDI_CALL(_kdinput->EnumDevices(DI8DEVCLASS_ALL, _kenumDevCallb,
                                           (void *)this, DIEDFL_ATTACHEDONLY));
        }
    }

    void Input::activeMouse(bool Exclusive){
        // check mouse device
        if (_kdmouse == 0 && _kenumDevice.mouse){

            // create mouse device
            DDI_CALL(_kdinput->CreateDevice(GUID_SysMouse, &_kdmouse, NULL));

            // set mouse exclusive option
            DWORD mExc;
            if (Exclusive){
                mExc = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
            }else{
                mExc = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
            }
            DDI_CALL(_kdmouse->SetCooperativeLevel(_kwhwnd, mExc));

            // set mouse data format (using standard format)
            DDI_CALL(_kdmouse->SetDataFormat(&c_dfDIMouse));

            // enum mouse capabilities
            // currently we dont need get capabilities (maybe later!)
            // DDI_CALL(_kdmouse->EnumObjects( _kenumObjCallb, NULL, DIDFT_ALL ));
        }
    }

    const KMouseInput *Input::getMouseInput(){
        // check mouse device
        if (_kdmouse != 0){

            // clear mouse state structure
            ZeroMemory( &_kdmouseState, sizeof(_kdmouseState));
            _kmouseInput.isChanged = false;

            // get current mouse-state
            // and fill our mouse-state structure
            HRESULT hRes;
            hRes = _kdmouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&_kdmouseState);
            if (hRes == DI_OK){
                POINT _kmousePos;
                GetCursorPos(&_kmousePos);
                _kmouseInput.xPos = _kmousePos.x;
                _kmouseInput.yPos =  _kmousePos.y;
                ScreenToClient(_kwhwnd, &_kmousePos);
                _kmouseInput.xWinPos = _kmousePos.x;
                _kmouseInput.yWinPos = _kmousePos.y;
                _kmouseInput.xDelta = _kdmouseState.lX;
                _kmouseInput.yDelta = _kdmouseState.lY;
                _kmouseInput.zDelta = _kdmouseState.lZ;
                _kmouseInput.leftButton = (_kdmouseState.rgbButtons[0] & 0x80);
                _kmouseInput.rightButton = (_kdmouseState.rgbButtons[1] & 0x80);
                _kmouseInput.middleButton = (_kdmouseState.rgbButtons[2] & 0x80);
                _kmouseInput.isChanged = true;

            // reacquire device (if device lost)
            }else if (hRes == DIERR_INPUTLOST || hRes == DIERR_NOTACQUIRED){
                _kdmouse->Acquire();
            }
        }
        return &_kmouseInput;
    }

    void Input::releaseMouse(){
        if (_kdmouse != 0){
            _kdmouse->Unacquire();
            _kdmouse->Release();
            _kdmouse = 0;
        }
    }

    void Input::activeKeyboard(bool Exclusive){
        // check keyboard device
        if (_kdkeyboard == 0 && _kenumDevice.keyboard){

            // create keyboard device
            DDI_CALL(_kdinput->CreateDevice(GUID_SysKeyboard, &_kdkeyboard, NULL));

            // set keyboard exclusive option
            DWORD mExc;
            if (Exclusive){
                mExc = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
            }else{
                mExc = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
            }
            DDI_CALL(_kdkeyboard->SetCooperativeLevel(_kwhwnd, mExc));

            // set keyboard data format
            DDI_CALL(_kdkeyboard->SetDataFormat(&c_dfDIKeyboard));

            // enum keyboard capabilities
            // currently we dont need get capabilities (maybe later!)
            // DDI_CALL(_kdkeyboard->EnumObjects( _kenumObjCallb, NULL, DIDFT_ALL ));
        }
    }

    const KKeyboardInput *Input::getKeyboardInput(){
        // check keyboard device
        if (_kdkeyboard != 0){

            // clear keyboard-state structure
            ZeroMemory( &_kdkeyboardState, sizeof(_kdkeyboardState));
            _kkeyboardInput.isChanged = false;

            // get current keyboard-state
            // and fill our keyboard-state structure
            HRESULT hRes;
            hRes = _kdkeyboard->GetDeviceState(sizeof(_kdkeyboardState),(LPVOID)&_kdkeyboardState);

            if (hRes == DI_OK){
                _kkeyboardInput.keys = _kdkeyboardState;
                _kkeyboardInput.lctrl = (_kdkeyboardState[DIK_LCONTROL] & 0x80);
                _kkeyboardInput.rctrl = (_kdkeyboardState[DIK_RCONTROL] & 0x80);
                _kkeyboardInput.lalt = (_kdkeyboardState[DIK_LALT] & 0x80);
                _kkeyboardInput.ralt = (_kdkeyboardState[DIK_RALT] & 0x80);
                _kkeyboardInput.lshift = (_kdkeyboardState[DIK_LSHIFT] & 0x80);
                _kkeyboardInput.rshift = (_kdkeyboardState[DIK_RSHIFT] & 0x80);
                _kkeyboardInput.isChanged = true;

            // reacquire device (if device lost)
            }else if (hRes == DIERR_INPUTLOST || hRes == DIERR_NOTACQUIRED){
                _kdkeyboard->Acquire();
            }
        }
        return &_kkeyboardInput;
    }

    void Input::releaseKeyboard(){
        if (_kdkeyboard != 0){
            _kdkeyboard->Unacquire();
            _kdkeyboard->Release();
            _kdkeyboard = 0;
        }
    }

    void Input::activeJoysticks(bool Exclusive){
        // check keyboard device
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
                    DDI_CALL(_kdjoysticks[i]->SetCooperativeLevel(_kwhwnd, mExc));

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

    const KJoystickInput *Input::getJoystickInput(Kite::U8 JoyID){
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
        return &_kjoystickInput;
    }

    void Input::releaseJoysticks(){
        for (int i = 0; i < _kenumDevice.joystickCount; ++i){
            if (_kdjoysticks[i] != 0){
                _kdjoysticks[i]->Unacquire();
                _kdjoysticks[i]->Release();
                _kdjoysticks[i] = 0;
            }
        }
    }

}
}
