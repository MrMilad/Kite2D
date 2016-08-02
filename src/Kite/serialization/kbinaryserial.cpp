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

#include "Kite/serialization/kbinaryserial.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include <luaintf\LuaIntf.h>
#include <cstdio>
#include "src/Kite/serialization/tp/blowfish.h"

namespace Kite {
	KBinarySerial::KBinarySerial() :
		_kpos(0),
		_kendfile(true) {}

	bool KBinarySerial::loadStream(KIStream *Stream, const std::string &Address) {
		_kpos = 0;
		_kdata.clear();
		Stream->close();

		// open stream
		if (!Stream->open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. saddress: %s", Address.c_str());
			return false;
		}

		// read header
		char format[8];
		if (!Stream->read((void *)&format[0], sizeof(format))) {
			KD_PRINT("can't read data from stream.");
			Stream->close();
			return false;
		}

		if (std::strcmp(format, "kserial") != 0) {
			KD_PRINT("incorrect file format.");
			Stream->close();
			return false;
		}

		// read data
		const SIZE headerSize = sizeof(format);
		_kdata.resize(Stream->getSize() - headerSize);
		if (!Stream->read((void *)&_kdata[0], _kdata.size())) {
			KD_PRINT("can't read data from stream");
			Stream->close();
			return false;
		}

		// read version
		U32 ver;
		readPOD(&ver, sizeof(U32), false);
		setVersion(ver);

		Stream->close();
		return true;
	}

	bool KBinarySerial::saveStream(KOStream *Stream, const std::string &Address, U32 Version) {
		Stream->close();

		// open stream
		if (!Stream->open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. saddress: %s", Address.c_str());
			return false;
		}

		// write header
		// format
		char format[] = "kserial";
		if (!Stream->write((void *)&format[0], sizeof(format))) {
			KD_PRINT("can't write data to stream");
			Stream->close();
			return false;
		}

		// version
		U8 buf[8];
		const U8 *fin = (const U8 *)&Version;

		// convert to little endian
		if (isBigEndian()) {
			U8 *ptr = (U8 *)&Version;
			for (U8 i = 0; i < sizeof(U32); i++) {
				buf[i] = ptr[(sizeof(U32) - 1) - i];
			}
			fin = buf;
		}
		if (!Stream->write((const void *)&fin[0], sizeof(U32))) {
			KD_PRINT("can't write data to stream");
			Stream->close();
			return false;
		}
		setVersion(Version);

		// write data
		if (!Stream->write((void *)&_kdata[0], _kdata.size())) {
			KD_PRINT("can't write data to stream");
			Stream->close();
			return false;
		}

		Stream->close();
		return true;
	}

	void KBinarySerial::writePOD(const void *Value, SIZE Size, bool Str) {
		if (Size <= 0) {
			return;
		}
		if (Str) {
			_kdata.reserve(_kdata.size() + Size);
			const char *chPtr = (const char *)Value;
			_kdata.insert(_kdata.end(), &chPtr[0], &chPtr[Size]);

		} else {

			// always we write data in little endian format
			// 32 bit
			U8 buf[8];
			const U8 *fin;

			// convert to little endian
			if (isBigEndian()) {
				U8 *ptr = (U8 *)Value;
				for (U8 i = 0; i < Size; i++) {
					buf[i] = ptr[(Size - 1) - i];
				}
				fin = buf;
			} else {
				fin = (const U8 *)Value;
			}

			_kdata.reserve(_kdata.size() + Size);
			_kdata.insert(_kdata.end(), &fin[0], &fin[Size]);
		}

		_kendfile = false;
	}

	void KBinarySerial::readPOD(void *Value, SIZE Size, bool Str) {
		if (endOfFile() || _kdata.empty() || Size <= 0) {
			return;
		}

		memcpy(Value, &_kdata[_kpos], Size);

		if (!Str) {
			// convert to big endian
			if (isBigEndian()) {
				U8 *ptr = (U8 *)Value;
				for (U8 i = 0; i < Size; i++) {
					ptr[i] = _kdata[_kpos + ((Size - 1) - i)];
				}
			}
		}

		// move read pointer
		_kpos += Size;

		// check end of data
		if (_kpos >= _kdata.size())
			_kendfile = true;
	}

	void KBinarySerial::clearCatch() {
		_kdata.clear();
		_kendfile = true;
		_kpos = 0;
	}

	void KBinarySerial::append(const KBinarySerial *Object) {
		_kdata.reserve(_kdata.size() + Object->_kdata.size());
		_kdata.insert(_kdata.end(), Object->_kdata.cbegin(), Object->_kdata.cend());
	}

	void KBinarySerial::setReadPos(U32 Pos){
		if (Pos < _kdata.size() || Pos == 0) {
			_kpos = Pos;
		} else {
			_kpos = _kdata.size();
			_kendfile = true;
			KD_PRINT("new pos is out of range and reached end of file.");
		}
	}

	KMETA_KBINARYSERIAL_SOURCE();
}