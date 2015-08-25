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
#ifndef KSERIALIZE_H
#define KSERIALIZE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kistream.h"
#include "Kite/Core/utility/kostream.h"
#include <string>
#include <utility>

namespace Kite {
	class KITE_FUNC_EXPORT KSerialize{
		// integers
		friend KSerialize &operator<<(KSerialize &Out, IL32 Value);
		friend KSerialize &operator<<(KSerialize &Out, I32 Value);
		friend KSerialize &operator<<(KSerialize &Out, I16 Value);
		friend KSerialize &operator<<(KSerialize &Out, I8 Value);

		friend KSerialize &operator>>(KSerialize &In, IL32 &Value);
		friend KSerialize &operator>>(KSerialize &In, I32 &Value);
		friend KSerialize &operator>>(KSerialize &In, I16 &Value);
		friend KSerialize &operator>>(KSerialize &In, I8 &Value);

		// unsighned integers
		friend KSerialize &operator<<(KSerialize &Out, UL32 Value);
		friend KSerialize &operator<<(KSerialize &Out, U32 Value);
		friend KSerialize &operator<<(KSerialize &Out, U16 Value);
		friend KSerialize &operator<<(KSerialize &Out, U8 Value);

		friend KSerialize &operator>>(KSerialize &In, UL32 &Value);
		friend KSerialize &operator>>(KSerialize &In, U32 &Value);
		friend KSerialize &operator>>(KSerialize &In, U16 &Value);
		friend KSerialize &operator>>(KSerialize &In, U8 &Value);
		
		// floates
		friend KSerialize &operator<<(KSerialize &Out, F64 Value);
		friend KSerialize &operator>>(KSerialize &In, F64 &Value);

		friend KSerialize &operator<<(KSerialize &Out, F32 Value);
		friend KSerialize &operator>>(KSerialize &In, F32 &Value);

		// bool
		friend KSerialize &operator<<(KSerialize &Out, bool Value);
		friend KSerialize &operator>>(KSerialize &In, bool &Value);

		// char
		friend KSerialize &operator<<(KSerialize &Out, const char *Value);
		friend KSerialize &operator>>(KSerialize &In, char *Value);

		// string
		friend KSerialize &operator<<(KSerialize &Out, const std::string &Value);
		friend KSerialize &operator>>(KSerialize &In, std::string &Value);

		// KSerialize itself!
		friend KSerialize &operator<<(KSerialize &Out, const KSerialize &Value);
		friend KSerialize &operator>>(KSerialize &In, KSerialize &Value);

		//std::pair
		template <typename X, typename Y>
		friend KSerialize &operator<<(KSerialize &Out, const std::pair<Y, X> &Value) {
			Out << Value.first << Value.second;
			return Out;
		}

		template <typename X, typename Y>
		friend KSerialize &operator>>(KSerialize &In, std::pair<Y, X> &Value) {
			In >> Value.first >> Value.second;
			return In;
		}
	public:
		KSerialize();
		bool loadFile(const std::string &FileName, U32 FileType = 0);

		bool loadStream(KIStream &Stream, U32 FileType = 0);

		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

		bool saveFile(const std::string &FileName);

		bool saveStream(KOStream &Stream);

		inline bool endOfFile() const { return _kendfile; }

		void clear();

	private:
		std::string _kdata;
		U32 _kpos;
		bool _kendfile;
	};
}

#endif // KSERIALIZE_H