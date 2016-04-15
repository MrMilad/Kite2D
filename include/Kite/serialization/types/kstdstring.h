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
#ifndef KSTDSTRING_H
#define KSTDSTRING_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include <string>

namespace Kite {
	static KBaseSerial &operator<<(KBaseSerial &Out, const std::string &Value) {
		// at first we write size of string
		SIZE strSize = Value.size();
		Out.writePOD((const void *)&strSize, sizeof(SIZE), false);

		// write string contents
		Out.writePOD((void *)Value.c_str(), Value.size(), true);

		return Out;
	}

	static KBaseSerial &operator>>(KBaseSerial &In, std::string &Value) {
		Value.clear();

		// read size of string
		SIZE strSize = 0;
		In.readPOD((void *)&strSize, sizeof(SIZE), false);

		if (strSize > 0) {

			// read contents to a temporary buffer
			char *temp = new char[strSize];
			In.readPOD((void *)temp, strSize, true);

			// insert buffer to the string object
			Value.reserve(strSize);
			Value.insert(0, (const char *)temp, strSize);

			// free buffer
			delete[] temp;
		}

		return In;
	}
}

#endif // KSTDSTRING_H