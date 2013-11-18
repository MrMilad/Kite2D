/*    
    Kite2D Game Framework.
    Copyright (C) 2010-2013  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef WIN32WINDOW_INL
#define WIN32WINDOW_INL

#include "src/Kite/window/win32/win32window.h"
static inline bool win32window::update(){
    KDEBUG_ASSERT_T(m_lastActiveWin)
    static MSG msg;
    while(true){
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        break;
    }
    return m_continueLoop;
}

#endif // WIN32WINDOW_INL
