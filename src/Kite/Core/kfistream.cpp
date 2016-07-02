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
#include "Kite/core/kfistream.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>
#include <cstdio>

namespace Kite{

	KFIStream::KFIStream() :
		_kfile(nullptr) {}

	KFIStream::~KFIStream() {
		if (_kfile) {
			fclose(_kfile);
		}
	}

	bool KFIStream::open(const std::string &Address, IOMode Type) {
		_kfullpath = Address;
		_kio = Type;
		if (Address.empty()) {
			KD_PRINT("address is empty");
			return false;
		}

		// close file if its opened
		if (_kfile != nullptr) {
			fclose(_kfile);
			_kfile = nullptr;
		}

		std::string mode;
		if (Type == IOMode::BIN) {
			mode = "rb";
		} else {
			mode = "r";
		}

		_kfile = fopen(Address.c_str(), mode.c_str());
		if (_kfile == NULL) {
			KD_FPRINT("can't open file. fname: %s", Address.c_str());
			return false;
		}

		// extract address info
		SIZE pos = _kfullpath.find_last_of("/\\");
		if (pos != std::string::npos) {
			_kpath = _kfullpath.substr(0, pos);
			_kfname = _kfullpath.substr(pos + 1);

		} else { // if we not found '/' or '\\'
			_kpath.clear();
			_kfname = _kfullpath;
		}

		return true;
	}

	SIZE KFIStream::read(void *Data, SIZE DataSize) {
		if (_kfile != nullptr) {
			return fread(Data, 1, (size_t)DataSize, _kfile);
		}
		return 0;
	}

	I32 KFIStream::seek(U64 Offset, I32 Origin) {
		if (_kfile != nullptr) {
			return fseek(_kfile, (long)Offset, Origin);
		}
		return -1;
	}

	I64 KFIStream::tell() {
		if (_kfile != nullptr) {
			return ftell(_kfile);
		}
		return -1;
	}

	bool KFIStream::isOpen() {
		if (_kfile != nullptr) {
			return true;
		}
		return false;
	}

	U64 KFIStream::getSize() {
		if (_kfile){
			long cur = ftell(_kfile);
			fseek(_kfile, 0, SEEK_END);
			U64 size = ftell(_kfile);
			fseek(_kfile, cur, SEEK_SET);
			return size;
		}
		return 0;
	}

	I32 KFIStream::eof() {
		if (_kfile != nullptr) {
			return feof(_kfile);
		}
		return 0;
	}

	I32 KFIStream::close() {
		if (_kfile != nullptr) {
			return fclose(_kfile);
			_kfile = nullptr;
		}
		return 0;
	}
	KMETA_KFISTREAM_SOURCE();
}