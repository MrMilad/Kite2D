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

#include "Kite/core/kcoredef.h"
#include "Kite/input/kinputtypes.h"
#include "Kite/input/kinputstructs.h"
#include "Kite/meta/kmetadef.h"
#include "kkeyboard.khgen.h"

struct SDL_Window;
union SDL_Event;
KMETA
namespace Kite{
	KM_CLASS(SCRIPTABLE)
    class KITE_FUNC_EXPORT KKeyboard{
		KM_INFO(KI_NAME = "keyboard")
		KKEYBOARD_BODY();
    public:
		/// initialize keyboard module
		static void initeKeyboard();

		KM_FUN()
		static bool isAnyKeyDown();

        /// returns the last state reported for the specified key
		/// uasge: c++ and lua
		KM_FUN()
        static bool isButtonPressed(KeyCode Button);

		/// returns the last state of the keyboard
		/// usage: c++ only
		/// ex: if (KKeyboard::getKeyboardState()[KCT_U]) { u pressed }
		static const U8 *getKeyboardState();

		/// get the current key modifier state for the keyboard
		/// usage: c++ and lua
		KM_FUN()
		static bool isModifierPressed(KeyModifier Modifier);

		/// get the current key modifier state for the keyboard
		/// usage: c++ and lua
		KM_FUN()
		static KeyModifier getModifierState();

	private:
		/// SDL dos not support mouse wheel in mouse input sections
		/// so we add an event watcher for catching wheel events
		static int _eventWatcher(void *Data, SDL_Event *Event);

		static U16 _kcount;
    };
}

#endif // KKEYBOARD_H
