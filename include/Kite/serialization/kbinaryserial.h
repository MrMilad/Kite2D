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
#include "Kite/utility/kistream.h"
#include "Kite/utility/kostream.h"
#include <string>
#include <vector>

namespace Kite {
	class KITE_FUNC_EXPORT KBinarySerial : public KBaseSerial{
	public:
		KBinarySerial(KBaseStorage &Allocator);

		bool loadFile(const std::string &FileName, U32 FileType = 0);

		bool loadStream(KIStream &Stream, U32 FileType = 0);

		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

		bool saveFile(const std::string &FileName);

		bool saveStream(KOStream &Stream);

		inline bool endOfFile() const { return _kendfile; }

		void clearCatch();

		void writePOD(const void *Value, SIZE Size, bool Str) final;

		void readPOD(void *Value, SIZE Size, bool Str) final;

	private:
		std::vector<U8> _kdata;
		U32 _kpos;
		bool _kendfile;
	};
}

#endif // KBINARYSERIAL_H