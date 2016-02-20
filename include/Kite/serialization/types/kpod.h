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
#ifndef KPOD_H
#define KPOD_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"

#define CHECK_VALIDATION(T) static_assert(std::is_arithmetic<T>::value, "non-pod unregistered types and raw pointers are not allowed for serialization.")

namespace Kite {
	namespace Internal {
		template<typename T>
		struct serialHelper1 {
			static void write(KBaseSerial &Out, const T &Value) {
				CHECK_VALIDATION(T);

				Out.writePOD((const void *)&Value, sizeof(T), false);
				return Out;
			}
		};
	}
}

#endif // KPOD_H