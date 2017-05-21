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
#ifndef KGEOMETRIC_H
#define KGEOMETRIC_H

#include "Kite/core/kcoredef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/kmathtypes.h"

namespace Kite{
	class KITE_FUNC_EXPORT KGeometric{
	public:
		KM_FUN()
		static bool circleContinePoint(const KVector2F32 &Center, F32 Radius, const KVector2F32 &Point);

		static bool polygonContinePoint(const std::vector<KVector2F32> &Points, const KVector2F32 &HitPoint);

		// bounding box points
		static KRectF32 aabbPoints(const std::vector<KVector2F32> &Points);

		// bounding box rectangles
		static KRectF32 aabbRects(const std::vector<KRectF32> &Rects);

		// bounding box circles (circles with same radius)
		static KRectF32 aabbCircles(const std::vector<KVector2F32> &Centers, F32 Radius);

		// get orientation of 3 points
		static Orientation pointsOrientation(const KVector2F32 &PointA, const KVector2F32 &PointB, const KVector2F32 &PointC);

		// line intersect
		static bool lineIntersect(const KVector2F32 &Line1Start, const KVector2F32 &Line1End,
								  const KVector2F32 &Line2Start, const KVector2F32 &Line2End,
								  KVector2F32 *OutPoint = nullptr);

		// circle intersect
		static bool circleIntersect(const KVector2F32 &Center1, F32 Radius1, const KVector2F32 &Center2, F32 Radius2);
	};
}

#endif // KGEOMETRIC_H