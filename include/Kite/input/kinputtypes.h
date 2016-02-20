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

namespace Kite{

    enum KKeyCodeTypes {
		KCT_UNKNOWN = 0,
		KCT_A = 4,
		KCT_B = 5,
		KCT_C = 6,
		KCT_D = 7,
		KCT_E = 8,
		KCT_F = 9,
		KCT_G = 10,
		KCT_H = 11,
		KCT_I = 12,
		KCT_J = 13,
		KCT_K = 14,
		KCT_L = 15,
		KCT_M = 16,
		KCT_N = 17,
		KCT_O = 18,
		KCT_P = 19,
		KCT_Q = 20,
		KCT_R = 21,
		KCT_S = 22,
		KCT_T = 23,
		KCT_U = 24,
		KCT_V = 25,
		KCT_W = 26,
		KCT_X = 27,
		KCT_Y = 28,
		KCT_Z = 29,

		KCT_1 = 30,
		KCT_2 = 31,
		KCT_3 = 32,
		KCT_4 = 33,
		KCT_5 = 34,
		KCT_6 = 35,
		KCT_7 = 36,
		KCT_8 = 37,
		KCT_9 = 38,
		KCT_0 = 39,

		KCT_RETURN = 40,
		KCT_ESCAPE = 41,
		KCT_BACKSPACE = 42,
		KCT_TAB = 43,
		KCT_SPACE = 44,

		KCT_MINUS = 45,
		KCT_EQUALS = 46,
		KCT_LEFTBRACKET = 47,
		KCT_RIGHTBRACKET = 48,
		KCT_BACKSLASH = 49, /**< Located at the lower left of the return
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
		KCT_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
									 *   instead of 49 for the same key, but all
									 *   OSes I've seen treat the two codes
									 *   identically. So, as an implementor, unless
									 *   your keyboard generates both of those
									 *   codes and your OS treats them differently,
									 *   you should generate KCT_BACKSLASH
									 *   instead of this code. As a user, you
									 *   should not rely on this code because SDL
									 *   will never generate it with most (all?)
									 *   keyboards.
									 */
		KCT_SEMICOLON = 51,
		KCT_APOSTROPHE = 52,
		KCT_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
		KCT_COMMA = 54,
		KCT_PERIOD = 55,
		KCT_SLASH = 56,

		KCT_CAPSLOCK = 57,

		KCT_F1 = 58,
		KCT_F2 = 59,
		KCT_F3 = 60,
		KCT_F4 = 61,
		KCT_F5 = 62,
		KCT_F6 = 63,
		KCT_F7 = 64,
		KCT_F8 = 65,
		KCT_F9 = 66,
		KCT_F10 = 67,
		KCT_F11 = 68,
		KCT_F12 = 69,

		KCT_PRINTSCREEN = 70,
		KCT_SCROLLLOCK = 71,
		KCT_PAUSE = 72,
		KCT_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
								  does send code 73, not 117) */
		KCT_HOME = 74,
		KCT_PAGEUP = 75,
		KCT_DELETE = 76,
		KCT_END = 77,
		KCT_PAGEDOWN = 78,
		KCT_RIGHT = 79,
		KCT_LEFT = 80,
		KCT_DOWN = 81,
		KCT_UP = 82,

		KCT_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
										*/
		KCT_KP_DIVIDE = 84,
		KCT_KP_MULTIPLY = 85,
		KCT_KP_MINUS = 86,
		KCT_KP_PLUS = 87,
		KCT_KP_ENTER = 88,
		KCT_KP_1 = 89,
		KCT_KP_2 = 90,
		KCT_KP_3 = 91,
		KCT_KP_4 = 92,
		KCT_KP_5 = 93,
		KCT_KP_6 = 94,
		KCT_KP_7 = 95,
		KCT_KP_8 = 96,
		KCT_KP_9 = 97,
		KCT_KP_0 = 98,
		KCT_KP_PERIOD = 99,

