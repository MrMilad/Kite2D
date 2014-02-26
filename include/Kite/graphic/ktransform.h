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

#include "Kite/system/ksystemdef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphicdef.h"
#include "Kite/system/kvector2.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTransform{
    public:
        /// constructors
        KTransform();  // init with identity
        KTransform(F32 *MatrixArray);
        KTransform(F32 A00, F32 A01, F32 A02,
                             F32 A10, F32 A11, F32 A12,
                             F32 A20, F32 A21, F32 A22);

        KTransform getGeneralInverse() const;
        //KTransform &getEuclideanInverse();

        inline const F32 *getMatrix() const {return _kmatrix;}

        inline KVector2F32 transformPoint(const KVector2F32 &Point) const{
            return KVector2F32(_kmatrix[0] * Point.x + _kmatrix[4] * Point.y  + _kmatrix[12],
                            _kmatrix[1] * Point.x  + _kmatrix[5] * Point.y  + _kmatrix[13]);
        }

        KRectF32 transformRect(const KRectF32& Rectangle) const;
        KTransform &combine(const KTransform &Transform);
        KTransform &translate(const KVector2F32 &Translate);
        KTransform &rotate(F32 Angle);
        KTransform &rotate(F32 Angle, const KVector2F32 &Center);
        KTransform &scale(const KVector2F32 &Scale);
        KTransform &scale(const KVector2F32 &Scale, const KVector2F32& Center);

        /// unit matrix
        static const KTransform UnitMatrix;
    private:
        #ifdef KITE_USE_SSE
            K_ALIGNED(F32 _kmatrix[16], 16)
        #else
            F32 _kmatrix[16]; // 4 x 4 matrix
        #endif
    }; // class

    KITE_FUNC_EXPORT KTransform operator *(const KTransform &left,const KTransform &right);
    KITE_FUNC_EXPORT KTransform &operator *=(KTransform &left, const KTransform &right);
}

#endif // KTRANSFORM_H
