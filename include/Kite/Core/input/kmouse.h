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
#ifndef KMOUSE_H
#define KMOUSE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/input/kinputtypes.h"

union SDL_Event;
namespace Kite{
    class KITE_FUNC_EXPORT KMouse{
    public:
		static void initeMouse();
        /// get the current state of a mouse button
		/// usage: c++ and lua
        static bool isButtonPressed(KMouseButtonTypes Button);

        /// retrieves the last reported cursor position,
        /// relative to the client area of the window
        static KVector2I32 getPosition();

		/// x: the amount scrolled horizontally, positive to the right and negative to the left
		/// y: the amount scrolled vertically, positive away from the user and negative toward the user
		static KVector2I32 getWheelValue();

        /// sets the position of the cursor,
        /// relative to the client area of the window
        static void setWindowPosition(const KVector2I32 &Position, KWindowHandle Handle);

		static void setGlobalPosition(const KVector2I32 &Position);

    private:
		/// SDL dot support mouse wheel in mouse input sections
		/// so we add an event watcher for catching wheel events
		static int _eventWatcher(void *Data, SDL_Event *Event);
        static KWindowHandle _kwinHandle;
		static KVector2I32 _kwheelVal;
    };
}

#endif // KMOUSE_H
