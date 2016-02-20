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
#ifndef KREFVARIANT_H
#define KREFVARIANT_H

#include "Kite/core/kcoredef.h"
#include "Kite/utility/kutilitydef.h"
#include "Kite/core/kvariantbase.h"
#include "Kite/core/kvariant.h"
#include "Kite/meta/kmetaobject.h"

namespace Kite {
	class KRefVariant : public KVariantBase {
	public:
		template <typename T>
		KRefVariant(const T& Value) :
			KVariantBase(const_cast<T *>(&Value), sizeof(T)) 
		{}

		KRefVariant(const KRefVariant& Right) :
			KVariantBase(Right.getData(), Right.getSize())
		{}

		KRefVariant(const KVariant& Right):
			KVariantBase(Right.getData(), Right.getSize())
		{}

		KRefVariant(void *Data, SIZE Size) :
			KVariantBase(Data, Size)
		{}

		KRefVariant() :
			KVariantBase(nullptr, 0) 
		{}
			
		KRefVariant& operator=(const KRefVariant& Right) {
			_kdata = Right.getData();
			_ksize = Right.getSize();
			return *this;
		}

		KRefVariant& operator=(const KVariant& Right) {
			_kdata = Right.getData();
			_ksize = Right.getSize();
			return *this;
		}

		template <typename T>
		KRefVariant& operator=(const T& Right) {
			_kdata = const_cast<T *>(&Right);
			_ksize = sizeof(T);
			return *this;
		}

		template <typename T>
		KRefVariant& copyByVal(const T& Right) {
			T *ptr = (T *)_kdata;
			(*ptr) = Right;
			return *this;
		}
	};
}

#endif // KREFVARIANT_H