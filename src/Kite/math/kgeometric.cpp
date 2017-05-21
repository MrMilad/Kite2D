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

#include "Kite/math/kgeometric.h"

namespace Kite {
	bool KGeometric::circleContinePoint(const KVector2F32 &Center, F32 Radius, const KVector2F32 &HitPoint) {
		if (Center.distance(HitPoint) <= Radius) {
			return true;
		}
		return false;
	}

	bool KGeometric::polygonContinePoint(const std::vector<KVector2F32> &Points, const KVector2F32 &HitPoint) {
		I32 i = 0, j = 0;
		bool c = false;
		for (i = 0, j = Points.size() - 1; i < Points.size(); j = i++) {
			if (((Points[i].y > HitPoint.y) != (Points[j].y > HitPoint.y)) &&
				(HitPoint.x < (Points[j].x - Points[i].x) * (HitPoint.y - Points[i].y) / (Points[j].y - Points[i].y) + Points[i].x))
				c = !c;
		}
		return c;
	}

	KRectF32 KGeometric::aabbPoints(const std::vector<KVector2F32> &Points) {
		if (Points.empty()) return KRectF32();

		F32 maxX = Points[0].x,
			maxY = Points[0].y,
			minX = Points[0].x,
			minY = Points[0].y;

		for (auto it = Points.begin(); it != Points.end(); ++it) {
			if (maxX < it->x) maxX = it->x;
			if (maxY < it->y) maxY = it->y;
			if (minX > it->x) minX = it->x;
			if (minY > it->y) minY = it->y;
		}

		return KRectF32(minX, maxX, minY, maxY);

	}

	KRectF32 KGeometric::aabbRects(const std::vector<KRectF32> &Rects) {
		if (Rects.empty()) return KRectF32();

		F32 maxX = Rects[0].right,
			maxY = Rects[0].top,
			minX = Rects[0].left,
			minY = Rects[0].bottom;

		for (auto it = Rects.begin(); it != Rects.end(); ++it) {
			if (maxX < it->right) maxX = it->right;
			if (maxY < it->top) maxY = it->top;
			if (minX > it->left) minX = it->left;
			if (minY > it->bottom) minY = it->bottom;
		}

		return KRectF32(minX, maxX, minY, maxY);
	}

	KRectF32 KGeometric::aabbCircles(const std::vector<KVector2F32> &Centers, F32 Radius) {
		if (Centers.empty()) return KRectF32();

		F32 maxX = Centers[0].x,
			maxY = Centers[0].y,
			minX = Centers[0].x,
			minY = Centers[0].y;

		for (auto it = Centers.begin(); it != Centers.end(); ++it) {
			if (maxX < it->x) maxX = it->x;
			if (maxY < it->y) maxY = it->y;
			if (minX > it->x) minX = it->x;
			if (minY > it->y) minY = it->y;
		}

		return KRectF32(minX - Radius, maxX + Radius, minY - Radius, maxY + Radius);
	}

	Orientation KGeometric::pointsOrientation(const KVector2F32 &PointA, const KVector2F32 &PointB, const KVector2F32 &PointC) {
		F32 area2 = (PointB.x - PointA.x)*(PointC.y - PointA.y) - (PointB.y - PointA.y)*(PointC.x - PointA.x);
		if (area2 < 0) return Orientation::CLOCKWISE;
		else if (area2 > 0) return Orientation::CCLOCKWISE;
		return Orientation::COLLINEAR;
	}

	bool KGeometric::lineIntersect(const KVector2F32 &Line1Start, const KVector2F32 &Line1End,
								   const KVector2F32 &Line2Start, const KVector2F32 &Line2End,
								   KVector2F32 *OutPoint = nullptr) {
		float s1_x, s1_y, s2_x, s2_y;
		s1_x = Line1End.x - Line1Start.x;     s1_y = Line1End.y - Line1Start.y;
		s2_x = Line2End.x - Line2Start.x;     s2_y = Line2End.y - Line2Start.y;

		float s, t;
		s = (-s1_y * (Line1Start.x - Line2Start.x) + s1_x * (Line1Start.y - Line2Start.y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (Line1Start.y - Line2Start.y) - s2_y * (Line1Start.x - Line2Start.x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			// Collision detected
			if (OutPoint != nullptr) {
				OutPoint->x = Line1Start.x + (t * s1_x);
				OutPoint->y = Line1Start.y + (t * s1_y);
			}
			return true;
		}

		return false; // No collision
	}

	bool  KGeometric::circleIntersect(const KVector2F32 &Center1, F32 Radius1, const KVector2F32 &Center2, F32 Radius2) {
		if (Center1.distance(Center2) <= (Radius1 + Radius2)) {
			return true;
		}
		return false;
	}
}