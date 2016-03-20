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
#ifndef KVARIANTBASE_H
#define KVARIANTBASE_H

#include "Kite/core/kcoredef.h"
#include <typeinfo>

namespace Kite {
	class KITE_FUNC_EXPORT KVariantBase {
	public:

		template <typename T>
		T *getDataSafe(){
			if (typeid(T).hash_code() == _ktype) {
				return reinterpret_cast<T *>(_kdata);
			}

			KDEBUG_PRINT("type mismatch");
			return nullptr;
		}

		template <typename T>
		const T *getDataSafe() const {
			if (typeid(T).hash_code() == _ktype) {
				return reinterpret_cast<T *>(_kdata);
			}

			KDEBUG_PRINT("type mismatch");
			return nullptr;
		}

		template <typename T>
		T *getData() {
			return reinterpret_cast<T *>(_kdata);
		}

		template <typename T>
		const T *getData() const {
			return reinterpret_cast<T *>(_kdata);
		}

		inline const void *getData() const { return _kdata; }

		inline void *getData() { return _kdata; }

		inline SIZE getSize() const { return _ksize; }

		inline SIZE getType() const { return _ktype; }

	protected:
		KVariantBase() :
			 _kdata(nullptr), _ksize(0), _ktype(0)
		{}

		KVariantBase(void* Data, SIZE Size, SIZE Type) :
			_kdata(Data), _ksize(Size), _ktype(Type)
		{}

		void *_kdata;
		SIZE _ksize;
		SIZE _ktype;
	};
}

#endif // KVARIANTBASE_H