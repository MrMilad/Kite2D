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
#ifndef KGLWINDOW_H
#define KGLWINDOW_H

#include <string>
#include "Kite/core/kcoredef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/window/kwindowtypes.h"
#include "Kite/window/kwindowstructs.h"
#include "Kite/meta/kmetadef.h"
#include "kglwindow.khgen.h"

struct SDL_Window;
KMETA
namespace Kite{
	KM_CLASS(SCRIPTABLE)
    class KITE_FUNC_EXPORT KGLWindow{
		KM_INFO(KI_NAME = "Window");
		KGLWINDOW_BODY();
    public:
        /// window will be created after open() call
        KGLWindow(KWindowState &WindowState);
        ~KGLWindow();

        bool update();
        void display(); // swap buffer

        void open();

		KM_FUN()
        void close();

        bool isOpen() const;

        /// manipulate window settings
		KM_PRO_SET(KP_NAME = "title")
        void setTitle(const std::string &Title);

		KM_PRO_GET(KP_NAME = "title", KP_TYPE = std::string, KP_CM = "window title")
		inline const std::string &getTitle() const { return _kwinstate.title; }

		KM_PRO_SET(KP_NAME = "size")
        void setSize(const KVector2I32 &Size);

		KM_PRO_GET(KP_NAME = "size", KP_TYPE = KVector2I32, KP_CM = "window size")
		inline KVector2I32 getSize() const { return KVector2I32(_kwinstate.width, _kwinstate.height); }

		KM_PRO_SET(KP_NAME = "position")
        void setPosition(const KVector2I32 &Pos);

		KM_PRO_GET(KP_NAME = "position", KP_TYPE = KVector2I32, KP_CM = "window position")
		inline KVector2I32 getPosition() const { return KVector2I32(_kwinstate.xpos, _kwinstate.ypos); }

		KM_PRO_SET(KP_NAME = "isShowCursor")
        void setShowCursor(bool Enable);

		KM_PRO_GET(KP_NAME = "isShowCursor", KP_TYPE = bool, KP_CM = "show cursor")
		inline bool isShowCursor() const { return _kwinstate.showCursor; }
		
        inline const KWindowState &getWindowState() const {return _kwinstate;}

		inline KWindowHandle getHandle() const { return _kwindow; }

        /// window callbacks
        void registerCallback(KCallWindowEvent Callback);
    private:
		KCallWindowEvent _kcallb;
        KWindowState _kwinstate;
		SDL_Window *_kwindow;
		void *_kcontext;
    };
}

#endif // KGLWINDOW_H