		KCT_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
										   *   keyboards have over ANSI ones,
										   *   located between left shift and Y.
										   *   Produces GRAVE ACCENT and TILDE in a
										   *   US or UK Mac layout, REVERSE SOLIDUS
										   *   (backslash) and VERTICAL LINE in a
										   *   US or UK Windows layout, and
										   *   LESS-THAN SIGN and GREATER-THAN SIGN
										   *   in a Swiss German, German, or French
										   *   layout. */
		KCT_APPLICATION = 101, /**< windows contextual menu, compose */
		KCT_POWER = 102, /**< The USB document says this is a status flag,
								  *   not a physical key - but some Mac keyboards
								  *   do have a power key. */
		KCT_KP_EQUALS = 103,
		KCT_F13 = 104,
		KCT_F14 = 105,
		KCT_F15 = 106,
		KCT_F16 = 107,
		KCT_F17 = 108,
		KCT_F18 = 109,
		KCT_F19 = 110,
		KCT_F20 = 111,
		KCT_F21 = 112,
		KCT_F22 = 113,
		KCT_F23 = 114,
		KCT_F24 = 115,
		KCT_EXECUTE = 116,
		KCT_HELP = 117,
		KCT_MENU = 118,
		KCT_SELECT = 119,
		KCT_STOP = 120,
		KCT_AGAIN = 121,   /**< redo */
		KCT_UNDO = 122,
		KCT_CUT = 123,
		KCT_COPY = 124,
		KCT_PASTE = 125,
		KCT_FIND = 126,
		KCT_MUTE = 127,
		KCT_VOLUMEUP = 128,
		KCT_VOLUMEDOWN = 129,
		/* not sure whether there's a reason to enable these */
		/*     KCT_LOCKINGCAPSLOCK = 130,  */
		/*     KCT_LOCKINGNUMLOCK = 131, */
		/*     KCT_LOCKINGSCROLLLOCK = 132, */
		KCT_KP_COMMA = 133,
		KCT_KP_EQUALSAS400 = 134,

		KCT_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
										   footnotes in USB doc */
		KCT_INTERNATIONAL2 = 136,
		KCT_INTERNATIONAL3 = 137, /**< Yen */
		KCT_INTERNATIONAL4 = 138,
		KCT_INTERNATIONAL5 = 139,
		KCT_INTERNATIONAL6 = 140,
		KCT_INTERNATIONAL7 = 141,
		KCT_INTERNATIONAL8 = 142,
		KCT_INTERNATIONAL9 = 143,
		KCT_LANG1 = 144, /**< Hangul/English toggle */
		KCT_LANG2 = 145, /**< Hanja conversion */
		KCT_LANG3 = 146, /**< Katakana */
		KCT_LANG4 = 147, /**< Hiragana */
		KCT_LANG5 = 148, /**< Zenkaku/Hankaku */
		KCT_LANG6 = 149, /**< reserved */
		KCT_LANG7 = 150, /**< reserved */
		KCT_LANG8 = 151, /**< reserved */
		KCT_LANG9 = 152, /**< reserved */

		KCT_ALTERASE = 153, /**< Erase-Eaze */
		KCT_SYSREQ = 154,
		KCT_CANCEL = 155,
		KCT_CLEAR = 156,
		KCT_PRIOR = 157,
		KCT_RETURN2 = 158,
		KCT_SEPARATOR = 159,
		KCT_OUT = 160,
		KCT_OPER = 161,
		KCT_CLEARAGAIN = 162,
		KCT_CRSEL = 163,
		KCT_EXSEL = 164,

		KCT_KP_00 = 176,
		KCT_KP_000 = 177,
		KCT_THOUSANDSSEPARATOR = 178,
		KCT_DECIMALSEPARATOR = 179,
		KCT_CURRENCYUNIT = 180,
		KCT_CURRENCYSUBUNIT = 181,
		KCT_KP_LEFTPAREN = 182,
		KCT_KP_RIGHTPAREN = 183,
		KCT_KP_LEFTBRACE = 184,
		KCT_KP_RIGHTBRACE = 185,
		KCT_KP_TAB = 186,
		KCT_KP_BACKSPACE = 187,
		KCT_KP_A = 188,
		KCT_KP_B = 189,
		KCT_KP_C = 190,
		KCT_KP_D = 191,
		KCT_KP_E = 192,
		KCT_KP_F = 193,
		KCT_KP_XOR = 194,
		KCT_KP_POWER = 195,
		KCT_KP_PERCENT = 196,
		KCT_KP_LESS = 197,
		KCT_KP_GREATER = 198,
		KCT_KP_AMPERSAND = 199,
		KCT_KP_DBLAMPERSAND = 200,
		KCT_KP_VERTICALBAR = 201,
		KCT_KP_DBLVERTICALBAR = 202,
		KCT_KP_COLON = 203,
		KCT_KP_HASH = 204,
		KCT_KP_SPACE = 205,
		KCT_KP_AT = 206,
		KCT_KP_EXCLAM = 207,
		KCT_KP_MEMSTORE = 208,
		KCT_KP_MEMRECALL = 209,
		KCT_KP_MEMCLEAR = 210,
		KCT_KP_MEMADD = 211,
		KCT_KP_MEMSUBTRACT = 212,
		KCT_KP_MEMMULTIPLY = 213,
		KCT_KP_MEMDIVIDE = 214,
		KCT_KP_PLUSMINUS = 215,
		KCT_KP_CLEAR = 216,
		KCT_KP_CLEARENTRY = 217,
		KCT_KP_BINARY = 218,
		KCT_KP_OCTAL = 219,
		KCT_KP_DECIMAL = 220,
		KCT_KP_HEXADECIMAL = 221,

