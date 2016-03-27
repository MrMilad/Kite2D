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
#ifndef KBASESERIAL_H
#define KBASESERIAL_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"

namespace Kite {

	enum class KSerialStateTypes : U8 {
		KST_SERIALIZE,
		KST_DESERIALIZE
	};

	class KITE_FUNC_EXPORT KBaseSerial {
	public:
		virtual void writePOD(const void *Value, SIZE Size, bool Str) = 0;
		virtual void readPOD(void *Value, SIZE Size, bool Str) = 0;
	};
}

#endif // KBASESERIAL_H
