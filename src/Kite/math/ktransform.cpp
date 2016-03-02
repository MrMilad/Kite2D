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
#include "Kite/math/ktransform.h"

namespace Kite {
	KVector2F32 KTransform::transformPoint(const KMatrix3 &Matrix, const KVector2F32 &Point){
		return KVector2F32(Point.x * Matrix[0] + Point.y * Matrix[1] + 1 * Matrix[2],
						   Point.x * Matrix[3] + Point.y * Matrix[4] + 1 * Matrix[5]);
	}

	KRect2F32 KTransform::transformQuad(const KMatrix3 &Matrix, const KRect2F32 &Quad){
		// transform all 4 points
		const KVector2F32 points[] =
		{
			transformPoint(Matrix, Quad.leftBottom),
			transformPoint(Matrix, Quad.leftTop),
			transformPoint(Matrix, Quad.rightBottom),
			transformPoint(Matrix, Quad.rightTop)
		};

		return KRect2F32(points[0], points[1], points[2], points[3]);
	}

	void KTransform::translate(KMatrix3 &Matrix, const KVector2F32 &Offset) {
		// build matrix
		KMatrix3 mat(1, 0, Offset.x,
					 0, 1, Offset.y,
					 0, 0, 1);

		// combine
		Matrix *= mat;
	}

	void KTransform::rotate(KMatrix3 &Matrix, F32 Angle, const KVector2F32 &Center) {
		// calculate
		F32 rad = Angle * KMATH_PIsub180;
		F32 cos = std::cos(rad);
		F32 sin = std::sin(rad);

		// build matrix
		KMatrix3 mat(cos, -sin, Center.x * (1 - cos) + Center.y * sin,
					 sin, cos, Center.y * (1 - cos) - Center.x * sin,
					 0, 0, 1);

		// combine
		Matrix *= mat;
	}

	void KTransform::scale(KMatrix3 &Matrix, const KVector2F32 &ScaleFactor, const KVector2F32 &Center) {
		// build matrix
		KMatrix3 mat(ScaleFactor.x, 0, Center.x * (1 - ScaleFactor.x),
					 0, ScaleFactor.y, Center.y * (1 - ScaleFactor.y),
					 0, 0, 1);

		// combine
		Matrix *= mat;
	}
}