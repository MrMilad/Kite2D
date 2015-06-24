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
#ifndef KTRANSFORMABLEGROUP_H
#define KTRANSFORMABLEGROUP_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Assist/graphic/ktransformable.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KTransformableGroup {
	public:
		void addObject(KTransformable *Object);

		void removeAllObjects();

		void setPosition(const KVector2F32& Position);

		/// move objects
		/// adds to the current position
		void move(const KVector2F32 &Steps);

		/// completely restore the previous Rotation
		/// range [0, 360]
		/// default 0
		void setRotation(F32 Angle);

		/// adds to the current rotation
		void rotate(F32 Angle);

		/// set scale
		/// completely restore the previous scale
		/// default (1, 1)
		void setScale(const KVector2F32 &Scale);

		/// multiplies the current scale
		void scale(const KVector2F32 &Scale);

		/// set center of (position, scale, rotation)
		/// relative to the top-left
		/// default (0, 0)
		void setCenter(const KVector2F32 &Center);

	private:
		std::vector<KTransformable *> _klist;
	};
}

#endif // KTRANSFORMABLEGROUP_H