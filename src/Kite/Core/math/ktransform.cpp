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
#include "Kite/core/math/ktransform.h"
#include <cmath>

namespace Kite{
    void KTransform::translate(Kite::KVector2F32 &Point, const Kite::KVector2F32 &Translate){
        Point.x += Translate.x;
        Point.y += Translate.y;
    }

    void KTransform::rotate(KVector2F32 &Point, F32 Angle){
            F32 radx = Angle * KMATH_PIsub180;
            F32 cosx = std::cos(radx);
            F32 sinx = std::sin(radx);

            Point.x = Point.x * cosx - Point.y * sinx;
            Point.y = Point.x * sinx + Point.y * cosx;
    }

    void KTransform::scale(KVector2F32 &Point, Kite::KVector2F32 Factors){
        Point.x *= Factors.x;
        Point.y *= Factors.y;
    }
}
