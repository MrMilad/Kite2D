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
#ifndef KWINDOWUTIL_H
#define KWINDOWUTIL_H

#include "Kite/window/kwindowstructs.h"

namespace Kite{

    /// returns the available states
    KITE_FUNC_EXPORT extern void getFullscreenStates(std::vector<KEnumDisplay> &DisplayList);

    /// returns the current desktop state
    KITE_FUNC_EXPORT extern KEnumDisplay getDesktopState();
}

#endif // KWINDOWUTIL_H
