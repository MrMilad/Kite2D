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
#include "Kite/Core/utility/kutilitydef.h"
#include "Kite/Core/utility/kvariantbase.h"
#include "Kite/Core/utility/kmeta.h"

namespace Kite {
	class KITE_FUNC_EXPORT KVariant : public KVariantBase {
	public:
		template <typename T>
		KVariant(const T& Value) :
			KVariantBase(KMETA_GET_TYPE(T), NULL)
		{
			_kdata = new char[_kobject->getSize()];
			memcpy(_kdata, Value, _kobject->getSize());
		}

		KVariant(const KMeta *Object, void *Data) :
			KVariantBase(Object, Data) 
		{}

		KVariant() :
			KVariantBase(NULL, NULL) 
		{}

		~KVariant() {
			delete[] reinterpret_cast<char *>(_kdata);
			_kdata = NULL;
		}

		KVariant& operator=(const KVariant& Right) {
			if (this == &Right)
				return *this;

			if (_kobject) {
				if (_kobject == Right.getMeta())
					memcpy(_kdata, Right.getData(), Right.getMeta()->getSize());
				else {
					KDEBUG_ASSERT(Right.getMeta()); // cannot make an instance of NULL meta!

					delete[] reinterpret_cast<char *>(_kdata);
					_kdata = NULL;

					// get meta type
					_kobject = Right.getMeta();

					// we require a new copy if meta does not match!
					if (_kobject) {
						_kdata = new char[_kobject->getSize()];
						memcpy(_kdata, Right.getData(), _kobject->getSize());
					}
				}
			}

			return *this;
		}

		template <typename TYPE>
		KVariant& operator=(const TYPE& Right) {
			// we require a new copy if meta does not match!
			if (_kobject != KMETA_GET_TYPE(T)) {
				KDEBUG_ASSERT(KMETA_GET_TYPE(T)); // cannot create instance of NULL meta!

				delete[] reinterpret_cast<char *>(_kdata);
				_kdata = NULL;

				// get meta type
				_kobject = KMETA_GET_TYPE(T);
				
				// create new copy
				_kdata = new char[_kobject->getSize()];
				memcpy(_kdata, Right.getData(), _kobject->getSize());

			} else {
				// copy
				memcpy(_kdata, Right.getData(), _kobject->getSize());
			}
			return *this;
		}
	};
}

#endif // KVARIANT_H