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

#include "Kite/core/kcoredef.h"
#include "Kite/math/kmathdef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/kmatrix3.h"
#include "Kite/meta/kmetadef.h"
#include "ktransform.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(SCRIPTABLE)
    class KITE_FUNC_EXPORT KTransform{
		KMETA_KTRANSFORM_BODY();
    public:
		/// transform a 2D point
		KM_FUN()
		static KVector2F32 transformPoint(const KMatrix3 &Matrix, const KVector2F32 &Point);

		/// transform a 2D quad
		KM_FUN()
		static KRect2F32 transformQuad(const KMatrix3 &Matrix, const KRect2F32 &Quad);

		/// combine the given matrix with a translation
		KM_FUN()
		static void translate(KMatrix3 &Matrix, const KVector2F32 &Offset);

		/// combine the given matrix with a rotation
		KM_FUN()
		static void rotate(KMatrix3 &Matrix, F32 Angle, const KVector2F32 &Center);

		/// combine the given matrix with a scaling
		KM_FUN()
		static void scale(KMatrix3 &Matrix, const KVector2F32 &ScaleFactor, const KVector2F32 &Center);
    }; // class
}

#endif // KTRANSFORM_H
