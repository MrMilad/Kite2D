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
#ifndef KGRAPHICUTIL_H
#define KGRAPHICUTIL_H

#include "Kite/system/ksystemdef.h"

namespace Kite{

/// return one side of maximum texture size (e.g 1024 = 1024x1024)
extern "C" { KITE_FUNC_EXPORT I32 getMaximumTextureSize(); }

/// maximum number of supported texture units
extern "C" { KITE_FUNC_EXPORT I32 getMaximumTextureUnit(); }

/// check availability of opengl shader programs
extern "C" { KITE_FUNC_EXPORT bool isShaderAvailable(); }

}

#endif // KGRAPHICUTIL_H
