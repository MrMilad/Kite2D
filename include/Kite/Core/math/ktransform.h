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
#ifndef KTRANSFORM_H
#define KTRANSFORM_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/math/kmatrix3.h"
#include "Kite/Core/utility/kserialize.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTransform{
		friend KSerialize &operator<<(KSerialize &Out, const KTransform &Value);
		friend KSerialize &operator>>(KSerialize &In, KTransform &Value);
    public:
		/// construct a transform (unit matrix)
		KTransform();

		/// construct a transform from a 3x3 matrix
		KTransform(const KMatrix3 &Matrix);

		/// return the transform as a 3x3 matrix
		inline const KMatrix3 *getMatrix() const { return &_kmatrix; }

		/// transform a 2D point
		KVector2F32 transformPoint(const KVector2F32 &Point) const;

		/// transform a rectangle
		KRect2F32 transformQuad(const KRect2F32 &Quad) const;

		/// combine the current transform with another one
		KTransform &combine(const KTransform &Transform);

		/// combine the current transform with a translation
		KTransform &translate(const KVector2F32 &Offset);

		/// combine the current transform with a rotation
		KTransform &rotate(F32 Angle, const KVector2F32 &Center);

		/// combine the current transform with a scaling
		KTransform &scale(const KVector2F32 &ScaleFactor, const KVector2F32 &Center);

	private:
		KMatrix3 _kmatrix; // 3x3 matrix defining the transformation
    }; // class
}

#endif // KTRANSFORM_H
