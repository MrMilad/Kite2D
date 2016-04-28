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
#include <cstdio>
#include "src/Kite/serialization/tp/blowfish.h"

namespace Kite {
	KBinarySerial::KBinarySerial() :
		_kpos(0),
		_kendfile(true) {}

	bool KBinarySerial::loadStream(KIStream *Stream, const std::string &Key) {
		_kpos = 0;
		_kdata.clear();
		// open file
		if (!Stream->isOpen()) {
			KD_PRINT("stream is closed");
			return false;
		}

		// read header
		char format[8];
		if (!Stream->read((void *)&format[0], sizeof(format))) {
			KD_PRINT("can't read data from stream.");
			return false;
		}

		if (std::strcmp(format, "kserial") != 0) {
			KD_PRINT("incorrect file format.");
			return false;
		}

		// read marker
		char marker[4];
		if (!Stream->read((void *)&marker[0], sizeof(marker))) {
			KD_PRINT("can't read data from stream.");
			return false;
		}

		if (std::strcmp(marker, "enc") != 0 && std::strcmp(marker, "nor") != 0) {
			KD_PRINT("incorrect file format.");
			return false;
		}

		// decript data
		// key must be a multiple of 8 bytes (up to a maximum of 56)
		if (!Key.empty()) {
			if (Key.size() > 56 || ((Key.size() % 8) != 0)) {
				KD_PRINT("key must be a multiple of 8 bytes and up to a maximum of 56.");
				return false;
			}

			if (std::strcmp(marker, "enc") != 0) {
				KD_PRINT("this stream is not encripted!");
				return false;
			}

			// read encripted data
			_kdata.resize(Stream->getSize() - (sizeof(format) + sizeof(marker)));
			if (!Stream->read((void *)&_kdata[0], _kdata.size())) {
				KD_PRINT("can't read data from stream");
				return false;
			}
			// create key
			std::vector<U8> key(Key.begin(), Key.end());
			Blowfish encript(key);

			// decript data
			encript.Decrypt(_kdata);

			// check signature
			char sig[8];
			U8 counter = 0;
			if (_kdata.size() < sizeof(sig)) {
				_kdata.clear();
				KD_PRINT("incorrect key.");
				return false;
			}

			for (SIZE i = _kdata.size() - sizeof(sig); i < _kdata.size(); ++i) {
				sig[counter] = _kdata[i];
				++counter;
			}

			if (std::strcmp(sig, "kserial") != 0) {
				_kdata.clear();
				KD_PRINT("incorrect key.");
				return false;
			}

			// remove signature
			for (SIZE i = 0; i < sizeof(sig); ++i) {
				_kdata.pop_back();
			}

			return true;
		}

		if (std::strcmp(marker, "nor") != 0) {
			KD_PRINT("this stream is encripted!");
			return false;
		}

		// read data
		_kdata.resize(Stream->getSize() - sizeof(format));
		if (!Stream->read((void *)&_kdata[0], _kdata.size())) {
			KD_PRINT("can't read data from stream");
			return false;
		}

		return true;
	}

	bool KBinarySerial::saveStream(KOStream *Stream, const std::string &Key) {
		// open file
		if (!Stream->isOpen()) {
			KD_PRINT("stream is closed");
			return false;
		}

		// write header
		char format[] = "kserial";
		if (!Stream->write((void *)&format[0], sizeof(format))) {
			KD_PRINT("can't write data to stream");
			return false;
		}

		// encript data
		// key must be a multiple of 8 bytes (up to a maximum of 56)
		if (!Key.empty()){
			if (Key.size() > 56 || ((Key.size() % 8) != 0)) {
				KD_PRINT("key must be a multiple of 8 bytes and up to a maximum of 56.");
				return false;
			}

			// write encription marker
			char marker[] = "enc";
			if (!Stream->write((void *)&marker[0], sizeof(marker))) {
				KD_PRINT("can't write data to stream");
				return false;
			}

			// encript data
			std::vector<U8> encdata = _kdata;
			std::vector<U8> key(Key.begin(), Key.end());
			Blowfish encript(key);

			// add signature
			encdata.push_back('k');
			encdata.push_back('s');
			encdata.push_back('e');
			encdata.push_back('r');
			encdata.push_back('i');
			encdata.push_back('a');
			encdata.push_back('l');
			encdata.push_back('\0');

			encript.Encrypt(encdata);

			// write encripted data
			if (Stream->write((void *)&encdata[0], encdata.size())) {
				return true;
			} else {
				KD_PRINT("can't write data to stream");
				return false;
			}
		}

		// write normal marker
		char marker[] = "nor";
		if (!Stream->write((void *)&marker[0], sizeof(marker))) {
			KD_PRINT("can't write data to stream");
			return false;
		}

		// write data
		if (!Stream->write((void *)&_kdata[0], _kdata.size())) {
			KD_PRINT("can't write data to stream");
			return false;
		}

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
}