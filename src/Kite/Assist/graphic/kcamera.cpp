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
#include "Kite/Assist/graphic/kcamera.h"
#include <cmath>

namespace Kite{
    KCamera::KCamera():
        _kcenter(0, 0),
        _krotation(0.0f),
        _kzoom(1.0f),
        _kneedUpdate(true),
		_kmatrix()
    {}

	KCamera::KCamera(const KRectF32 &Viewport) :
		_krotation(0.0f),
		_kzoom(1.0f),
		_kneedUpdate(true),
		_kmatrix()
	{
		setViewport(Viewport);
	}

	void KCamera::setViewport(const KRectF32 &Viewport){
		_kcenter.x = (Viewport.left + Viewport.right) / 2.0f;
		_kcenter.y = (Viewport.top + Viewport.bottom) / 2.0f;
		_ksize.x = Viewport.right - Viewport.left;
		_ksize.y = Viewport.top - Viewport.bottom;
	}

    const KMatrix3 &KCamera::getMatrix() const{
        // recompute the matrix if needed
        if (_kneedUpdate){
            // rotation components
            F32 angle	= _krotation * KMATH_PIsub180;
            F32 cosx	= (F32)(std::cos(angle));
            F32 sinx	= (F32)(std::sin(angle));
			//F32 tx = -(_kviewport.right + _kviewport.left) / (_kviewport.right - _kviewport.left);
			//F32 ty = -(_kviewport.top + _kviewport.bottom) / (_kviewport.top - _kviewport.bottom);
			F32 tx = -_kcenter.x * cosx - _kcenter.y * sinx + _kcenter.x;
			F32 ty = _kcenter.x * sinx - _kcenter.y * cosx + _kcenter.y;

           // projection components
           F32 a = 2.f / (_ksize.x / _kzoom);
           F32 b = -2.f / (_ksize.y / _kzoom);
		   F32 c = -a * _kcenter.x;
		   F32 d = -b * _kcenter.y;

            // rebuild the projection matrix
           _kmatrix = KMatrix3( a * cosx,	a * sinx,   a * tx + c,
                                -b * sinx,  b * cosx,	b * ty + d,
                                0.0f,       0.0f,       1.0f);

		   /*_kmatrix = KMatrix3(	a,		0.0f,	tx,
								0.0f,	b,		ty,
								0.0f,	0.0f,	1.0f);*/

            _kneedUpdate = false;
        }

		return _kmatrix;
    }
}
