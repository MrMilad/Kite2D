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
#ifndef KSTDBITSET_H
#define KSTDBITSET_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdstring.h"
#include <bitset>

namespace Kite {

	template<SIZE T1>
	KBaseSerial &operator<<(KBaseSerial &Out, const std::bitset<T1> &Value) {
		Out << Value.to_string();
		return Out;
	}

	template<SIZE T1>
	KBaseSerial &operator>>(KBaseSerial &In, std::bitset<T1> &Value) {
		std::string val;
		In >> val;
		Value = std::bitset<T1>(val);
		return In;
	}
}

#endif // KSTDBITSET_H