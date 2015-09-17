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
#ifndef KBYTESARRAY_H
#define KBYTESARRAY_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kistream.h"
#include "Kite/Core/utility/kostream.h"
#include <string>
#include <vector>
#include <utility>

namespace Kite {
	class KITE_FUNC_EXPORT KBytesArray{
		// use case: only primitive types (U8, I8, U32, F32, ...)
		// non-pointers
		template <typename T>
		friend KBytesArray &operator<<(KBytesArray &Out, T Value) {
			Out._convertAndSave((void *)&Value, sizeof(T));
			return Out;
		}

		template <typename T>
		friend KBytesArray &operator>>(KBytesArray &In, T &Value) {
			In._readAndConvert((void *)&Value, sizeof(T));
			return In;
		}

		// void pointers
		friend KBytesArray &operator<<(KBytesArray &Out, const KVoidStream &Value) {
			Out._convertAndSave(Value.pointer, Value.size);
			return Out;
		}

		friend KBytesArray &operator>>(KBytesArray &In, KVoidStream &Value) {
			In._readAndConvert(Value.pointer, Value.size);
			return In;
		}

		// bool
		friend KBytesArray &operator<<(KBytesArray &Out, bool Value);
		friend KBytesArray &operator>>(KBytesArray &In, bool &Value);

		// string
		friend KBytesArray &operator<<(KBytesArray &Out, const std::string &Value);
		friend KBytesArray &operator>>(KBytesArray &In, std::string &Value);

		// KBytesArray itself!
		/*friend KBytesArray &operator<<(KBytesArray &Out, const KBytesArray &Value);
		friend KBytesArray &operator>>(KBytesArray &In, KBytesArray &Value);*/

		//std::pair
		template <typename X, typename Y>
		friend KBytesArray &operator<<(KBytesArray &Out, const std::pair<Y, X> &Value) {
			Out << Value.first << Value.second;
			return Out;
		}

		template <typename X, typename Y>
		friend KBytesArray &operator>>(KBytesArray &In, std::pair<Y, X> &Value) {
			In >> Value.first >> Value.second;
			return In;
		}
	public:
		KBytesArray();
		bool loadFile(const std::string &FileName, U32 FileType = 0);

		bool loadStream(KIStream &Stream, U32 FileType = 0);

		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

		bool saveFile(const std::string &FileName);

		bool saveStream(KOStream &Stream);

		inline bool endOfFile() const { return _kendfile; }

		void clear();

	private:
		void _convertAndSave(void *Value, U8 Size);
		void _readAndConvert(void *Value, U8 Size);
		std::vector<U8> _kdata;
		U32 _kpos;
		bool _kendfile;
	};
}

#endif // KBYTESARRAY_H