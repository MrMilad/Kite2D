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
		KMETA_KGLWINDOW_BODY();
    public:
        /// window will be created after open() call
        KGLWindow(KWindowState &WindowState);
        ~KGLWindow();

        bool update();
        void display(); // swap buffer

        void open();
        void close();

        bool isOpen() const;

        /// manipulate window settings
		KM_FUN()
        void setTitle(const std::string &Title);

		KM_FUN()
        void setSize(U32 Width, U32 Height);

		KM_FUN()
        void setPosition(U32 XPosition, U32 YPosition);

		KM_FUN()
        void setShowCursor(bool Enable);

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
