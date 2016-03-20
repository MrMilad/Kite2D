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
#ifndef KSTDLIST_H
#define KSTDLIST_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include <vector>

namespace Kite {

	namespace Internal {
		template<typename T2, typename T3>
		struct serialHelper3<std::vector, T2, T3> {
			static void write(KBaseSerial &Out, const std::vector<T2, T3> &Value) {
				bool empty = true;
				U32 size = 0;

				if (!Value.empty()) {
					empty = false;
					size = Value.size();
					Out << empty;
					Out << size;
					for (auto it = Value.begin(); it != Value.end(); ++it) {
						Out << (*it);
					}
				} else {
					empty = true;
					Out << empty;
				}
			}

			static void read(KBaseSerial &In, std::vector<T2, T3> &Value) {
				bool empty = true;
				U32 size = 0;

				In >> empty;

				if (!empty) {
					In >> size;
					if (Value.size() >= size) {
						for (auto it = Value.begin(); it != Value.end(); ++it) {
							In >> (*it);
						}
					}
				}
			}
		};
	}
}

#endif // KSTDLIST_H