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
#include "Kite/core/kvariantbase.h"
#include "Kite/core/kvariant.h"

namespace Kite {
	class KRefVariant : public KVariantBase {
	public:
		template <typename T>
		KRefVariant(T &Value) :
			KVariantBase(&Value, sizeof(T), typeid(T).hash_code())
		{}

		KRefVariant(KRefVariant& Right) :
			KVariantBase(Right.getData(), Right.getSize(), Right.getType())
		{}

		KRefVariant(KVariant& Right):
			KVariantBase(Right.getData(), Right.getSize(), Right.getType())
		{}

		KRefVariant() :
			KVariantBase(nullptr, 0, 0) 
		{}
			
		KRefVariant& operator=(KRefVariant& Right) {
			_kdata = Right.getData();
			_ksize = Right.getSize();
			return *this;
		}

		KRefVariant& operator=(KVariant& Right) {
			_kdata = Right.getData();
			_ksize = Right.getSize();
			_ktype = Right.getType();
			return *this;
		}

		template <typename T>
		KRefVariant& operator=(const T& Right) {
			_kdata = const_cast<T *>(&Right);
			_ksize = sizeof(T);
			_ktype = typeid(T).hash_code();
			return *this;
		}
	};
}

#endif // KREFVARIANT_H