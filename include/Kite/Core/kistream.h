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
#ifndef KISTREAM_H
#define KISTREAM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include "kistream.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(ABSTRACT, ISTREAM)
	class KITE_FUNC_EXPORT KIStream{
		KMETA_KISTREAM_BODY();
	public:
		virtual ~KIStream(){}

		KM_FUN()
		virtual bool open(const std::string &Address, IOMode Type) = 0;

		virtual SIZE read(void *Data, SIZE DataSize) = 0;

		virtual I32 seek(U64 Offset, I32 Origin) = 0;

		virtual I64 tell() = 0;

		virtual I32 eof() = 0;

		KM_FUN()
		virtual U64 getSize() = 0;

		KM_FUN()
		virtual bool isOpen() = 0;

		KM_FUN()
		virtual I32 close() = 0;
	};
}

#endif // KISTREAM_H