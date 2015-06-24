/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KINPUTSTREAM_H
#define KINPUTSTREAM_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
	class KITE_FUNC_EXPORT KInputStream{
	public:
		virtual ~KInputStream(){}

		virtual U64 read(void *Data, U64 DataSize) = 0;

		virtual I32 seek(I64 Offset, I32 Origin) = 0;

		virtual I64 tell() = 0;

		virtual bool isOpen() = 0;

		// returns nonzero if we are at end of file/data
		virtual I32 eof() = 0;

		virtual I32 close() = 0;
	};
}

#endif // KINPUTSTREAM_H