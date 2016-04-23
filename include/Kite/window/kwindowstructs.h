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
#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/window/kwindowtypes.h"
#include "Kite/serialization/kserialization.h"
#include "kwindowstructs.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(POD)
    struct KWindowState{
		KMETA_KWINDOWSTATE_BODY();

        KM_VAR() std::string title;
		KM_VAR() U8 oglMajor;
		KM_VAR() U8 oglMinor;
		KM_VAR() U8 swapInterval;
		KM_VAR() I32 width;
		KM_VAR() I32 height;
		KM_VAR() I32 xpos;
		KM_VAR() I32 ypos;
		KM_VAR() bool showCursor;
		KM_VAR() bool fullscreen;
		KM_VAR() bool resizable;

		KWindowState(const std::string &Title = "Kite2D Window!",
					 I32 Width = 200, I32 Height = 200,
					 I32 XPosition = 0, I32 YPosition = 0,
					 bool ShowCursor = true, bool Fullscreen = false,
					 bool Resizable = false,
					 U8 SwapInterval = 0,								// Vsync is disabled by default
					 U8 OGLMajorVer = 3, U8 OGLMinorVer = 3):			// minimum version is 3.3):
            title(Title),
            oglMajor(OGLMajorVer), oglMinor(OGLMinorVer),
            swapInterval(SwapInterval),
            width(Width), height(Height),
            xpos(XPosition), ypos(YPosition),
            showCursor(ShowCursor), fullscreen(Fullscreen),
            resizable(Resizable)
        {}
    };

	KM_CLASS(POD)
    struct KEnumDisplay{
		KMETA_KENUMDISPLAY_BODY();

		KM_VAR() I32 width;
		KM_VAR() I32 height;
		KM_VAR() I32 refreshRate;
		KM_VAR() I32 colorDepth;

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

    };

}

#endif // KWINDOWSTRUCTS_H
