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
#ifndef KSTREAM_H
#define KSTREAM_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kinputstream.h"
#include <cstring>

namespace Kite{
	class KITE_FUNC_EXPORT KStream{
	public:
		// return an input stream
		// return 0 on error
		virtual KInputStream *openRead(const std::string &Name) = 0;
	};
}

#endif // KSTREAM_H