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
#ifndef KSTDSMARTPTR_H
#define KSTDSMARTPTR_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/meta/kmetamanager.h"
#include <memory>
#include <string>

namespace Kite {

	namespace Internal {
		template<typename T2>
		struct serialHelper2<std::shared_ptr, T2> {
			static void write(KBaseSerial &Out, const std::shared_ptr<T2> &Value) {
				bool empty = true;

				if (Value) {
					empty = false;
					Out << empty;
					Out << *Value.get();
				} else {
					empty = true;
					Out << empty;
				}
			}

			static void read(KBaseSerial &In, std::shared_ptr<T2> &Value) {
				bool empty = true;
				Value.reset();

				In >> empty;

				if (!empty) {
					if (In.getAllocator() != nullptr) {
						StorageDeleter<T2> deleter;
						deleter.storage = &In.getAllocator();
						Value.reset(allocateNew<T2>(In.getAllocator()), deleter);
					} else {
						Value.reset(new T2);
					}
					
					In >> (*Value);
				}
			}
		};
	}
}

#endif // KSTDSMARTPTR_H