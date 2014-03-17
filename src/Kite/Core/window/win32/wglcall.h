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
#ifndef WGLEFP_H
#define WGLEFP_H

#include <windows.h>
#include <wingdi.h>
#include "gl/gl.h"
#include "extlibs/headers/gl/wglext.h"
#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
namespace Internal{
    // WGL function pointer (OGL extensions)
    static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 0;
    static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;
    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;

    extern bool initeWGL(HDC Hdc){
        HGLRC hCurrentContext = wglGetCurrentContext();

        // there is no current context
        if (!hCurrentContext){

            // setup the pixel format descriptor
            PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
                1,                     // version number
                PFD_DRAW_TO_WINDOW |   // support window
                PFD_SUPPORT_OPENGL |   // support OpenGL
                PFD_DOUBLEBUFFER,      // double buffered
                PFD_TYPE_RGBA,         // RGBA type
                32,                     // 32-bit color depth
                0, 0, 0, 0, 0, 0,      // color bits ignored
                0,                     // no alpha buffer
                0,                     // shift bit ignored
                0,                     // no accumulation buffer
                0, 0, 0, 0,            // accum bits ignored
                0,                     // no z-buffer
                0,                     // no stencil buffer
                0,                     // no auxiliary buffer
                PFD_MAIN_PLANE,        // main layer
                0,                     // reserved
                0, 0, 0                // layer masks ignored
            };

            // Set a dummy pixel format so that we can get access to wgl functions
            KDEBUG_ASSERT(SetPixelFormat(Hdc, 1,&pfd));

            // create new context
            if (!(hCurrentContext = wglCreateContext(Hdc))){
                KDEBUG_PRINT("wglCreateContext failed");
                return false;
            }

            // set as current
            if (!wglMakeCurrent(Hdc, hCurrentContext)){
                wglDeleteContext(hCurrentContext);
                KDEBUG_PRINT("wglMakeCurrent failed");
                return false;
            }

            // load function entry points
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

            // test all function pointers
            KDEBUG_ASSERT_T(wglChoosePixelFormatARB);
            KDEBUG_ASSERT_T(wglCreateContextAttribsARB);
            KDEBUG_ASSERT_T(wglSwapIntervalEXT);

            wglMakeCurrent(Hdc, 0);
            wglDeleteContext(hCurrentContext);

        // there is a current context
        }else{
            if (!wglMakeCurrent(Hdc, hCurrentContext)){
                KDEBUG_PRINT("wglMakeCurrent failed");
                return false;
            }

            // load function entry points
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

            // test all function pointers
            KDEBUG_ASSERT_T(wglChoosePixelFormatARB);
            KDEBUG_ASSERT_T(wglCreateContextAttribsARB);
            KDEBUG_ASSERT_T(wglSwapIntervalEXT);
        }
        return true;
    }
}
}

#endif // WGLEFP_H
