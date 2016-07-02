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
#ifndef KBINARYSERIAL_H
#define KBINARYSERIAL_H

#include "Kite/core/kcoredef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/meta/kmetadef.h"
#include <string>
#include <vector>
#include "kbinaryserial.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SERIALIZER)
	class KITE_FUNC_EXPORT KBinarySerial : public KBaseSerial{
		KMETA_KBINARYSERIAL_BODY();
	public:
		KM_CON()
		KBinarySerial();

		KM_FUN()
		bool loadStream(KIStream *Stream, const std::string &Address) final;

		KM_FUN()
		bool saveStream(KOStream *Stream, const std::string &Address, U32 Version) final;

		void writePOD(const void *Value, SIZE Size, bool Str) final;

		void readPOD(void *Value, SIZE Size, bool Str) final;

		KM_FUN()
		inline bool endOfFile() const { if (_kdata.size() > _kpos) return false;  return true; }

		KM_FUN()
		void clearCatch();

	private:
		std::vector<U8> _kdata;
		U32 _kpos;
		bool _kendfile;
	};
}

#endif // KBINARYSERIAL_H