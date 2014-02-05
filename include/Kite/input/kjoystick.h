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

#include "Kite/system/ksystemdef.h"
#include "Kite/input/kinputstructs.h"
#include "Kite/window/kwindowstructs.h"

namespace Kite{
namespace Internal{
    class Input;
}
    class KITE_FUNC_EXPORT KJoystick{
    public:
        KJoystick(KWindowHandle WindowHandle, bool Exclusive);
        ~KJoystick();

        const KJoystickInput *getInput(U8 JoystickID);
        bool isInstalled() const;
        U8 getCount() const;
    private:
        Kite::Internal::Input *_kimpl;
    };
}

#endif // KJOYSTICK_H
