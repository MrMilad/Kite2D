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

namespace Kite {
	KBinarySerial::KBinarySerial() :
		_kpos(0),
		_kendfile(true) {}

	bool KBinarySerial::loadStream(KIStream *Stream, U32 FileType) {
		bool ret = false;

		if (Stream->isOpen()) {
			// read header
			char header[8];
			if (Stream->read(&header, 7) == 7) {
				header[7] = '\0';

				// check header
				if (strcmp("kserial\0", header) == 0) {

					// get file size
					U64 size = Stream->getSize();
					Stream->seek(0, SEEK_SET);

					// read date
					_kdata.clear();
					_kdata.resize((U32)size);
					_kpos = 0;
					if (Stream->read(&_kdata[0], size) == size) {
						_kdata.erase(_kdata.begin(), _kdata.begin() + 7);
						_kendfile = false;
						ret = true;
					} else {
						KD_PRINT("can't read stream");
					}
				} else {
					KD_PRINT("wrong data format");
				}
			} else {
				KD_PRINT("cant read header");
			}
		} else {
			KD_PRINT("can't open stream");
		}

		return ret;
	}

	bool KBinarySerial::saveFile(const std::string &FileName) {
		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "wb");
		if (file != NULL) {

			// inite header 
			char format[] = { 'k', 's', 'e', 'r', 'i', 'a', 'l' };
			_kdata.insert(_kdata.begin(), &format[0], &format[7]);
			// write data
			if (fwrite(&_kdata[0], _kdata.size(), 1, file) == 1) {
				ret = true;
			} else {
				KD_FPRINT("write data error. fname: %s", FileName.c_str());
			}
		} else {
			KD_FPRINT("can't open file. fname: %s", FileName.c_str());
		}

		// cleanup
		fclose(file);
		return ret;
	}

	bool KBinarySerial::saveStream(KOStream *Stream) {
		bool ret = false;

		// open file
		if (Stream->isOpen()) {

			// inite header (30 bytes lenght)
			char format[] = { 'k', 's', 'e', 'r', 'i', 'a', 'l' };
			_kdata.insert(_kdata.begin(), &format[0], &format[7]);

			// write data
			if (Stream->write((void *)&_kdata[0], _kdata.size())) {
				ret = true;
			} else {
				KD_PRINT("can't write data to stream");
			}
		} else {
			KD_PRINT("can't open stream");
		}

		return ret;
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