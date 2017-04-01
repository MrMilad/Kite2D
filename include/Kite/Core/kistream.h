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
#include "ktypes.khgen.h"
#include "kistream.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(ABSTRACT, ISTREAM)
	class KITE_FUNC_EXPORT KIStream{
		KM_INFO(KI_NAME = "InStreamBase");
		KISTREAM_BODY();
	public:
		virtual ~KIStream();

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "drivedType", KP_TYPE = InStream, KP_CM = "type of the drived stream")
			virtual inline InStream getDrivedType() const = 0;

		/// will be implemented by KHParser
		/// usage: access base class in lua
		//virtual KIStream *getBase() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string, KP_CM = "type name")
			virtual inline const std::string &getTypeName() const = 0;

		KM_FUN()
		virtual bool open(const std::string &Address, IOMode Type) = 0;

		virtual SIZE read(void *Data, SIZE DataSize) = 0;

		virtual I32 seek(U64 Offset, I32 Origin) = 0;

		virtual I64 tell() const = 0;

		virtual I32 eof() const = 0;

		KM_FUN()
		virtual U64 getSize() const = 0;

		KM_FUN()
		virtual bool isOpen() const = 0;

		KM_FUN()
		virtual I32 close() = 0;

		KM_FUN()
		virtual const KFileInfo *getFileInfo() const = 0;

		KM_FUN()
		virtual void getFileInfoStr(const std::string &Address, KFileInfo &FileInfo) const = 0;

		KM_FUN()
		virtual IOMode getIOMode() const = 0;

		/// will be implemented by KHParser
		virtual KIStream *clone() const = 0;
	};
}

#endif // KISTREAM_H