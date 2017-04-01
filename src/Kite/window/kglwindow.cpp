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
#include "Kite/window/kglwindow.h"
#include "src/Kite/window/sdlcall.h"
#include "sdl/SDL_events.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>

namespace Kite{

    KGLWindow::KGLWindow(KWindowState &WindowState):
		_kcallb(0),
        _kwindow(0),
        _kwinstate(WindowState),
		_kcontext(0)
    {}

    KGLWindow::~KGLWindow(){
		close();
    }

    void KGLWindow::open(){
        // initialize SDL
        Internal::initeSDL();

        // destroy currently window
        if (_kwindow){
			KD_PRINT("this window is currently opened!");
			return;
        }

		// make flags
		U32 flag = SDL_WINDOW_OPENGL;
		if (_kwinstate.fullscreen) {
			_kwinstate.xpos = 0;
			_kwinstate.ypos = 0;
			flag = flag | SDL_WINDOW_FULLSCREEN;
		} else {
			if (_kwinstate.resizable) {
				flag = flag | SDL_WINDOW_RESIZABLE;
			}
		}

		// OGL version
		// opengl 3.3 or greater
		if (_kwinstate.oglMajor <= 3) {
			_kwinstate.oglMajor = 3;
			if (_kwinstate.oglMinor < 3)
				_kwinstate.oglMinor = 3;
		}

		// Set our OpenGL version.
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _kwinstate.oglMajor));
		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _kwinstate.oglMinor));

		// optimize for 2D purpose
		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0));
		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0));

		// double buffer
		DSDL_CALL(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));

		// Create our window 
		DSDL_CALL(_kwindow = SDL_CreateWindow(_kwinstate.title.c_str(), _kwinstate.xpos, _kwinstate.ypos,
											  _kwinstate.width, _kwinstate.height, flag));

		// Create an OpenGL context associated with the window.
		DSDL_CALL(_kcontext = SDL_GL_CreateContext(_kwindow));

		DSDL_CALL(SDL_GL_SetSwapInterval(_kwinstate.swapInterval));

		if (!_kwinstate.showCursor) {
			DSDL_CALL(SDL_ShowCursor(SDL_DISABLE));
		}
    }

    void KGLWindow::close(){
		if (_kwindow) {
			// Delete our OpengL context
			DSDL_CALL(SDL_GL_DeleteContext(_kcontext));

			// Destroy our window
			DSDL_CALL(SDL_DestroyWindow(_kwindow));
			_kwindow = 0;
		}
    }

    void KGLWindow::setTitle(const std::string &Title){
		if (_kwindow) {
			DSDL_CALL(SDL_SetWindowTitle(_kwindow, Title.c_str()));
			_kwinstate.title = Title;
		}
    }

    void KGLWindow::setSize(const KVector2I32 &Size){
		if (_kwindow) {
			DSDL_CALL(SDL_SetWindowSize(_kwindow, Size.x, Size.y));
			_kwinstate.width = Size.x;
			_kwinstate.height = Size.y;
		}
    }

    void KGLWindow::setPosition(const KVector2I32 &Pos){
		if (_kwindow) {
			DSDL_CALL(SDL_SetWindowPosition(_kwindow, Pos.x, Pos.y));
			_kwinstate.xpos = Pos.x;
			_kwinstate.ypos = Pos.y;
		}
    }

    void KGLWindow::setShowCursor(bool Enable){
        if (Enable){
			DSDL_CALL(SDL_ShowCursor(SDL_ENABLE));
        }else{
			DSDL_CALL(SDL_ShowCursor(SDL_DISABLE));
        }
        _kwinstate.showCursor = Enable;
    }

    bool KGLWindow::isOpen() const{
        if (_kwindow)
            return true;
        return false;
    }

	bool KGLWindow::update() {
		static SDL_Event event;
		if (_kwindow == 0) {
			return false;
		}
		DSDL_CALL(SDL_PollEvent(&event));
		if (event.type == SDL_WINDOWEVENT) {
			switch (event.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				if (_kcallb) {
					(_kcallb)(KWE_SHOWN);
				}
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				if (_kcallb) {
					(_kcallb)(KWE_HIDDEN);
				}
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				if (_kcallb) {
					(_kcallb)(KWE_EXPOSED);
				}
				break;
			case SDL_WINDOWEVENT_MOVED:
				DSDL_CALL(SDL_GetWindowPosition(_kwindow, &_kwinstate.xpos, &_kwinstate.ypos));
				if (_kcallb) {
					(_kcallb)(KWE_MOVED);
				}
				break;
			case SDL_WINDOWEVENT_RESIZED:
				DSDL_CALL(SDL_GetWindowSize(_kwindow, &_kwinstate.width, &_kwinstate.height));
				if (_kcallb) {
					(_kcallb)(KWE_RESIZED);
				}
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				DSDL_CALL(SDL_GetWindowSize(_kwindow, &_kwinstate.width, &_kwinstate.height));
				if (_kcallb) {
					(_kcallb)(KWE_SIZE_CHANGED);
				}
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				if (_kcallb) {
					(_kcallb)(KWE_MINIMIZED);
				}
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				if (_kcallb) {
					(_kcallb)(KWE_MAXIMIZED);
				}
				break;
			case SDL_WINDOWEVENT_RESTORED:
				if (_kcallb) {
					(_kcallb)(KWE_RESTORED);
				}
				break;
			case SDL_WINDOWEVENT_ENTER:
				if (_kcallb) {
					(_kcallb)(KWE_ENTER);
				}
				break;
			case SDL_WINDOWEVENT_LEAVE:
				if (_kcallb) {
					(_kcallb)(KWE_LEAVE);
				}
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				if (_kcallb) {
					(_kcallb)(KWE_FOCUS_GAINED);
				}
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				if (_kcallb) {
					(_kcallb)(KWE_FOCUS_LOST);
				}
				break;
			case SDL_WINDOWEVENT_CLOSE:
				if (_kcallb) {
					(_kcallb)(KWE_CLOSE);
				}
				return false;
			default:
				if (_kcallb) {
					(_kcallb)(KWE_UNKNOWN);
				}
				break;
			}
		}

		return true;
	}

	void KGLWindow::display() {
		if (_kwindow) {
			DSDL_CALL(SDL_GL_SwapWindow(_kwindow));
		}
	}

    void KGLWindow::registerCallback(KCallWindowEvent Callback){
		_kcallb = Callback;
    }

	KGLWINDOW_SOURCE();
}
