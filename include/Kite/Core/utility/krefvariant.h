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

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitydef.h"
#include "Kite/Core/utility/kvariantbase.h"
#include "Kite/Core/utility/kvariant.h"
#include "Kite/Core/utility/kmetaobject.h"
#include "Kite/Core/utility/kbytesarray.h"

namespace Kite {
	class KRefVariant : public KVariantBase {
		// KrefVariant
		friend KBytesArray &operator<<(KBytesArray &Out, const KRefVariant &Value) {
			const KMetaObject *meta = Value.getMeta();
			void *data = Value.getData();

			// primitive data
			if (meta->getFlag() & KMFLAG_POD) {

				// bool
				if (meta->getFlag() & KMFLAG_POD_BOOL) {
					const bool *pointer = (const bool *)data;
					Out << *pointer;
					return Out;

				// string 
				} else if (meta->getFlag() & KMFLAG_POD_STRING) {
					const std::string *pointer = (const std::string *)data;
					Out << *pointer;
					return Out;
				}

				// integers
				Out << KVoidStream(meta->getSize(), data);
				return Out;

			// non-primitive with memeber (recursive)
			} else if (meta->hasMembers()) {
				const KMetaMember *mem = meta->getMembers();
				while (mem) {
					void *offsetData = KPTR_GET_ADD(Value.getData(), mem->offset);
					Out << KRefVariant(mem->meta, offsetData);
					mem = mem->next;
				}
			}

			// non-primitive without mmber
			return Out;
		}

		friend KBytesArray &operator>>(KBytesArray &In, KRefVariant &Value) {
			const KMetaObject *meta = Value.getMeta();
			void *data = Value.getData();

			// primitive data
			if (meta->getFlag() & KMFLAG_POD) {

				// bool
				if (meta->getFlag() & KMFLAG_POD_BOOL) {
					bool *pointer = (bool *)data;
					In >> *pointer;
					return In;

					// string 
				} else if (meta->getFlag() & KMFLAG_POD_STRING) {
					std::string *pointer = (std::string *)data;
					In >> *pointer;
					return In;
				}

				// integers
				In >> KVoidStream(meta->getSize(), data);
				return In;

				// non-primitive with memeber (recursive)
			} else if (meta->hasMembers()) {
				const KMetaMember *mem = meta->getMembers();
				while (mem) {
					void *offsetData = KPTR_GET_ADD(Value.getData(), mem->offset);
					In >> KRefVariant(mem->meta, offsetData);
					mem = mem->next;
				}
			}

			return In;
		}

	public:
		template <typename T>
		KRefVariant(const T& Value) :
			KVariantBase(KMETA_GET_TYPE(T), const_cast<T *>(&Value)) 
		{}

		KRefVariant(const KRefVariant& Right) :
			KVariantBase(Right.getMeta(), Right.getData())
		{}

		KRefVariant(const KVariant& Right):
			KVariantBase(Right.getMeta(), Right.getData())
		{}

		KRefVariant(const KMetaObject *Object, void *Data) :
			KVariantBase(Object, Data)
		{}

		KRefVariant() :
			KVariantBase(NULL, NULL) 
		{}
			
		KRefVariant& operator=(const KRefVariant& Right) {
			_kobject = Right.getMeta();
			_kdata = Right.getData();
			return *this;
		}

		KRefVariant& operator=(const KVariant& Right) {
			_kobject = Right.getMeta();
			_kdata = Right.getData();
			return *this;
		}

		template <typename T>
		KRefVariant& operator=(const T& Right) {
			_kobject = KMETA_GET_TYPE(T);
			_kdata = const_cast<T *>(&Right);
			return *this;
		}
	};
}

#endif // KREFVARIANT_H