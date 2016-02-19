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
#ifndef KKEYBOARD_H
#define KKEYBOARD_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/input/kinputtypes.h"
#include "Kite/Core/input/kinputstructs.h"

struct SDL_Window;
union SDL_Event;
namespace Kite{
    class KITE_FUNC_EXPORT KKeyboard{
    public:
		/// initialize keyboard module
		static void initeKeyboard();

        /// returns the last state reported for the specified key
		/// uasge: c++ and lua
        static bool isButtonPressed(KKeyCodeTypes Button);

		/// returns the last state of the keyboard
		/// usage: c++ only
		static const U8 *getKeyboardState();

		/// get the current key modifier state for the keyboard
		/// usage: c++ and lua
		static bool isModifierPressed(KKeyModifierTypes Modifier);

		/// get the current key modifier state for the keyboard
		/// usage: c++
		/// ex: if (KKeyboard::getKeyboardState()[KCT_U]) { u pressed TODO }
		static KKeyModifierTypes getModifierState();

    };
}

#endif // KKEYBOARD_H
