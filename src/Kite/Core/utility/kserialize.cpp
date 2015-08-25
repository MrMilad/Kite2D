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
#include <cstdlib>

namespace Kite {
	KSerialize &operator<<(KSerialize &Out, const KSerialize &Value) {
		Out << Value._kpos << Value._kendfile << Value._kdata;
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, KSerialize &Value) {
		In >> Value._kpos >> Value._kendfile >> Value._kdata;
		return In;
	}

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
			char header[9];
			if (fread(&header, sizeof(header), 1, file) == 1) {
				header[8] = '\0';

				// check header
				if (strcmp("kserial\x1f\0", header) == 0) {

					// get file size
					fseek(file, 0, SEEK_END);
					U32 size = ftell(file);
					rewind(file);

					// read date
					_kdata.clear();
					_kdata.resize(size);
					_kpos = 0;
					if (fread(&_kdata[0], (size_t)size, 1, file) == 1) {
						_kdata.erase(0, 8);
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
			char header[9];
			if (Stream.read(&header, sizeof(header)) == sizeof(header)) {
				header[8] = '\0';

				// check header
				if (strcmp("kserial\x1f\0", header) == 0) {

					// get file size
					U64 size = Stream.getSize();
					Stream.seek(0, SEEK_SET);

					// read date
					_kdata.clear();
					_kdata.resize((U32)size);
					_kpos = 0;
					if (Stream.read(&_kdata[0], size) == size) {
						_kdata.erase(0, 8);
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
			_kdata.insert(0, "kserial\x1f");
			// write data
			if (fwrite(_kdata.c_str(), _kdata.size(), 1, file) == 1) {
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
			_kdata.insert(0, "kserial\x1f");

			// write data
			if (Stream.write((void *)_kdata.c_str(), _kdata.size())) {
				ret = true;
			} else {
				KDEBUG_PRINT("write data error")
			}
		} else {
			KDEBUG_PRINT("open file error");
		}

		return ret;
	}

	KSerialize &operator<<(KSerialize &Out, IL32 Value){
		char buf[50];
		char tok[2] = { '\x1f', '\0' };
		Out._kdata.append(_ltoa(Value, buf, 10));
		Out._kdata.append(tok);
		Out._kendfile = false;
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, I32 Value) {
		return Out << (IL32)Value;
	}

	KSerialize &operator<<(KSerialize &Out, I16 Value) {
		return Out << (IL32)Value;
	}

	KSerialize &operator<<(KSerialize &Out, I8 Value) {
		return Out << (IL32)Value;
	}

	KSerialize &operator<<(KSerialize &Out, UL32 Value) {
		char buf[50];
		char tok[2] = { '\x1f', '\0' };
		sprintf(buf, "%lu", Value);
		Out._kdata.append(buf);
		Out._kdata.append(tok);
		Out._kendfile = false;
		return Out;
	}

	KSerialize &operator<<(KSerialize &Out, U32 Value) {
		return Out << (UL32)Value;
	}

	KSerialize &operator<<(KSerialize &Out, U16 Value) {
		return Out << (UL32)Value;
	}

	KSerialize &operator<<(KSerialize &Out, U8 Value) {
		return Out << (UL32)Value;
	}

	KSerialize &operator>>(KSerialize &In, IL32 &Value){
		if (In.endOfFile() || In._kdata.empty()) {
			Value = 0;
			return In;
		}
		
		// read value
		Value = atoi(&In._kdata[In._kpos]);

		// move read pointer
		char tok[2] = { '\x1f', '\0' };
		size_t tpos = In._kdata.find(tok, In._kpos);
		if (tpos == std::string::npos) {
			In._kpos = In._kdata.size();
			In._kendfile = true;
		} else {
			In._kpos = tpos;

			// skip token chracter
			++In._kpos;
		}

		if (In._kpos >= In._kdata.size())
			In._kendfile = true;

		return In;
	}

	KSerialize &operator>>(KSerialize &In, I32 &Value) {
		IL32 temp;
		In >> temp;
		Value = (I32)temp;
		return In;
	}

	KSerialize &operator>>(KSerialize &In, I16 &Value) {
		IL32 temp;
		In >> temp;
		Value = (I16)temp;
		return In;
	}

	KSerialize &operator>>(KSerialize &In, I8 &Value) {
		IL32 temp;
		In >> temp;
		Value = (I8)temp;
		return In;
	}
	
	KSerialize &operator>>(KSerialize &In, UL32 &Value) {
		if (In.endOfFile() || In._kdata.empty()) {
			Value = 0;
			return In;
		}

		// read value
		Value = strtoul(&In._kdata[In._kpos], 0, 0);

		// move read pointer
		char tok[2] = { '\x1f', '\0' };
		size_t tpos = In._kdata.find(tok, In._kpos);
		if (tpos == std::string::npos) {
			In._kpos = In._kdata.size();
			In._kendfile = true;
		} else {
			In._kpos = tpos;

			// skip token chracter
			++In._kpos;
		}

		if (In._kpos >= In._kdata.size())
			In._kendfile = true;

		return In;
	}

	KSerialize &operator>>(KSerialize &In, U32 &Value) {
		UL32 temp;
		In >> temp;
		Value = (U32)temp;
		return In;
	}

	KSerialize &operator>>(KSerialize &In, U16 &Value) {
		UL32 temp;
		In >> temp;
		Value = (U16)temp;
		return In;
	}

	KSerialize &operator>>(KSerialize &In, U8 &Value) {
		UL32 temp;
		In >> temp;
		Value = (U8)temp;
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, F64 Value) {
		char tok[2] = { '\x1f', '\0' };
		Out._kdata.append(std::to_string(Value));
		Out._kdata.append(tok);
		Out._kendfile = false;
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, F64 &Value) {
		if (In.endOfFile() || In._kdata.empty()) {
			Value = 0;
			return In;
		}

		// read value
		Value = convertStrToDouble(&In._kdata[In._kpos]);

		// move read pointer
		char tok[2] = { '\x1f', '\0' };
		size_t tpos = In._kdata.find(tok, In._kpos);
		if (tpos == std::string::npos) {
			In._kpos = In._kdata.size();
			In._kendfile = true;
		} else {
			In._kpos = tpos;

			// skip token chracter
			++In._kpos;
		}

		if (In._kpos >= In._kdata.size())
			In._kendfile = true;

		return In;
	}

	KSerialize &operator<<(KSerialize &Out, F32 Value) {
		return Out << (F64)Value;
	}

	KSerialize &operator>>(KSerialize &In, F32 &Value) {
		F64 temp;
		In >> temp;
		Value = (F32)temp;
		return In;
	}

	KSerialize &operator<<(KSerialize &Out, const std::string &Value) {
		Out << Value.c_str();
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, std::string &Value) {
		if (In.endOfFile() || In._kdata.empty()) {
			Value.clear();
			return In;
		}

		// string lenght
		char tok[2] = { '\x1f', '\0' };
		size_t tpos = In._kdata.find(tok, In._kpos);
		if (tpos == std::string::npos) {
			In._kpos = In._kdata.size();
			In._kendfile = true;
			Value.clear();
		} else {

			Value.assign(&In._kdata[In._kpos], (tpos - In._kpos));

			In._kpos = tpos;

			// skip token chracter
			++In._kpos;
		}

		if (In._kpos >= In._kdata.size())
			In._kendfile = true;

		return In;
	}

	KSerialize &operator<<(KSerialize &Out, const char *Value) {
		char tok[2] = { '\x1f', '\0' };
		Out._kdata.append(Value);
		Out._kdata.append(tok);
		Out._kendfile = false;
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, char *Value) {
		if (In.endOfFile() || In._kdata.empty()) {
			Value = 0;
			return In;
		}

		//  string lenght
		char tok[2] = { '\x1f', '\0' };
		size_t tpos = In._kdata.find(tok, In._kpos);
		if (tpos == std::string::npos) {
			In._kpos = In._kdata.size();
			In._kendfile = true;
			Value = 0;
		} else {

			strncpy(Value, &In._kdata[In._kpos], (tpos - In._kpos));
			Value[(tpos - In._kpos)] = '\0';

			In._kpos = tpos;

			// skip token chracter
			++In._kpos;
		}

		if (In._kpos >= In._kdata.size())
			In._kendfile = true;

		return In;
	}

	KSerialize &operator<<(KSerialize &Out, bool Value) {
		char tok[2] = { '\x1f', '\0' };
		if (Value) {
			Out << (const char *)"1";
		} else {
			Out << (const char *)"0";
		}
		return Out;
	}

	KSerialize &operator>>(KSerialize &In, bool &Value) {
		char temp[2];
		In >> temp;
		if (temp[0] == '1') {
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