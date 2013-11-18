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
#ifndef KWINDOWSTRUCTS_H
#define KWINDOWSTRUCTS_H

#include <vector>
#include <string>
#include "Kite/system/ksystemdef.h"
#include "Kite/window/kwindowtypes.h"

namespace Kite{
    struct KWindowState{
        std::wstring title;
        U8 oglMajor, oglMinor;
        U32 width, height;
        I32 xpos, ypos;
        bool showCursor, fullscreen, topmost, resizable, vsync;
        KColorDepthTypes colorDepth;

        KWindowState(const std::wstring &Title = L"",
                     U8 OGLMajorVer = 2, U8 OGLMinorVer = 0, // if this version not available version will reduse and try again ...
                     U32 Width = 800, U32 Height = 600,
                     I32 XPosition = 0, I32 YPosition = 0,
                     bool ShowCursor = true, bool Fullscreen = false,
                     bool Topmost = false, bool Resizable = false,
                     bool VSync = false,
                     KColorDepthTypes ColorDepth = KCOLOR_DEPTH_32):
            title(Title),
            oglMajor(OGLMajorVer), oglMinor(OGLMinorVer),
            width(Width), height(Height),
            xpos(XPosition), ypos(YPosition),
            showCursor(ShowCursor), fullscreen(Fullscreen),
            topmost(Topmost), resizable(Resizable),
            vsync(VSync),
            colorDepth(ColorDepth)
        {}
    };

    struct KEnumDisplay{
        U32 width, height;
        KColorDepthTypes colorDepth;
        KAspectRatioTypes aspectRatio;

        KEnumDisplay(const U32 Width = 0, const U32 Height = 0,
                      const KColorDepthTypes ColorDepth = KCOLOR_DEPTH_OUT,
                      const KAspectRatioTypes AspectRatio = KASPRAT_UNKNOWN):
            width(Width), height(Height),
            colorDepth(ColorDepth),
            aspectRatio(AspectRatio)
        {}

        /* Operators */
        bool operator ==(const KEnumDisplay &right) const {
            if (this->width == right.width)
                if (this->height == right.height)
                    if (this->colorDepth == right.colorDepth)
                        return true;
            return false;
        }
        bool operator !=(const KEnumDisplay &right) const {
            return !(*this == right);
        }
    };

    /* multi-platform common handles */
    #if defined(KITE_PLATFORM_WINDOWS)

        // Window handle is HWND (void *) on Windows
        typedef void *KWindowHandle;

        // OpenGL Context is HGLRC
        typedef void *KContextHandle;

    #elif defined(KITE_PLATFORM_LINUX) || defined(KITE_PLATFORM_FREEBSD)

        // Window handle is Window (unsigned long) on Unix - X11
        typedef unsigned long KWindowHandle;

    #elif defined(KITE_PLATFORM_MACOS)

        // Window handle is NSWindow (void*) on Mac OS X - Cocoa
        typedef void *KWindowHandle;

    #endif

    typedef std::vector<KEnumDisplay> KEnumDisplayList;

}

#endif // KWINDOWSTRUCTS_H
