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
#ifndef KSTDARRAY_H
#define KSTDARRAY_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/kserialization.h"
#include <array>

namespace Kite {

	template<typename T1, size_t T2>
	KBaseSerial &operator<<(KBaseSerial &Out, const std::array<T1, T2> &Value) {
		const size_t size = T2;

		Out << size;
		for (auto it = Value.begin(); it != Value.end(); ++it) {
			Out << (*it);
		}

		return Out;
	}

	template<typename T1, size_t T2>
	KBaseSerial &operator>>(KBaseSerial &In, std::array<T1, T2> &Value) {
		size_t size = 0;

		// check array size
		In >> size;
		if (size == T2) {
			for (SIZE i = 0; i < size; ++i) {
				In >> Value[i];
			}
		}
		return In;
	}
}

#endif // KSTDARRAY_H