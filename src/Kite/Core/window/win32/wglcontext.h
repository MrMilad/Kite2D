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
#ifndef WGLCONTEXT_H
#define WGLCONTEXT_H

#include "windows.h"
#include "WinGDI.h"
#include "Kite/core/system/knoncopyable.h"
#include "Kite/core/window/kwindowstructs.h"

namespace Kite{
namespace Internal{
    class WGLContext : KNonCopyable{
    public:
        WGLContext();
        virtual ~WGLContext();

        void bind(KWindowHandle WindowHandle,
                  const KWindowState &WindowState,
                  const WGLContext *ShareContext);
        void unbind();

        inline void display(){
            KDEBUG_ASSERT_T(_kcontext);
            SwapBuffers(_khdc);
        }

        void setActiveRender();
        void setVSync(bool Enable);

        inline const WGLContext *getSharedContext() const {return _ksharedContext;}
        inline KContextHandle getContext() const {return _kcontext;}
        inline bool isVsyncEnable() const {return _kisVSync;}
        inline bool isActiveRender() const {return _kisActiveCon;}
        static inline bool isAnyContext() {if (_kinsCounter > 0) return true; return false;}

    private:
        static void _inite();
        void _create();
        const WGLContext *_ksharedContext;
        KWindowState _kwinState;
        HGLRC _kcontext;
        HWND _khwnd;
        HDC _khdc;
        bool _kisActiveCon;
        bool _kisVSync;
        static bool _kisInited;
        static unsigned int _kinsCounter;
    };
}
}

#endif // KWGLCONTEXT_H
