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
#ifndef KWINDOWSTRUCTS_H
#define KWINDOWSTRUCTS_H

#include <vector>
#include <string>
#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/window/kwindowtypes.h"
#include "Kite/core/utility/KBytesArray.h"

namespace Kite{
    struct KWindowState{
        std::string title;
        U8 oglMajor, oglMinor, swapInterval;
        U32 width, height;
        I32 xpos, ypos;
        bool showCursor, fullscreen, border, resizable;

        KWindowState(const std::string &Title = "",
                     U8 OGLMajorVer = 3, U8 OGLMinorVer = 3,    // minimum version is 3.3
                     U8 SwapInterval = 0,                       // Vsync is disabled by default
                     U32 Width = 200, U32 Height = 200,
                     I32 XPosition = 0, I32 YPosition = 0,
                     bool ShowCursor = true, bool Fullscreen = false,
                     bool Border = true, bool Resizable = false):
            title(Title),
            oglMajor(OGLMajorVer), oglMinor(OGLMinorVer),
            swapInterval(SwapInterval),
            width(Width), height(Height),
            xpos(XPosition), ypos(YPosition),
            showCursor(ShowCursor), fullscreen(Fullscreen),
            border(Border), resizable(Resizable)
        {}

		friend KBytesArray &operator>>(KBytesArray &In, KWindowState &Value) {
			In >> Value.title >> Value.oglMajor >> Value.oglMinor;
			In >> Value.swapInterval >> Value.width >> Value.height;
			In >> Value.xpos >> Value.ypos >> Value.showCursor;
			In >> Value.fullscreen >> Value.border >> Value.resizable;
			return In;
		}

		friend KBytesArray &operator<<(KBytesArray &Out, const KWindowState &Value) {
			Out << Value.title << Value.oglMajor << Value.oglMinor;
			Out << Value.swapInterval << Value.width << Value.height;
			Out << Value.xpos << Value.ypos << Value.showCursor;
			Out << Value.fullscreen << Value.border << Value.resizable;
			return Out;
		}
    };

    struct KEnumDisplay{
        I32 width, height, refreshRate, colorDepth;

        KEnumDisplay(I32 Width = 0, I32 Height = 0,
                     I32 RefreshRate = 0, I32 ColorDepth = 0):
            width(Width), height(Height),
            refreshRate(RefreshRate),
            colorDepth(ColorDepth)
        {}

        /* Operators */
        bool operator ==(const KEnumDisplay &right) const {
            if (this->width == right.width)
                if (this->height == right.height)
                    if (this->refreshRate == right.refreshRate)
                        if (this->colorDepth == right.colorDepth)
                            return true;
            return false;
        }
        bool operator !=(const KEnumDisplay &right) const {
            return !(*this == right);
        }

		friend KBytesArray &operator>>(KBytesArray &In, KEnumDisplay &Value) {
			In >> Value.width >> Value.height >> Value.refreshRate >> Value.colorDepth;
			return In;
		}

		friend KBytesArray &operator<<(KBytesArray &Out, const KEnumDisplay &Value) {
			Out << Value.width << Value.height << Value.refreshRate << Value.colorDepth;
			return Out;
		}
    };

}

#endif // KWINDOWSTRUCTS_H
