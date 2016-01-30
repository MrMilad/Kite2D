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
#ifndef KVARIANT_H
#define KVARIANT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kvariantbase.h"
#include "Kite/Core/meta/kmetaobject.h"

namespace Kite {
	class KITE_FUNC_EXPORT KVariant : public KVariantBase {
	public:
		template <typename T>
		KVariant(const T& Value) :
			KVariantBase(nullptr, sizeof(T))
		{
			_kdata = new char[getSize()];
			memcpy(_kdata, Value, getSize());
		}

		KVariant(void *Data, SIZE Size) :
			KVariantBase(Data, Size) 
		{}

		KVariant() :
			KVariantBase(nullptr, 0) 
		{}

		~KVariant() {
			delete[] reinterpret_cast<char *>(_kdata);
			_kdata = nullptr;
			_ksize = 0;
		}

		KVariant& operator=(const KVariant& Right) {
			if (this == &Right)
				return *this;

			if (_ksize == Right.getSize())
				memcpy(_kdata, Right.getData(), Right.getSize());
			else {
				delete[] reinterpret_cast<char *>(_kdata);
				_kdata = nullptr;

				// get meta type
				_ksize = Right.getSize();

				// we require a new copy if meta does not match!
				_kdata = new char[getSize()];
				memcpy(_kdata, Right.getData(), getSize());
			}

			return *this;
		}

		template <typename TYPE>
		KVariant& operator=(const TYPE& Right) {
			// we require a new copy if meta does not match!
			if (_ksize == sizeof(TYPE)) {
				// copy
				memcpy(_kdata, Right.getData(), getSize());

			} else {

				delete[] reinterpret_cast<char *>(_kdata);
				_kdata = nullptr;

				// get meta type
				_ksize = sizeof(TYPE);

				// create new copy
				_kdata = new char[getSize()];
				memcpy(_kdata, Right.getData(), getSize());
			}
			return *this;
		}
	};
}

#endif // KVARIANT_H