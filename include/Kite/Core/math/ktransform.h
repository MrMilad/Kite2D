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
#ifndef KTRANSFORM_H
#define KTRANSFORM_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include "Kite/Core/math/kmatrix3.h"
#include "Kite/Core/math/kvector2.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTransform{
    public:
        static void translate(KVector2F32 &Point, const KVector2F32 &Translate);
        static void rotate(KVector2F32 &Point, F32 Angle);
        static void scale(KVector2F32 &Point, KVector2F32 Factors);
    }; // class
}

#endif // KTRANSFORM_H
