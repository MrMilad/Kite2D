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
#ifndef KMETAFUNCTION_H
#define KMETAFUNCTION_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitytypes.h"
#include "Kite/Core/utility/kmeta.h"
#include <string>

namespace Kite {
	struct KMetaFuncParam {
		std::string name;		//!< Name of the member
		I32 value;				//!< Value of the member
		U32 number;				//!< Number of the member in given order
		KMetaFuncParam *next;	//!< Pointer to next member

		KMetaFuncParam(const std::string &Name, I32 Value, U32 Number) :
			name(Name), value(Value), number(Number) {
		}
	};

	class KITE_FUNC_EXPORT KMetaFunction : public KMeta {
	public:

	};
}

#endif // KMETAFUNCTION_H

