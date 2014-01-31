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
#ifndef SWINDOW_H
#define SWINDOW_H

#include "Kite/window/win32/window.h"
#include "Kite/window/win32/wglcontext.h"

namespace Kite{
namespace Internal{
    class SWindow{
    public:
        static wglcontext _kcontext; // single context for all windows
        Window _kwindow;
    };
}
}

#endif // SWINDOW_H
