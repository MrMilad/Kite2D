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

#include "Kite/Core/utility/kserialize.h"
#include "Kite/Core/system/ksystemutil.h"
#include "Kite/Core/utility/kmeminputstream.h"
#include <cstdio>

namespace Kite {
	/*KSerialize &operator<<(KSerialize &Out, const KSerialize &Value) {
		Out << Value._kpos << Value._kendfile << Value._kdata;
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, KSerialize &Value) {
		In >> Value._kpos >> Value._kendfile >> Value._kdata;
		return In;
	}*/

	KSerialize::KSerialize() :
		_kpos(0),
		_kendfile(true)
	{}

	bool KSerialize::loadFile(const std::string &FileName, U32 FileType) {
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

	bool KSerialize::loadStream(KIStream &Stream, U32 FileType) {
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

	bool KSerialize::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		KMemInputStream temp(Data, Size);

		return loadStream(temp);
	}

	bool KSerialize::saveFile(const std::string &FileName) {
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

	bool KSerialize::saveStream(KOStream &Stream) {
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

	void KSerialize::_convertAndSave(void *Value, U8 Size) {
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

	void KSerialize::_readAndConvert(void *Value, U8 Size) {
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

	KSerialize &operator<<(KSerialize &Out, I64 Value){
		Out._convertAndSave(&Value, 8);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, I32 Value) {
		Out._convertAndSave(&Value, 4);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, I16 Value) {
		Out._convertAndSave(&Value, 2);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, I8 Value) {
		Out._kdata.push_back(Value);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, U64 Value) {
		Out._convertAndSave(&Value, 8);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, U32 Value) {
		Out._convertAndSave(&Value, 4);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, U16 Value) {
		Out._convertAndSave(&Value, 2);
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, U8 Value) {
		Out._kdata.push_back(Value);
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, I64 &Value){
		In._readAndConvert(&Value, 8);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, I32 &Value) {
		In._readAndConvert(&Value, 4);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, I16 &Value) {
		In._readAndConvert(&Value, 2);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, I8 &Value) {
		Value = In._kdata[In._kpos];
		++In._kpos;
		return In;
	}
	
	KSerialize &operator>>(KSerialize &In, U64 &Value) {
		In._readAndConvert(&Value, 8);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, U32 &Value) {
		In._readAndConvert(&Value, 4);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, U16 &Value) {
		In._readAndConvert(&Value, 2);
		return In;
	}

	KSerialize &operator>>(KSerialize &In, U8 &Value) {
		Value = In._kdata[In._kpos];
		++In._kpos;
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, F64 Value) {
		Out._convertAndSave(&Value, 8);
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, F64 &Value) {
		In._readAndConvert(&Value, 8);
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, F32 Value) {
		Out._convertAndSave(&Value, 4);
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, F32 &Value) {
		In._readAndConvert(&Value, 4);
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, const std::string &Value) {
		Out << (U32)Value.size();
		Out._kdata.reserve(Out._kdata.size() + Value.size());
		Out._kdata.insert(Out._kdata.end(), &Value[0], &Value[Value.size()]);

		return Out;
	}

	KSerialize &operator>>(KSerialize &In, std::string &Value) {
		if (In.endOfFile() || In._kdata.empty()) {
			Value.clear();
			return In;
		}

		U32 size = 0;
		In >> size;
		Value.clear();
		Value.reserve(size);
		Value.insert(0, (const char *)&In._kdata[In._kpos], size);

		In._kpos += size;

		// check end of data
		if (In._kpos >= In._kdata.size())
			In._kendfile = true;
		
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, bool Value) {
		if (Value) {
			Out << (U8)1;
		} else {
			Out << (U8)0;
		}
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, bool &Value) {
		U8 temp;
		In >> temp;
		if (temp == 1) {
			Value = true;
		} else {
			Value = false;
		}

		return In;
	}

	void KSerialize::clear() {
		_kdata.clear();
		_kendfile = true;
		_kpos = 0;
	}
}