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
#ifndef KINPUTTYPES_H
#define KINPUTTYPES_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "kinputtypes.khgen.h"

KMETA
namespace Kite{

	KM_ENUM()
    enum class KeyCode : U32{
		UNKNOWN = 0,
		KEY_A = 4,
		KEY_B = 5,
		KEY_C = 6,
		KEY_D = 7,
		KEY_E = 8,
		KEY_F = 9,
		KEY_G = 10,
		KEY_H = 11,
		KEY_I = 12,
		KEY_J = 13,
		KEY_K = 14,
		KEY_L = 15,
		KEY_M = 16,
		KEY_N = 17,
		KEY_O = 18,
		KEY_P = 19,
		KEY_Q = 20,
		KEY_R = 21,
		KEY_S = 22,
		KEY_T = 23,
		KEY_U = 24,
		KEY_V = 25,
		KEY_W = 26,
		KEY_X = 27,
		KEY_Y = 28,
		KEY_Z = 29,

		NUM_1 = 30,
		NUM_2 = 31,
		NUM_3 = 32,
		NUM_4 = 33,
		NUM_5 = 34,
		NUM_6 = 35,
		NUM_7 = 36,
		NUM_8 = 37,
		NUM_9 = 38,
		NUM_0 = 39,

		RETURN = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,

		MINUS = 45,
		EQUALS = 46,
		LEFTBRACKET = 47,
		RIGHTBRACKET = 48,
		BACKSLASH = 49, /**< Located at the lower left of the return
									 *   key on ISO keyboards and at the right end
									 *   of the QWERTY row on ANSI keyboards.
									 *   Produces REVERSE SOLIDUS (backslash) and
									 *   VERTICAL LINE in a US layout, REVERSE
									 *   SOLIDUS and VERTICAL LINE in a UK Mac
									 *   layout, NUMBER SIGN and TILDE in a UK
									 *   Windows layout, DOLLAR SIGN and POUND SIGN
									 *   in a Swiss German layout, NUMBER SIGN and
									 *   APOSTROPHE in a German layout, GRAVE
									 *   ACCENT and POUND SIGN in a French Mac
									 *   layout, and ASTERISK and MICRO SIGN in a
									 *   French Windows layout.
									 */
		NONUSHASH = 50, /**< ISO USB keyboards actually use this code
									 *   instead of 49 for the same key, but all
									 *   OSes I've seen treat the two codes
									 *   identically. So, as an implementor, unless
									 *   your keyboard generates both of those
									 *   codes and your OS treats them differently,
									 *   you should generate BACKSLASH
									 *   instead of this code. As a user, you
									 *   should not rely on this code because SDL
									 *   will never generate it with most (all?)
									 *   keyboards.
									 */
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53, /**< Located in the top left corner (on both ANSI
								 *   and ISO keyboards). Produces GRAVE ACCENT and
								 *   TILDE in a US Windows layout and in US and UK
								 *   Mac layouts on ANSI keyboards, GRAVE ACCENT
								 *   and NOT SIGN in a UK Windows layout, SECTION
								 *   SIGN and PLUS-MINUS SIGN in US and UK Mac
								 *   layouts on ISO keyboards, SECTION SIGN and
								 *   DEGREE SIGN in a Swiss German layout (Mac:
								 *   only on ISO keyboards), CIRCUMFLEX ACCENT and
								 *   DEGREE SIGN in a German layout (Mac: only on
								 *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
								 *   French Windows layout, COMMERCIAL AT and
								 *   NUMBER SIGN in a French Mac layout on ISO
								 *   keyboards, and LESS-THAN SIGN and GREATER-THAN
								 *   SIGN in a Swiss German, German, or French Mac
								 *   layout on ANSI keyboards.
								 */
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,

		CAPSLOCK = 57,

		F1 = 58,
		F2 = 59,
		F3 = 60,
		F4 = 61,
		F5 = 62,
		F6 = 63,
		F7 = 64,
		F8 = 65,
		F9 = 66,
		F10 = 67,
		F11 = 68,
		F12 = 69,

		PRINTSCREEN = 70,
		SCROLLLOCK = 71,
		PAUSE = 72,
		INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
								  does send code 73, not 117) */
		HOME = 74,
		PAGEUP = 75,
		DEL = 76,
		END = 77,
		PAGEDOWN = 78,
		RIGHT = 79,
		LEFT = 80,
		DOWN = 81,
		UP = 82,

		NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
										*/
		KP_DIVIDE = 84,
		KP_MULTIPLY = 85,
		KP_MINUS = 86,
		KP_PLUS = 87,
		KP_ENTER = 88,
		KP_1 = 89,
		KP_2 = 90,
		KP_3 = 91,
		KP_4 = 92,
		KP_5 = 93,
		KP_6 = 94,
		KP_7 = 95,
		KP_8 = 96,
		KP_9 = 97,
		KP_0 = 98,
		KP_PERIOD = 99,

		NONUSBACKSLASH = 100, /**< This is the additional key that ISO
										   *   keyboards have over ANSI ones,
										   *   located between left shift and Y.
										   *   Produces GRAVE ACCENT and TILDE in a
										   *   US or UK Mac layout, REVERSE SOLIDUS
										   *   (backslash) and VERTICAL LINE in a
										   *   US or UK Windows layout, and
										   *   LESS-THAN SIGN and GREATER-THAN SIGN
										   *   in a Swiss German, German, or French
										   *   layout. */
		APPLICATION = 101, /**< windows contextual menu, compose */
		POWER = 102, /**< The USB document says this is a status flag,
								  *   not a physical key - but some Mac keyboards
								  *   do have a power key. */
		KP_EQUALS = 103,
		F13 = 104,
		F14 = 105,
		F15 = 106,
		F16 = 107,
		F17 = 108,
		F18 = 109,
		F19 = 110,
		F20 = 111,
		F21 = 112,
		F22 = 113,
		F23 = 114,
		F24 = 115,
		EXECUTE = 116,
		HELP = 117,
		MENU = 118,
		SELECT = 119,
		STOP = 120,
		AGAIN = 121,   /**< redo */
		UNDO = 122,
		CUT = 123,
		COPY = 124,
		PASTE = 125,
		FIND = 126,
		MUTE = 127,
		VOLUMEUP = 128,
		VOLUMEDOWN = 129,
		/* not sure whether there's a reason to enable these */
		/*     LOCKINGCAPSLOCK = 130,  */
		/*     LOCKINGNUMLOCK = 131, */
		/*     LOCKINGSCROLLLOCK = 132, */
		KP_COMMA = 133,
		KP_EQUALSAS400 = 134,

		INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
										   footnotes in USB doc */
		INTERNATIONAL2 = 136,
		INTERNATIONAL3 = 137, /**< Yen */
		INTERNATIONAL4 = 138,
		INTERNATIONAL5 = 139,
		INTERNATIONAL6 = 140,
		INTERNATIONAL7 = 141,
		INTERNATIONAL8 = 142,
		INTERNATIONAL9 = 143,
		LANG1 = 144, /**< Hangul/English toggle */
		LANG2 = 145, /**< Hanja conversion */
		LANG3 = 146, /**< Katakana */
		LANG4 = 147, /**< Hiragana */
		LANG5 = 148, /**< Zenkaku/Hankaku */
		LANG6 = 149, /**< reserved */
		LANG7 = 150, /**< reserved */
		LANG8 = 151, /**< reserved */
		LANG9 = 152, /**< reserved */

		ALTERASE = 153, /**< Erase-Eaze */
		SYSREQ = 154,
		CANCEL = 155,
		CLEAR = 156,
		PRIOR = 157,
		RETURN2 = 158,
		SEPARATOR = 159,
		OUTE = 160,
		OPER = 161,
		CLEARAGAIN = 162,
		CRSEL = 163,
		EXSEL = 164,

		KP_00 = 176,
		KP_000 = 177,
		THOUSANDSSEPARATOR = 178,
		DECIMALSEPARATOR = 179,
		CURRENCYUNIT = 180,
		CURRENCYSUBUNIT = 181,
		KP_LEFTPAREN = 182,
		KP_RIGHTPAREN = 183,
		KP_LEFTBRACE = 184,
		KP_RIGHTBRACE = 185,
		KP_TAB = 186,
		KP_BACKSPACE = 187,
		KP_A = 188,
		KP_B = 189,
		KP_C = 190,
		KP_D = 191,
		KP_E = 192,
		KP_F = 193,
		KP_XOR = 194,
		KP_POWER = 195,
		KP_PERCENT = 196,
		KP_LESS = 197,
		KP_GREATER = 198,
		KP_AMPERSAND = 199,
		KP_DBLAMPERSAND = 200,
		KP_VERTICALBAR = 201,
		KP_DBLVERTICALBAR = 202,
		KP_COLON = 203,
		KP_HASH = 204,
		KP_SPACE = 205,
		KP_AT = 206,
		KP_EXCLAM = 207,
		KP_MEMSTORE = 208,
		KP_MEMRECALL = 209,
		KP_MEMCLEAR = 210,
		KP_MEMADD = 211,
		KP_MEMSUBTRACT = 212,
		KP_MEMMULTIPLY = 213,
		KP_MEMDIVIDE = 214,
		KP_PLUSMINUS = 215,
		KP_CLEAR = 216,
		KP_CLEARENTRY = 217,
		KP_BINARY = 218,
		KP_OCTAL = 219,
		KP_DECIMAL = 220,
		KP_HEXADECIMAL = 221,

