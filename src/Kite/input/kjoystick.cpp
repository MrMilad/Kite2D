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

#include "Kite/system/ksystemdef.h"
#include "Kite/input/kjoystick.h"

#if defined (KITE_PLATFORM_WINDOWS)

    #include "src/Kite/input/win32/input.h"

#elif defined (KITE_PLATFORM_LINUX)


#elif defined (KITE_PLATFORM_MACOS)


#endif

namespace Kite{

    KJoystick::KJoystick(KWindowHandle WindowHandle, bool Exclusive){
    #if defined (KITE_PLATFORM_WINDOWS)
        _kimpl = Kite::Internal::Input::CreateInstance();
        _kimpl->setup((HWND)WindowHandle);
        _kimpl->activeJoysticks(Exclusive);
    #elif defined (KITE_PLATFORM_LINUX)


    #elif defined (KITE_PLATFORM_MACOS)


    #endif
    }

    KJoystick::~KJoystick(){
        _kimpl->releaseJoysticks();
    }

    const KJoystickInput *KJoystick::getInput(U8 JoystickID){
        return _kimpl->getJoystickInput(JoystickID);
    }

    bool KJoystick::isInstalled() const{
        return _kimpl->getEnumDevices()->joystick;
    }

    U8 KJoystick::getCount() const{
        return _kimpl->getEnumDevices()->joystickCount;
    }
}
