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

	KOrientationTypes KGeometric::getOrientation(const KVector2F32 &PointA, const KVector2F32 &PointB, const KVector2F32 &PointC){
		F32 area2 = (PointB.x - PointA.x)*(PointC.y - PointA.y) - (PointB.y - PointA.y)*(PointC.x - PointA.x);
		if (area2 < 0) return KO_CLOCKWISE;
		else if (area2 > 0) return KO_CCLOCKWISE;
		return KO_COLLINEAR;
	}

	U32 KGeometric::getConvexHull(const KVector2F32 *Points, U32 Size, KVector2F32 *ConvexHull){
		// initialize a deque D[] from bottom to top so that the
		// 1st three vertices of P[] are a ccw triangle
		KVector2F32* D = new KVector2F32[2 * Size + 1];
		int bot = Size - 2, top = bot + 3;    // initial bottom and top deque indices
		D[bot] = D[top] = Points[2];        // 3rd vertex is at both bot and top
		if (getOrientation(Points[0], Points[1], Points[2]) > 0) {
			D[bot + 1] = Points[0];
			D[bot + 2] = Points[1];           // ccw vertices are: 2,0,1,2
		}
		else {
			D[bot + 1] = Points[1];
			D[bot + 2] = Points[0];           // ccw vertices are: 2,1,0,2
		}

		// compute the hull on the deque D[]
		for (int i = 3; i < Size; i++) {   // process the rest of vertices
			// test if next vertex is inside the deque hull
			if ((getOrientation(D[bot], D[bot + 1], Points[i]) > 0) &&
				(getOrientation(D[top - 1], D[top], Points[i]) > 0))
				continue;         // skip an interior vertex

			// incrementally add an exterior vertex to the deque hull
			// get the rightmost tangent at the deque bot
			while (getOrientation(D[bot], D[bot + 1], Points[i]) <= 0)
				++bot;                 // remove bot of deque
			D[--bot] = Points[i];           // insert Points[i] at bot of deque

			// get the leftmost tangent at the deque top
			while (getOrientation(D[top - 1], D[top], Points[i]) <= 0)
				--top;                 // pop top of deque
			D[++top] = Points[i];           // push Points[i] onto top of deque
		}

		// transcribe deque D[] to the output hull array ConvexHull[]
		int count;        // hull vertex counter
		for (count = 0; count <= (top - bot); count)
			ConvexHull[count] = D[bot + count];

		delete D;
		return count - 1;
	}

	bool KGeometric::isIntersect(const KVector2F32 &Line1Start, const KVector2F32 &Line1End,
		const KVector2F32 &Line2Start, const KVector2F32 &Line2End){
		U32 test1, test2;
		test1 = getOrientation(Line1Start, Line1End, Line2Start) * getOrientation(Line1Start, Line1End, Line2End);
		test2 = getOrientation(Line2Start, Line2End, Line1Start) * getOrientation(Line2Start, Line2End, Line1End);
		return (test1 <= 0) && (test2 <= 0);
	}

	bool  KGeometric::isIntersect(const KVector2F32 &Center1, F32 Radius1, const KVector2F32 &Center2, F32 Radius2){
		if (distance(Center1, Center2) <= (Radius1 + Radius2)){
			return true;
		}
		return false;
	}
}