		LCTRL = 224,
		LSHIFT = 225,
		LALT = 226, /**< alt, option */
		LGUI = 227, /**< windows, command (apple), meta */
		RCTRL = 228,
		RSHIFT = 229,
		RALT = 230, /**< alt gr, option */
		RGUI = 231, /**< windows, command (apple), meta */

		MODE = 257,    /**< I'm not sure if this is really not covered
									*   by any of the above, but since there's a
									*   special KMOD_MODE for it I'm adding it here
									*/

									/* @} *//* Usage page 0x07 */

											/**
											*  \name Usage page 0x0C
											*
											*  These values are mapped from usage page 0x0C (USB consumer page).
											*/
											/* @{ */

		AUDIONEXT = 258,
		AUDIOPREV = 259,
		AUDIOSTOP = 260,
		AUDIOPLAY = 261,
		AUDIOMUTE = 262,
		MEDIASELECT = 263,
		WWW = 264,
		MAIL = 265,
		CALCULATOR = 266,
		COMPUTER = 267,
		AC_SEARCH = 268,
		AC_HOME = 269,
		AC_BACK = 270,
		AC_FORWARD = 271,
		AC_STOP = 272,
		AC_REFRESH = 273,
		AC_BOOKMARKS = 274,

		/* @} *//* Usage page 0x0C */

				/**
				*  \name Walther keys
				*
				*  These are values that Christian Walther added (for mac keyboard?).
				*/
				/* @{ */

		BRIGHTNESSDOWN = 275,
		BRIGHTNESSUP = 276,
		DISPLAYSWITCH = 277, /**< display mirroring/dual display
										  switch, video mode switch */
		KBDILLUMTOGGLE = 278,
		KBDILLUMDOWN = 279,
		KBDILLUMUP = 280,
		EJECT = 281,
		SLEEP = 282,

		APP1 = 283,
		APP2 = 284,

		/* @} *//* Walther keys */

				/* Add any other keys here. */

		MAX_SIZE = 512 /**< not a key, just marks the number of scancodes
								for array bounds */
    };
	KEYCODE_BODY();

	KM_ENUM()
	enum class KeyModifier : U32 {
		NONE = 0x0000,
		LSHIFT = 0x0001,
		RSHIFT = 0x0002,
		LCTRL = 0x0040,
		RCTRL = 0x0080,
		LALT = 0x0100,
		RALT = 0x0200,
		LGUI = 0x0400,
		RGUI = 0x0800,
		NUM = 0x1000,
		CAPS = 0x2000,
		MODE = 0x4000,
		RESERVED = 0x8000
	};
	KEYMODIFIER_BODY();

    /*enum class KButtonStateTypes : U8{
        KBS_RELEASE = 0,
        KBS_PRESS,
        KBS_REPEAT
    };*/

	KM_ENUM()
	enum class MouseButton {
		LEFT = 0,
		RIGHT,
		MIDDLE,
		X1,
		X2
	};
	MOUSEBUTTON_BODY();

	KM_ENUM()
    enum class JoystcikID{
        KJI_JOYSTICK1 = 0,
        KJI_JOYSTICK2 = 1,
        KJI_JOYSTICK3 = 2,
        KJI_JOYSTICK4 = 3
    };
	JOYSTCIKID_BODY();

    /// mouse callbacks
    typedef void(*KCallMouseButton )(KWindowHandle WinHandle, I32 Button, I32 State, I32 ModifierKeys);
    typedef void(*KCallMousePosition)(KWindowHandle WinHandle, F64 XPos, F64 YPos);
    typedef void(*KCallMouseEnter)(KWindowHandle WinHandle, bool Entered);
    typedef void(*KCallMouseScroll)(KWindowHandle WinHandle, F64 XOffset, F64 YOffset);

    /// keyboard callbacks
    typedef void(*KCallKeyboardKey)(KWindowHandle WinHandle, I32 Key, I32 ScanCode, I32 State, I32 ModifierKeys);
    typedef void(*KCallKeyboardUnicode)(KWindowHandle WinHandle, U32 Code);
}
#endif // KINPUTTYPES_H
