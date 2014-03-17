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
#include "src/Kite/core/window/win32/wglcontext.h"
#include "src/Kite/core/window/win32/wglcall.h"

namespace Kite{
namespace Internal{
    unsigned int WGLContext::_kinsCounter = 0;
    WGLContext::WGLContext():
        _ksharedContext(0),
        _kcontext(0),
        _khdc(0),
        _kisActiveCon(false),
        _kisVSync(false)
    {
        inite();
    }

    WGLContext::~WGLContext(){}

    void WGLContext::bind(const Window &OpenWindow, const WGLContext *ShareContext){
        unbind();

        HGLRC shContext = 0;

        if (!OpenWindow.isOpen()){
            KDEBUG_PRINT("invalid or closed window");
            return;
        }

        // get window dc (draw surface)
        _khwnd = (HWND)OpenWindow.getWindowHandle();
        KDEBUG_ASSERT(_khdc = GetDC(_khwnd));

        // checking for shared context
        if (ShareContext != 0){
            shContext = (HGLRC)ShareContext->getContext();
            _ksharedContext = ShareContext;
        }

        int nPixelFormat;
        int nPixCount = 0;
        int nColorDepth = (int)OpenWindow.getWindowState().colorDepth;

        // Specify the important attributes we care about
        int pixAttribs[] = {
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // Must support OGL rendering
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // pf that can run a window
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // must be HW accelerated
        WGL_COLOR_BITS_ARB, nColorDepth, // 8 bits of each R, G ,B and A
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // Double buffered context
        WGL_SAMPLE_BUFFERS_ARB, GL_FALSE, // Antialiasing
        WGL_SAMPLES_ARB, 0, // 0-8x Antialiasing Level
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // pf should be RGBA type
        0 }; // NULL termination

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            (BYTE)nColorDepth,     // 32-bit color depth
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

        // ask OpenGL to find the most relevant format matching our attribs
        // only get one format back.
        wglChoosePixelFormatARB(_khdc, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*)&nPixCount);
        KDEBUG_ASSERT_T(nPixelFormat != -1);

        // got a format, now set it as the current one
        KDEBUG_ASSERT(SetPixelFormat(_khdc, nPixelFormat, &pfd ));

        // if requested version > 2
        if (OpenWindow.getWindowState().oglMajor > 2){
            // set version of context format
            GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, OpenWindow.getWindowState().oglMajor,
                                WGL_CONTEXT_MINOR_VERSION_ARB, OpenWindow.getWindowState().oglMinor,
                                WGL_CONTEXT_FLAGS_ARB, 0,
                                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                0};

            // cretae context
            KDEBUG_ASSERT(_kcontext = wglCreateContextAttribsARB(_khdc, shContext, attribs));

        // if requested version <= 2
        }else{
            // create context
            KDEBUG_ASSERT(_kcontext = wglCreateContext(_khdc));
        }

        // set as current context
        KDEBUG_ASSERT(wglMakeCurrent(_khdc, _kcontext));

        // share display-lists
        if (_ksharedContext != 0)
                KDEBUG_ASSERT(wglShareLists(shContext, _kcontext));

        // set VSync
        if (OpenWindow.getWindowState().vsync){
            wglSwapIntervalEXT(1);
            _kisVSync = true;
        }
        ++_kinsCounter;
    }

    void WGLContext::unbind(){
        if (_kcontext){
            wglMakeCurrent(_khdc, NULL);
            wglDeleteContext(_kcontext);
            ReleaseDC(_khwnd, _khdc);
            _kcontext = 0;
            _khdc = 0;
            --_kinsCounter;
        }
    }

    void WGLContext::setActiveRender(){
        if(_kcontext){
            KDEBUG_ASSERT(wglMakeCurrent(_khdc, _kcontext));
        }
    }

    void WGLContext::setVSync(bool Enable){
        if(_kcontext){
            if (Enable){
                KDEBUG_ASSERT(wglSwapIntervalEXT(1));
                _kisVSync = true;
            }else{
                KDEBUG_ASSERT(wglSwapIntervalEXT(0));
                _kisVSync = false;
            }
        }
    }

    void WGLContext::inite(){
        // initialize wgl
        Window dummyWin;
        KWindowState dummyState;
        HDC dummyDC;

        dummyState.width = 10;
        dummyState.height = 10;

        dummyWin.open(dummyState);
        KDEBUG_ASSERT(dummyDC = GetDC((HWND)dummyWin.getWindowHandle()));
        KDEBUG_ASSERT(initeWGL(dummyDC));
        ReleaseDC((HWND)dummyWin.getWindowHandle(), dummyDC);
        dummyWin.close();
    }
}
}
