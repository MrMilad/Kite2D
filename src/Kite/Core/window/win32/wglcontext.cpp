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
#include "src/Kite/core/window/win32/window.h"
#include "src/Kite/core/window/win32/wglefp.h"

namespace Kite{
namespace Internal{
    bool WGLContext::_kisInited = false;
    unsigned int WGLContext::_kinsCounter = 0;
    WGLContext::WGLContext():
        _ksharedContext(0),
        _kcontext(0),
        _khwnd(0),
        _khdc(0),
        _kisActiveCon(false),
        _kisVSync(false)
    {
        _inite();
    }

    WGLContext::~WGLContext(){}


    void WGLContext::bind(KWindowHandle WindowHandle,const KWindowState &WindowState, const WGLContext *ShareContext){
        if (_kcontext != 0){
            KDEBUG_PRINT("context created and binded before.")
            return;
        }
        _kwinState = WindowState;
        _khwnd = (HWND)WindowHandle;
        HGLRC shContext = 0;
        if (ShareContext != 0){
            shContext = (HGLRC)ShareContext->getContext();
            _ksharedContext = ShareContext;
        }

        int nPixelFormat;
        int nPixCount = 0;
        int nColorDepth = (int)_kwinState.colorDepth;

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
        KDEBUG_ASSERT(_khdc = GetDC(_khwnd));
        wglChoosePixelFormatARB(_khdc, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*)&nPixCount);

        if(nPixelFormat != -1){
            // got a format, now set it as the current one
            KDEBUG_ASSERT(SetPixelFormat(_khdc, nPixelFormat, &pfd ));

            // create the OpenGL contex.
            // if requested version > 2
            if (_kwinState.oglMajor > 2){
                while (!_kcontext && (_kwinState.oglMajor >= 3)){
                    // set version of context format
                    GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, _kwinState.oglMajor,
                                        WGL_CONTEXT_MINOR_VERSION_ARB, _kwinState.oglMinor,
                                        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                        0 };

                    _kcontext = wglCreateContextAttribsARB(_khdc, shContext, attribs);
                    // If we couldn't create the context, lower the version number and try again
                    // stop at 3.0
                    if (_kcontext == 0){
                        if (_kwinState.oglMinor == 0){
                            _kwinState.oglMinor = 9;
                            --_kwinState.oglMajor;
                        }else{
                            --_kwinState.oglMinor;
                        }
                    }
                }
            }else{
                // requested version is <= 2 or we couldn't create the higher version
                // anyway we try to create 2.x or 1.x version.
                _kcontext = wglCreateContext(_khdc);
            }
            KDEBUG_ASSERT_T(_kcontext);
            KDEBUG_ASSERT(wglMakeCurrent(_khdc, _kcontext));
            if (_ksharedContext != 0)
                    KDEBUG_ASSERT(wglShareLists(shContext, _kcontext));
        }else{
            KDEBUG_PRINT("can't find any pixel format.");
            KDEBUG_BREAK;
        }

        if (_kwinState.vsync){
            wglSwapIntervalEXT(1);
            _kisVSync = true;
        }
        ++_kinsCounter;
    }

    void WGLContext::unbind(){
        if (_kcontext != 0)
            --_kinsCounter;
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(_kcontext);
        ReleaseDC(_khwnd, _khdc);
        _khdc = 0;
        _kcontext = 0;
    }

    void WGLContext::setActiveRender(){
        KDEBUG_ASSERT_T(_kcontext);
        wglMakeCurrent(_khdc, _kcontext);
    }

    void WGLContext::setVSync(bool Enable){
        KDEBUG_ASSERT_T(_kcontext);
        if (Enable){
            wglSwapIntervalEXT(1);
            _kisVSync = true;
        }else{
            wglSwapIntervalEXT(0);
            _kisVSync = false;
        }
    }

    void WGLContext::_inite(){
        if (_kisInited)
            return;

        HDC mHDC;
        HGLRC mContext;
        KWindowState dummyState(L"", 2, 0, 0, 0);
        Window dummyWin;

        dummyWin.open(dummyState);
        KDEBUG_ASSERT_T(dummyWin.getWindowHandle());

        // setup the pixel format descriptor
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            32, // 32-bit color depth
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

        // get DeviceContext of dummy window
        KDEBUG_ASSERT(mHDC = GetDC((HWND)dummyWin.getWindowHandle()));

        // Set a dummy pixel format so that we can get access to wgl functions
        KDEBUG_ASSERT(SetPixelFormat(mHDC, 1,&pfd));

        // Create OGL context and make it current
        KDEBUG_ASSERT(mContext = wglCreateContext(mHDC));
        KDEBUG_ASSERT(wglMakeCurrent(mHDC, mContext));

        // Get the WGL function pointer (OGL extensions)
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

        // Test all function pointers
        KDEBUG_ASSERT_T(wglChoosePixelFormatARB);
        KDEBUG_ASSERT_T(wglCreateContextAttribsARB);
        KDEBUG_ASSERT_T(wglSwapIntervalEXT);

        // cleanup unnecessary things
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(mContext);
        ReleaseDC((HWND)dummyWin.getWindowHandle(), mHDC);
        dummyWin.close(); // destroy dummy window
        mContext = 0;
        mHDC = 0;
        _kisInited = true;
    }
}
}
