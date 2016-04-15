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
#ifndef KSTDDEQUE_H
#define KSTDDEQUE_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include <deque>

namespace Kite {

	template<typename T1>
	KBaseSerial &operator<<(KBaseSerial &Out, const std::deque<T1> &Value) {
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

		return Out;
	}

	template<typename T1>
	KBaseSerial &operator>>(KBaseSerial &In, std::deque<T1> &Value) {
		bool empty = true;
		U32 size = 0;
		Value.clear();

		In >> empty;

		if (!empty) {
			In >> size;
			Value.resize(size);
			for (auto it = Value.begin(); it != Value.end(); ++it) {
				In >> (*it);
			}
		}

		return In;
	}
}

#endif // KSTDDEQUE_H