		KCT_LCTRL = 224,
		KCT_LSHIFT = 225,
		KCT_LALT = 226, /**< alt, option */
		KCT_LGUI = 227, /**< windows, command (apple), meta */
		KCT_RCTRL = 228,
		KCT_RSHIFT = 229,
		KCT_RALT = 230, /**< alt gr, option */
		KCT_RGUI = 231, /**< windows, command (apple), meta */

		KCT_MODE = 257,    /**< I'm not sure if this is really not covered
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

		KCT_AUDIONEXT = 258,
		KCT_AUDIOPREV = 259,
		KCT_AUDIOSTOP = 260,
		KCT_AUDIOPLAY = 261,
		KCT_AUDIOMUTE = 262,
		KCT_MEDIASELECT = 263,
		KCT_WWW = 264,
		KCT_MAIL = 265,
		KCT_CALCULATOR = 266,
		KCT_COMPUTER = 267,
		KCT_AC_SEARCH = 268,
		KCT_AC_HOME = 269,
		KCT_AC_BACK = 270,
		KCT_AC_FORWARD = 271,
		KCT_AC_STOP = 272,
		KCT_AC_REFRESH = 273,
		KCT_AC_BOOKMARKS = 274,

		/* @} *//* Usage page 0x0C */

				/**
				*  \name Walther keys
				*
				*  These are values that Christian Walther added (for mac keyboard?).
				*/
				/* @{ */

		KCT_BRIGHTNESSDOWN = 275,
		KCT_BRIGHTNESSUP = 276,
		KCT_DISPLAYSWITCH = 277, /**< display mirroring/dual display
										  switch, video mode switch */
		KCT_KBDILLUMTOGGLE = 278,
		KCT_KBDILLUMDOWN = 279,
		KCT_KBDILLUMUP = 280,
		KCT_EJECT = 281,
		KCT_SLEEP = 282,

		KCT_APP1 = 283,
		KCT_APP2 = 284,

		/* @} *//* Walther keys */

				/* Add any other keys here. */

		KCT_MAX_SIZE = 512 /**< not a key, just marks the number of scancodes
								for array bounds */
    };

	enum class KKeyModifierTypes : U32 {
		KMT_NONE = 0x0000,
		KMT_LSHIFT = 0x0001,
		KMT_RSHIFT = 0x0002,
		KMT_LCTRL = 0x0040,
		KMT_RCTRL = 0x0080,
		KMT_LALT = 0x0100,
		KMT_RALT = 0x0200,
		KMT_LGUI = 0x0400,
		KMT_RGUI = 0x0800,
		KMT_NUM = 0x1000,
		KMT_CAPS = 0x2000,
		KMT_MODE = 0x4000,
		KMT_RESERVED = 0x8000
	};

    /*enum class KButtonStateTypes : U8{
        KBS_RELEASE = 0,
        KBS_PRESS,
        KBS_REPEAT
    };*/

	enum KMouseButtonTypes {
		KMB_LEFT = 0,
		KMB_RIGHT,
		KMB_MIDDLE,
		KMB_X1,
		KMB_X2
	};

    enum KJoystcikIDTypes{
        KJI_JOYSTICK1 = 0,
        KJI_JOYSTICK2 = 1,
        KJI_JOYSTICK3 = 2,
        KJI_JOYSTICK4 = 3
    };

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
