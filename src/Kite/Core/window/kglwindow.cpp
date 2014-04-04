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
#include "Kite/core/window/kglwindow.h"
#include "src/Kite/Core/window/fwcall.h"

namespace Kite{
    KGLWindow::KGLWindow():
        _kwindow(0)
    {}

    KGLWindow::KGLWindow(KWindowState &WindowState):
        _kwindow(0),
        _kwinstate(WindowState)
    {}

    KGLWindow::~KGLWindow(){
        if (_kwindow){
            glfwDestroyWindow((GLFWwindow *)_kwindow);
            _kwindow = 0;
        }
    }

    bool KGLWindow::update(){
        if (!glfwWindowShouldClose((GLFWwindow *)_kwindow)){
            glfwPollEvents();
            return true;
        }
        return false;
    }

    void KGLWindow::display(){
        glfwSwapBuffers((GLFWwindow *)_kwindow);
    }

    void KGLWindow::open(){
        // initialize glfe
        Internal::initeGLFW();

        // destroy currently window
        if (_kwindow){
            glfwDestroyWindow((GLFWwindow *)_kwindow);
            _kwindow = 0;
        }

        glfwWindowHint(GLFW_RESIZABLE, _kwinstate.resizable);

        glfwWindowHint(GLFW_DECORATED, _kwinstate.border);

        glfwWindowHint(GLFW_DEPTH_BITS , 0);
        glfwWindowHint(GLFW_STENCIL_BITS, 0);

        // opengl 3.3 or greater
        if (_kwinstate.oglMajor <= 3){
            _kwinstate.oglMajor = 3;
            if (_kwinstate.oglMinor < 3)
                _kwinstate.oglMinor = 3;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _kwinstate.oglMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _kwinstate.oglMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);

        // full screen
        GLFWmonitor *monitor = NULL;
        if (_kwinstate.fullscreen){
            monitor = glfwGetPrimaryMonitor();
            _kwinstate.xpos = 0;
            _kwinstate.ypos = 0;
        }

        // create window
        _kwindow = (void *)glfwCreateWindow(_kwinstate.width, _kwinstate.height,
                                        _kwinstate.title.c_str(), monitor, NULL);

        // make context current
        glfwMakeContextCurrent((GLFWwindow *)_kwindow);

        // set position
        if (!_kwinstate.fullscreen){
            glfwSetWindowPos((GLFWwindow *)_kwindow, _kwinstate.xpos, _kwinstate.ypos);
        }

        // show cursor
        if (!_kwinstate.showCursor)
            glfwSetInputMode((GLFWwindow *)_kwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // swap interval (vsync)
        glfwSwapInterval(_kwinstate.swapInterlal);
    }

    void KGLWindow::open(KWindowState &WindowState){
        _kwinstate = WindowState;
        open();
    }

    void KGLWindow::close(){
        glfwSetWindowShouldClose((GLFWwindow *)_kwindow, GL_TRUE);
    }

    void KGLWindow::setTitle(const std::string &Title){
        glfwSetWindowTitle((GLFWwindow *)_kwindow, Title.c_str());
        _kwinstate.title = Title;
    }

    void KGLWindow::setSize(U32 Width, U32 Height){
        glfwSetWindowSize((GLFWwindow *)_kwindow, Width, Height);
        _kwinstate.width = Width;
        _kwinstate.height = Height;
    }

    void KGLWindow::setPosition(U32 XPosition, U32 YPosition){
        glfwSetWindowPos((GLFWwindow *)_kwindow, XPosition, YPosition);
        _kwinstate.xpos = XPosition;
        _kwinstate.ypos = YPosition;
    }

    void KGLWindow::setShowCursor(bool Enable){
        if (Enable){
            glfwSetInputMode((GLFWwindow *)_kwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode((GLFWwindow *)_kwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        _kwinstate.showCursor = Enable;
    }

    KVector2I32 KGLWindow::getFrameBufferSize(){
        KVector2I32 bufSize;
        glfwGetFramebufferSize((GLFWwindow *)_kwindow, &bufSize.x, &bufSize.y);
        return bufSize;
    }

//    bool KGLWindow::setFullscreen(bool Enable){

//    }

    bool KGLWindow::isOpen() const{
        if (_kwindow)
            return true;
        return false;
    }

    void KGLWindow::registerCallback(void *Callback, KWindowCallbackTypes CallbackType){
        switch (CallbackType){
        case KWC_CLOSE:
            glfwSetWindowCloseCallback((GLFWwindow *)_kwindow, (GLFWwindowclosefun) Callback);
            break;
        case KWC_FOCUSCHANGE:
            glfwSetWindowFocusCallback((GLFWwindow *)_kwindow, (GLFWwindowfocusfun) Callback);
            break;
        case KWC_FBCHANGE:
            glfwSetFramebufferSizeCallback((GLFWwindow *)_kwindow, (GLFWframebuffersizefun) Callback);
            break;
        case KWC_POSCHANGE:
            glfwSetWindowPosCallback((GLFWwindow *)_kwindow, (GLFWwindowposfun) Callback);
            break;
        case KWC_SIZECHANGE:
            glfwSetWindowSizeCallback((GLFWwindow *)_kwindow, (GLFWwindowsizefun) Callback);
            break;
        default:
            KDEBUG_PRINT("invalid window callback");
            break;
        }
    }

    void KGLWindow::unregisterCallback(KWindowCallbackTypes CallbackType){
        switch (CallbackType){
        case KWC_ALL:
            glfwSetWindowCloseCallback((GLFWwindow *)_kwindow, 0);
            glfwSetWindowFocusCallback((GLFWwindow *)_kwindow, 0);
            glfwSetWindowPosCallback((GLFWwindow *)_kwindow, 0);
            glfwSetWindowSizeCallback((GLFWwindow *)_kwindow, 0);
            break;
        case KWC_CLOSE:
            glfwSetWindowCloseCallback((GLFWwindow *)_kwindow, 0);
            break;
        case KWC_FOCUSCHANGE:
            glfwSetWindowFocusCallback((GLFWwindow *)_kwindow, 0);
            break;
        case KWC_FBCHANGE:
            glfwSetFramebufferSizeCallback((GLFWwindow *)_kwindow, 0);
            break;
        case KWC_POSCHANGE:
            glfwSetWindowPosCallback((GLFWwindow *)_kwindow, 0);
            break;
        case KWC_SIZECHANGE:
            glfwSetWindowSizeCallback((GLFWwindow *)_kwindow, 0);
            break;
        default:
            KDEBUG_PRINT("invalid window callback");
            break;
        }
    }
}
