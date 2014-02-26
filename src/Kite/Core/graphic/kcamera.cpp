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
#include "Kite/graphic/kcamera.h"

namespace Kite{
    KCamera::KCamera():
        _kviewport(0.0f, 0.0f, 1.0f, 1.0f),
        _kcenter(0, 0),
        _krotation(0.0f),
        _kzoom(1.0f),
        _kneedUpdateIn(true)
    {}

    KCamera::KCamera(const KRectF32 &Viewport):
        _kviewport(Viewport),
        _kcenter(0.0f, 0.0f),
        _krotation(0.0f),
        _kzoom(1.0f),
        _kneedUpdateIn(true)
    {}

/*    const KTransform &KCamera::getTransform() const{
//        // recompute the matrix if needed
//        if (_kneedUpdateTr){
//            // rotation components
//            F32 angle  = _krotation * KMATH_PIsub180;
//            F32 cosx = (F32)(std::cos(angle));
//            F32 sinx   = (F32)(std::sin(angle));
//            F32 tx     = -_kcenter.x * cosx - _kcenter.y * sinx + _kcenter.x;
//            F32 ty     =  _kcenter.x * sinx - _kcenter.y * cosx + _kcenter.y;

//            // projection components
//            F32 a =  2.f / _ksize.x;
//            F32 b = -2.f / _ksize.y;
//            F32 c = -a * _kcenter.x;
//            F32 d = -b * _kcenter.y;

//            // rebuild the projection matrix
//            _ktransform = KTransform( a * cosx, a * sinx,   a * tx + c,
//                                    -b * sinx,   b * cosx, b * ty + d,
//                                     0.0f,        0.0f,        1.0f);
//            _kneedUpdateTr = false;
//        }

//        return _ktransform;
//    }*/

    KTransform KCamera::getInverseTransform() const{
        if (_kneedUpdateIn){
            _kneedUpdateIn = false;
            return (_ktransformIn = _ktransform.getGeneralInverse());
        }
        return _ktransformIn;
    }

    void KCamera::setRotation(F32 Angle){
        _ktransform.rotate(Angle, _kcenter);
        _kneedUpdateIn = true;
    }

    void KCamera::setZoom(F32 Factor){
        _ktransform.scale(KVector2F32(Factor, Factor), _kcenter);
        _kneedUpdateIn = true;
    }

    KVector2F32 KCamera::convertPoint(const KVector2I32 &Point){
        // convert from viewport to homogeneous coordinates
        KVector2F32 coordinates;
        coordinates.x = -1.f + 2.f * (Point.x - _kviewport.x) / _kviewport.width;
        coordinates.y = 1.f  - 2.f * (Point.y - _kviewport.y)  / _kviewport.height;

        // transform by the inverse of the view matrix
        return getInverseTransform().transformPoint(KVector2F32(coordinates.x, coordinates.y));
    }
}
