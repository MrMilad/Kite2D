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
#include "Kite/core/math/ktransform.h"
#include <cmath>

namespace Kite{
	KTransform::KTransform() :
		_kmatrix()
	{}

	KTransform::KTransform(const KMatrix3 &Matrix) :
		_kmatrix(Matrix)
	{}

	KVector2F32 KTransform::transformPoint(const KVector2F32 &Point) const{
		return KVector2F32(Point.x * _kmatrix[0] + Point.y * _kmatrix[1] + 1 * _kmatrix[2],
			Point.x * _kmatrix[3] + Point.y * _kmatrix[4] + 1 * _kmatrix[5]);
	}

	KRect2F32 KTransform::transformQuad(const KRect2F32 &Quad) const{
		// transform all 4 points
		const KVector2F32 points[] =
		{
			transformPoint(Quad.leftBottom),
			transformPoint(Quad.leftTop),
			transformPoint(Quad.rightBottom),
			transformPoint(Quad.rightTop)
		};

		return KRect2F32(points[0], points[1], points[2], points[3]);
	}

	KTransform &KTransform::combine(const KTransform &Transform){
		_kmatrix *= (*Transform.getMatrix());
		return *this;
	}

	KTransform &KTransform::translate(const KVector2F32 &Offset){
		// build matrix
		KMatrix3 mat(1, 0, Offset.x,
					0, 1, Offset.y,
					0, 0, 1);

		// combine
		_kmatrix *= mat;
		
		return *this;
	}

	KTransform &KTransform::rotate(F32 Angle, const KVector2F32 &Center){
		// calculate
		F32 rad = Angle * KMATH_PIsub180;
		F32 cos = std::cos(rad);
		F32 sin = std::sin(rad);

		// build matrix
		KMatrix3 mat(cos, -sin, Center.x * (1 - cos) + Center.y * sin,
			sin, cos, Center.y * (1 - cos) - Center.x * sin,
			0, 0, 1);

		// combine
		_kmatrix *= mat;

		return *this;
	}

	KTransform &KTransform::scale(const KVector2F32 &ScaleFactor, const KVector2F32 &Center){
		// build matrix
		KMatrix3 mat(ScaleFactor.x, 0, Center.x * (1 - ScaleFactor.x),
			0, ScaleFactor.y, Center.y * (1 - ScaleFactor.y),
			0, 0, 1);

		// combine
		_kmatrix *= mat;

		return *this;
	}
}
