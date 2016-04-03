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
#ifndef KSERIALIZATION_H
#define KSERIALIZATION_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/serialization/kbaseserial.h"
#include <type_traits>

#define CHECK_VALIDATION(T) static_assert(std::is_arithmetic<T>::value, "non-pod, unregistered or unserializable types and raw pointers are not allowed for serialization.")

namespace Kite {
	// serialization (POD)
	template <typename T1>
	KBaseSerial &operator<<(KBaseSerial &Out, const T1 &Value) {
		CHECK_VALIDATION(T1);
		Out.writePOD((const void *)&Value, sizeof(T1), false);
		return Out;
	}

	// deserialization (POD)
	template <typename T1>
	KBaseSerial &operator>>(KBaseSerial &In, T1 &Value) {
		CHECK_VALIDATION(T1);
		In.readPOD((void *)&Value, sizeof(T1), false);
		return In;
	}

	// 1-dimensional array
	template<typename T, SIZE N>
	KBaseSerial &operator<<(KBaseSerial &Out, const T(&Value)[N]) {

		for (SIZE i = 0; i < N; i++) {
			Out << Value[i];
		}

		return Out;
	}

	template<typename T, SIZE N>
	KBaseSerial &operator>>(KBaseSerial &In, T(&Value)[N]) {

		for (SIZE i = 0; i < N; i++) {
			In >> Value[i];
		}

		return In;
	}

	// 2-dimensional array
	template<typename T, SIZE N, SIZE Y>
	KBaseSerial &operator<<(KBaseSerial &Out, const T(&Value)[N][Y]) {
		for (SIZE i = 0; i < N; i++) {
			for (SIZE j = 0; j < Y; j++) {
				Out << Value[i][j];
			}
		}

		return Out;
	}

	template<typename T, SIZE N, SIZE Y>
	KBaseSerial &operator>>(KBaseSerial &In, T(&Value)[N][Y]) {
		for (SIZE i = 0; i < N; i++) {
			for (SIZE j = 0; j < Y; j++) {
				In >> Value[i][j];
			}
		}

		return In;
	}
}

#endif // KSERIALIZATION_H