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

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{

    enum KKeyboardKeyTypes {
        KKK_UNKNOWN = -1,
        KKK_SPACE = 32,
        KKK_APOSTROPHE = 39, // '
        KKK_COMMA = 44, // ,
        KKK_MINUS = 45, // -
        KKK_PERIOD = 46, // .
        KKK_SLASH = 47, // /
        KKK_0 = 48,
        KKK_1 = 49,
        KKK_2 = 50,
        KKK_3 = 51,
        KKK_4 = 52,
        KKK_5 = 53,
        KKK_6 = 54,
        KKK_7 = 55,
        KKK_8 = 56,
        KKK_9 = 57,
        KKK_SEMICOLON = 59,
        KKK_EQUAL = 61, // =
        KKK_A = 65,
        KKK_B = 66,
        KKK_C = 67,
        KKK_D = 68,
        KKK_E = 69,
        KKK_F = 70,
        KKK_G = 71,
        KKK_H = 72,
        KKK_I = 73,
        KKK_J = 74,
        KKK_K = 75,
        KKK_L = 76,
        KKK_M = 77,
        KKK_N = 78,
        KKK_O = 79,
        KKK_P = 80,
        KKK_Q = 81,
        KKK_R = 82,
        KKK_S = 83,
        KKK_T = 84,
        KKK_U = 85,
        KKK_V = 86,
        KKK_W = 87,
        KKK_X = 88,
        KKK_Y = 89,
        KKK_Z = 90,
        KKK_LEFT_BRACKET = 91, // [
        KKK_BACKSLASH = 92,
        KKK_RIGHT_BRACKET = 93, // ]
        KKK_GRAVE_ACCENT = 96, // `
        KKK_WORLD_1 = 161, // non-US #1
        KKK_WORLD_2 = 162, // non-US #2
        KKK_ESCAPE = 256,
        KKK_ENTER = 257,
        KKK_TAB = 258,
        KKK_BACKSPACE = 259,
        KKK_INSERT = 260,
        KKK_DELETE = 261,
        KKK_RIGHT = 262,
        KKK_LEFT = 263,
        KKK_DOWN = 264,
        KKK_UP = 265,
        KKK_PAGE_UP = 266,
        KKK_PAGE_DOWN = 267,
        KKK_HOME = 268,
        KKK_END = 269,
        KKK_CAPS_LOCK = 280,
        KKK_SCROLL_LOCK = 281,
        KKK_NUM_LOCK = 282,
        KKK_PRINT_SCREEN = 283,
        KKK_PAUSE = 284,
        KKK_F1 = 290,
        KKK_F2 = 291,
        KKK_F3 = 292,
        KKK_F4 = 293,
        KKK_F5 = 294,
        KKK_F6 = 295,
        KKK_F7 = 296,
        KKK_F8 = 297,
        KKK_F9 = 298,
        KKK_F10 = 299,
        KKK_F11 = 300,
        KKK_F12 = 301,
        KKK_F13 = 302,
        KKK_F14 = 303,
        KKK_F15 = 304,
        KKK_F16 = 305,
        KKK_F17 = 306,
        KKK_F18 = 307,
        KKK_F19 = 308,
        KKK_F20 = 309,
        KKK_F21 = 310,
        KKK_F22 = 311,
        KKK_F23 = 312,
        KKK_F24 = 313,
        KKK_F25 = 314,
        KKK_KP_0 = 320,
        KKK_KP_1 = 321,
        KKK_KP_2 = 322,
        KKK_KP_3 = 323,
        KKK_KP_4 = 324,
        KKK_KP_5 = 325,
        KKK_KP_6 = 326,
        KKK_KP_7 = 327,
        KKK_KP_8 = 328,
        KKK_KP_9 = 329,
        KKK_KP_DECIMAL = 330,
        KKK_KP_DIVIDE = 331,
        KKK_KP_MULTIPLY = 332,
        KKK_KP_SUBTRACT = 333,
        KKK_KP_ADD = 334,
        KKK_KP_ENTER = 335,
        KKK_KP_EQUAL = 336,
        KKK_LEFT_SHIFT = 340,
        KKK_LEFT_CONTROL = 341,
        KKK_LEFT_ALT = 342,
        KKK_LEFT_SUPER = 343,
        KKK_RIGHT_SHIFT = 344,
        KKK_RIGHT_CONTROL = 345,
        KKK_RIGHT_ALT = 346,
        KKK_RIGHT_SUPER = 347,
        KKK_MENU = 348,
        KKK_LAST = KKK_MENU
    };

    enum KMouseButtonTypes{
        KMB_LEFT = 0,
        KMB_RIGHT,
        KMB_MIDDLE,
        KMB_X1,
        KMB_X2
    };

    enum KButtonStateTypes{
        KBS_RELEASE = 0,
        KBS_PRESS,
        KBS_REPEAT
    };

    enum KModifierKeysTypes{
        KMK_SHIFT = 0x0001,
        KMK_CONTROL = 0x0002,
        KMK_ALT = 0x0004,
        KMK_SUPER = 0x0008
    };

    enum KMouseCallbackTypes{
        KMC_ALL = 0, // use only for unregistering callback
        KMC_MBUTTON,
        KMC_MPOSITION,
        KMC_MENTER,
        KMC_MSCROLL
    };

    enum KKeyboardCallbackTypes{
        KKC_ALL = 0,
        KKC_KEY,
        KKC_UNICODE
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
