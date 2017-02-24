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
#ifndef KOSTREAM_H
#define KOSTREAM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include "ktypes.khgen.h"
#include "kostream.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(ABSTRACT, OSTREAM)
	class KITE_FUNC_EXPORT KOStream {
		KMETA_KOSTREAM_BODY();
	public:
		virtual ~KOStream();

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "drivedType", KP_TYPE = OStreamTypes, KP_CM = "type of the drived stream")
		virtual inline OStreamTypes getDrivedType() const = 0;

		/// will be implemented by KHParser
		/// usage: access base class in lua
		virtual KOStream *getBase() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string, KP_CM = "type name")
		virtual inline const std::string &getTypeName() const = 0;

		KM_FUN()
		virtual bool open(const std::string &Address, IOMode Type) = 0;

		virtual SIZE write(const void *Data, SIZE DataSize) const = 0;

		KM_FUN()
		virtual bool isOpen() const = 0;

		KM_FUN()
		virtual I32 close() = 0;

		KM_FUN()
		virtual const KFileInfo *getFileInfo() = 0;

		KM_FUN()
		virtual void getFileInfoStr(const std::string &Address, KFileInfo &FileInfo) = 0;


		KM_FUN()
		virtual IOMode getIOMode() const = 0;
	};
}

#endif // KOSTREAM_H