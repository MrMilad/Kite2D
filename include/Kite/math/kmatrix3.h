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
#ifndef KMATRIX3_H
#define KMATRIX3_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbaseserial.h"
#include "kmatrix3.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(POD)
    class KITE_FUNC_EXPORT KMatrix3{
		KM_INFO(KI_NAME = "Matrix3");
		KMATRIX3_BODY();
    public:
		KM_CON(F32, F32, F32, F32, F32, F32, F32, F32, F32)
		KMatrix3(F32 E00 = 1.0f, F32 E01 = 0.0f, F32 E02 = 0.0f,
				 F32 E10 = 0.0f, F32 E11 = 1.0f, F32 E12 = 0.0f,
				 F32 E20 = 0.0f, F32 E21 = 0.0f, F32 E22 = 1.0f);

        inline const F32 *getArray() const {return _kmatrix;}

		KM_FUN()
		inline const F32 at(U8 Index) const { if (Index < 9) return _kmatrix[Index]; return 0; }

		inline float KMatrix3::operator[](int index) const { return _kmatrix[index]; }

		inline float& KMatrix3::operator[](int index) { return _kmatrix[index]; }

		KMatrix3 operator+(const KMatrix3 &right) const;

		KMatrix3 operator-(const KMatrix3 &right) const;

		KMatrix3 operator-(const KMatrix3& right);

		KMatrix3 &operator+=(const KMatrix3 &right);

		KMatrix3 &operator-=(const KMatrix3 &right);

		KMatrix3 operator*(const KMatrix3& right) const;

		KMatrix3& KMatrix3::operator*=(const KMatrix3& right);

		bool KMatrix3::operator==(const KMatrix3& right) const;

		bool KMatrix3::operator!=(const KMatrix3& right) const;

		KM_FUN()
		static const KMatrix3 &getIdentity();

    private:
		KM_OPE(KO_ADD)
		KMatrix3 luaAddOpr(const KMatrix3 &right) const;

		KM_OPE(KO_SUB)
		KMatrix3 luaSubOpr(const KMatrix3 &right) const;

		KM_OPE(KO_MUL)
		KMatrix3 luaMulOpr(const KMatrix3 &right) const;

        KM_VAR(UNBIND) F32 _kmatrix[9];
    };
}

#endif // KMATRIX3_H
