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

#include "Kite/core/physique/kpointinpoly.h"

namespace Kite{
	bool KPointInPoly::isPointInBox(const KRectF32 &Box, const KVector2F32 &HitPoint){
		if (Box.left <= HitPoint.x && Box.bottom <= HitPoint.y
			&& (Box.right) >= HitPoint.x && (Box.top) >= HitPoint.y){
			return true;
		}
		return false;
	}

	bool KPointInPoly::isPointInCircle(const KVector2F32 &Center, F32 Radius, const KVector2F32 &HitPoint){
		if (distance(Center, HitPoint) <= Radius){
			return true;
		}
		return false;
	}

	bool KPointInPoly::isPointInPoly(const KVector2F32 *Points, U32 Size, const KVector2F32 &HitPoint){
		U32 i, j;
		for (i = 0, j = Size - 1; i < Size; j = i++) {
			if (((Points[i].y > HitPoint.y) != (Points[j].y > HitPoint.y)) &&
				(HitPoint.x < (Points[j].x - Points[i].x) * (HitPoint.y - Points[i].y) / (Points[j].y - Points[i].y) + Points[i].x))
				return true;
		}
		return false;
	}

	KRectF32 KPointInPoly::getBoundingBox(const KVector2F32 *Points, U32 Size){
		F32 maxX = Points[0].x,
			maxY = Points[0].y,
			minX = Points[0].x,
			minY = Points[0].y;

		for (U32 i = 1; i < Size; i++){
			if (maxX < Points[i].x) maxX = Points[i].x;
			if (maxY < Points[i].y) maxY = Points[i].y;
			if (minX > Points[i].x) minX = Points[i].x;
			if (minY > Points[i].y) minY = Points[i].y;
		}

		return KRectF32(minX, maxX, maxY, minY);

	}

	KRectF32 KPointInPoly::getBoundingBox(const KRectF32 *Rects, U32 Size){
		return KRectF32(0, 0, 0, 0);
	}

	KRectF32 KPointInPoly::getBoundingBox(const KVector2F32 *Centers, const F32 *Radiuses, U32 Size){
		return KRectF32(0, 0, 0, 0);
	}
}