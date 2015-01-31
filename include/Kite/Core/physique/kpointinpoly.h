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
#ifndef KPOINTINPOLY_H
#define KPOINTINPOLY_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/core/graphic/kgraphicstructs.h"
#include "Kite/Core/math/kvector2.h"

namespace Kite{
	class KITE_FUNC_EXPORT KPointInPoly{
	public:
		static bool isPointInBox(const KRectF32 &Box, const KVector2F32 &HitPoint);
		static bool isPointInCircle(const KVector2F32 &Center, F32 Radius, const KVector2F32 &HitPoint);
		static bool isPointInPoly(const KVector2F32 *Points, U32 Size, const KVector2F32 &HitPoint);

		// bonding box points
		static KRectF32 getBoundingBox(const KVector2F32 *Points, U32 Size);

		// bounding box rectangles
		static KRectF32 getBoundingBox(const KRectF32 *Rects, U32 Size);

		// bounding box cycles
		static KRectF32 getBoundingBox(const KVector2F32 *Centers, const F32 *Radiuses, U32 Size);
	};
}

#endif // KPOINTINPOLY_H