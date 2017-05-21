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
#include "Kite/stream/kfilestream.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>
#include <cstdio>

namespace Kite{

	KFileStream::KFileStream() :
		_kfile(nullptr),
		_kisOpen(false)
	{}

	KFileStream::~KFileStream() {
		if (_kisOpen) {
			fclose(_kfile);
		}
	}

	bool KFileStream::openRead(const std::string &Address, IOMode IOType) {
		_kio = IOType;
		if (Address.empty()) {
			KD_PRINT("address is empty");
			return false;
		}

		// close file if its opened
		if (_kisOpen) {
			fclose(_kfile);
			_kisOpen = false;
		}

		std::string mode;
		if (IOType == IOMode::BIN) {
			mode = "rb";
		} else {
			mode = "r";
		}

		_kfile = fopen(Address.c_str(), mode.c_str());
		if (_kfile == NULL) {
			KD_FPRINT("can't open file. fname: %s", Address.c_str());
			return false;
		}

		// file information
		_getFileInfoStr(Address, _kfinfo);

		_kisOpen = true;
		return true;
	}

	bool KFileStream::openWrite(const std::string &Address, IOMode IOType, WriteMode WriteType) {
		_kio = IOType;
		if (Address.empty()) {
			KD_PRINT("address is empty");
			return false;
		}

		// close file if its opened
		if (_kisOpen) {
			fclose(_kfile);
			_kisOpen = false;
		}

		std::string mode;
		if (WriteType == WriteMode::APPEND) {
			mode = "a";
		} else {
			mode = "w";
		}
		if (IOType == IOMode::BIN) {
			mode += "b";
		}

		_kfile = fopen(Address.c_str(), mode.c_str());
		if (_kfile == NULL) {
			KD_FPRINT("can't open file. fname: %s", Address.c_str());
			return false;
		}

		// file information
		_getFileInfoStr(Address, _kfinfo);
		_kisOpen = true;
		return true;
	}

	SIZE KFileStream::read(void *Data, SIZE DataSize) {
		if (_kisOpen) {
			return fread(Data, 1, (size_t)DataSize, _kfile);
		}
		KD_PRINT("stream is not open");
		return 0;
	}

	SIZE KFileStream::write(const void *Data, SIZE DataSize) {
		if (_kisOpen) {
			return fwrite(Data, 1, (size_t)DataSize, _kfile);
		}
		KD_PRINT("stream is not open");
		return 0;
	}

	I32 KFileStream::seek(U64 Offset, I32 Origin) {
		if (_kisOpen) {
			return fseek(_kfile, (long)Offset, Origin);
		}
		KD_PRINT("stream is not open");
		return -1;
	}

	I64 KFileStream::tell() const {
		if (_kisOpen) {
			return ftell(_kfile);
		}
		KD_PRINT("stream is not open");
		return -1;
	}

	U64 KFileStream::getSize() const {
		if (_kisOpen){
			long cur = ftell(_kfile);
			fseek(_kfile, 0, SEEK_END);
			U64 size = ftell(_kfile);
			fseek(_kfile, cur, SEEK_SET);
			return size;
		}
		KD_PRINT("stream is not open");
		return 0;
	}

	I32 KFileStream::eof() const {
		if (_kisOpen) {
			return feof(_kfile);
		}
		KD_PRINT("stream is not open");
		return 0;
	}

	I32 KFileStream::close() {
		if (_kisOpen) {
			_kisOpen = false;
			return fclose(_kfile);
		}
		return 0;
	}

	void KFileStream::_getFileInfoStr(const std::string &Address, KFileInfo &FInfo) const {
		// extract address info
		SIZE pos = Address.find_last_of("/\\");
		FInfo.fullPath = Address;
		if (pos != std::string::npos) {
			FInfo.path = Address.substr(0, pos);
			FInfo.name = Address.substr(pos + 1);

		} else { // if we not found '/' or '\\'
			FInfo.path.clear();
			FInfo.name = Address;
		}
	}
	KFILESTREAM_SOURCE();
}