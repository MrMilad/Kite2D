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

#include "Kite/Core/utility/KBytesArray.h"
#include "Kite/Core/system/ksystemutil.h"
#include "Kite/Core/utility/kmeminputstream.h"
#include <cstdio>

namespace Kite {
	KBytesArray::KBytesArray() :
		_kpos(0),
		_kendfile(true)
	{}

	bool KBytesArray::loadFile(const std::string &FileName, U32 FileType) {
		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "rb");

		if (file != NULL) {
			// read header
			char header[8];
			if (fread(&header, sizeof(header) - 1, 1, file) == 1) {
				header[7] = '\0';

				// check header
				if (strcmp("kserial\0", header) == 0) {

					// get file size
					fseek(file, 0, SEEK_END);
					U32 size = ftell(file);
					rewind(file);

					// read date
					_kdata.clear();
					_kdata.resize(size);
					_kpos = 0;
					if (fread(&_kdata[0], (size_t)size, 1, file) == 1) {
						_kdata.erase(_kdata.begin(), _kdata.begin() + 7);
						_kendfile = false;
						ret = true;
					} else {
						KDEBUG_PRINT("read file data error");
					}
				} else {
					KDEBUG_PRINT("wrong file format");
				}
			} else {
				KDEBUG_PRINT("read header error");
			}
		} else {
			KDEBUG_PRINT("open file error");
		}

		fclose(file);
		return ret;
	}

	bool KBytesArray::loadStream(KIStream &Stream, U32 FileType) {
		bool ret = false;

		if (Stream.isOpen()) {
			// read header
			char header[8];
			if (Stream.read(&header, 7) == 7) {
				header[7] = '\0';

				// check header
				if (strcmp("kserial\0", header) == 0) {

					// get file size
					U64 size = Stream.getSize();
					Stream.seek(0, SEEK_SET);

					// read date
					_kdata.clear();
					_kdata.resize((U32)size);
					_kpos = 0;
					if (Stream.read(&_kdata[0], size) == size) {
						_kdata.erase(_kdata.begin(), _kdata.begin() + 7);
						_kendfile = false;
						ret = true;
					} else {
						KDEBUG_PRINT("read file data error");
					}
				} else {
					KDEBUG_PRINT("wrong file format");
				}
			} else {
				KDEBUG_PRINT("read header error");
			}
		} else {
			KDEBUG_PRINT("open file error");
		}

		return ret;
	}

	bool KBytesArray::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		KMemInputStream temp(Data, Size);

		return loadStream(temp);
	}

	bool KBytesArray::saveFile(const std::string &FileName) {
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
				KDEBUG_PRINT("write data error")
			}
		} else {
			KDEBUG_PRINT("open file error");
		}

		// cleanup
		fclose(file);
		return ret;
	}

	bool KBytesArray::saveStream(KOStream &Stream) {
		bool ret = false;

		// open file
		if (Stream.isOpen()) {

			// inite header (30 bytes lenght)
			char format[] = { 'k', 's', 'e', 'r', 'i', 'a', 'l' };
			_kdata.insert(_kdata.begin(), &format[0], &format[7]);

			// write data
			if (Stream.write((void *)&_kdata[0], _kdata.size())) {
				ret = true;
			} else {
				KDEBUG_PRINT("write data error")
			}
		} else {
			KDEBUG_PRINT("open file error");
		}

		return ret;
	}

	void KBytesArray::_convertAndSave(void *Value, U8 Size) {
		// always we write data in little endian format
		U8 buf[8];
		U8 *fin;

		// convert to little endian
		if (isBigEndian()) {
			U8 *ptr = (U8 *)Value;
			for (U8 i = 0; i < Size; i++) {
				buf[i] = ptr[(Size - 1) - i];
			}
			fin = buf;
		} else {
			fin = (U8 *)Value;
		}

		for (U32 i = 0; i < Size; i++) {
			_kdata.push_back(fin[i]);
		}

		_kendfile = false;
	}

	void KBytesArray::_readAndConvert(void *Value, U8 Size) {
		if (endOfFile() || _kdata.empty()) {
			return;
		}

		memcpy(Value, &_kdata[_kpos], (size_t)Size);

		// convert to big endian
		if (isBigEndian()) {
			U8 *ptr = (U8 *)Value;
			for (U8 i = 0; i < Size; i++) {
				ptr[i] = _kdata[_kpos + ((Size - 1) - i)];
			}
		}

		// move read pointer
		_kpos += Size;

		// check end of data
		if (_kpos >= _kdata.size())
			_kendfile = true;
	}

	void KBytesArray::clear() {
		_kdata.clear();
		_kendfile = true;
		_kpos = 0;
	}
}