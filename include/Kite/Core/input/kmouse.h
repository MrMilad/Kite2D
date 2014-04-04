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
#ifndef KMOUSE_H
#define KMOUSE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kvector2.h"
#include "Kite/Core/input/kinputtypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KMouse{
    public:
        /// set window handle
        inline static void setWindowHandle(KWindowHandle Window) {_kwinHandle = Window;}

        /// returns the last reported state of a mouse button
        static KButtonStateTypes getButtonState(KMouseButtonTypes Button);

        /// retrieves the last reported cursor position,
        /// relative to the client area of the window
        static KVector2F64 getPosition();

        /// sets the position of the cursor,
        /// relative to the client area of the window
        static void setPosition(const KVector2F64 &Position);

        /// input callbacks
        static void registerCallback(void *Callback, KMouseCallbackTypes CallbackType);
        static void unregisterCallback(KMouseCallbackTypes CallbackType);

    private:
        static KWindowHandle _kwinHandle;
    };
}

#endif // KMOUSE_H
