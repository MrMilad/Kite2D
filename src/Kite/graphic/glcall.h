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
#ifndef GLCALL_H
#define GLCALL_H

//#define GLEW_STATIC
#include "GL/glew.h"
#include "Kite/system/ksystemdef.h"

namespace Kite{
namespace Internal{
    extern bool initeGLEW();
    extern bool checkLastGLErr();
}
}

// OpenGL debug macro
#if defined(KITE_DEV_DEBUG)
    #define DGL_CALL(call) call; KDEBUG_ASSERT_T(Kite::Internal::checkLastGLErr())
#else
    #define DGL_CALL(call) call;
#endif

#endif // GLCALL_H
