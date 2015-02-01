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

#include "Kite/core/math/kgeometric.h"

namespace Kite{
	bool KGeometric::isPointInBox(const KRectF32 &Box, const KVector2F32 &HitPoint){
		if (Box.left <= HitPoint.x && Box.bottom <= HitPoint.y
			&& (Box.right) >= HitPoint.x && (Box.top) >= HitPoint.y){
			return true;
		}
		return false;
	}

	bool KGeometric::isPointInCircle(const KVector2F32 &Center, F32 Radius, const KVector2F32 &HitPoint){
		if (distance(Center, HitPoint) <= Radius){
			return true;
		}
		return false;
	}

	bool KGeometric::isPointInPoly(const KVector2F32 *Points, U32 Size, const KVector2F32 &HitPoint){
		U32 i, j;
		for (i = 0, j = Size - 1; i < Size; j = i++) {
			if (((Points[i].y > HitPoint.y) != (Points[j].y > HitPoint.y)) &&
				(HitPoint.x < (Points[j].x - Points[i].x) * (HitPoint.y - Points[i].y) / (Points[j].y - Points[i].y) + Points[i].x))
				return true;
		}
		return false;
	}

	KRectF32 KGeometric::getBoundingBox(const KVector2F32 *Points, U32 Size){
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

		return KRectF32(minX, maxX, minY, maxY);

	}

	KRectF32 KGeometric::getBoundingBox(const KRectF32 *Rects, U32 Size){
		F32 maxX = Rects[0].right,
			maxY = Rects[0].top,
			minX = Rects[0].left,
			minY = Rects[0].bottom;

		for (U32 i = 1; i < Size; i++){
			if (maxX < Rects[i].right) maxX = Rects[i].right;
			if (maxY < Rects[i].top) maxY = Rects[i].top;
			if (minX > Rects[i].left) minX = Rects[i].left;
			if (minY > Rects[i].bottom) minY = Rects[i].bottom;
		}

		return KRectF32(minX, maxX, minY, maxY);
	}

	KRectF32 KGeometric::getBoundingBox(const KVector2F32 *Centers, F32 Radius, U32 Size){
		F32 maxX = Centers[0].x,
			maxY = Centers[0].y,
			minX = Centers[0].x,
			minY = Centers[0].y;

		for (U32 i = 1; i < Size; i++){
			if (maxX < Centers[i].x) maxX = Centers[i].x;
			if (maxY < Centers[i].y) maxY = Centers[i].y;
			if (minX > Centers[i].x) minX = Centers[i].x;
			if (minY > Centers[i].y) minY = Centers[i].y;
		}

		return KRectF32(minX - Radius, maxX + Radius, minY - Radius, maxY + Radius);
	}
}