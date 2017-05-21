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
		KM_INFO(KI_NAME = "BinarySerial")
		friend KBaseSerial &operator<<(KBinarySerial &Out, const KBinarySerial &Value) {
			// write data size
			SIZE dsize = Value._kdata.size();
			Out.writePOD(&dsize, sizeof(SIZE), false);

			// write version
			U32 ver = Value.getVersion();
			Out.writePOD(&ver, sizeof(U32), false);

			// write data
			Out.append(&Value);

			return Out;
		}
		friend KBaseSerial &operator>>(KBinarySerial &In, KBinarySerial &Value) {
			Value.clearCatch();

			// read size
			SIZE dsize = 0;
			In.readPOD(&dsize, sizeof(SIZE), false);
			Value._kdata.resize(dsize);

			// read version
			U32 ver = 0;
			In.readPOD(&ver, sizeof(U32), false);
			Value.setVersion(ver);

			// read data 
			if (dsize > 0) {
				// pass true for str parameter because we dont need binary calculatin
				In.readPOD((void*)&Value._kdata[0], dsize, true);
			}

			return In;
		}
		KBINARYSERIAL_BODY();
	public:
		KM_CON()
		KBinarySerial();

		KM_FUN()
		bool loadStream(KIOStream &Stream, const std::string &Address) final;

		KM_FUN()
		bool saveStream(KIOStream &Stream, const std::string &Address, U32 Version) final;

		void writePOD(const void *Value, SIZE Size, bool Str) final;

		void readPOD(void *Value, SIZE Size, bool Str) final;

		KM_FUN()
		inline bool endOfFile() const { if (_kdata.size() > _kpos) return false;  return true; }

		KM_FUN()
		void clearCatch();

		KM_FUN()
		void append(const KBinarySerial *Object);

		KM_FUN()
		void setReadPos(U32 Pos);

		KM_FUN()
		U32 getReadPos() const { return _kpos; }

	private:
		std::vector<U8> _kdata;
		U32 _kpos;
		bool _kendfile;
	};
}

#endif // KBINARYSERIAL_H