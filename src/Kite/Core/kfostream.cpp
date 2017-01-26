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
#include "Kite/core/kfostream.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf\LuaIntf.h>
#include <cstdio>

namespace Kite {
	KFOStream::KFOStream() :
		_kfile(nullptr) {}

	KFOStream::~KFOStream() {
		if (_kfile) {
			fclose(_kfile);
		}
	}

	bool KFOStream::open(const std::string &Address, IOMode Type) {
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
			mode = "wb";
		} else {
			mode = "w";
		}

		_kfile = fopen(Address.c_str(), mode.c_str());
		if (_kfile == NULL) {
			KD_FPRINT("can't open file. fname: %s", Address.c_str());
			return false;
		}

		// file information
		getFileInfoStr(Address, _kfinfo);

		return true;
	}

	SIZE KFOStream::write(const void *Data, SIZE DataSize) const {
		if (_kfile != nullptr) {
			return fwrite(Data, 1, (size_t)DataSize, _kfile);
		}
		return 0;
	}

	bool KFOStream::isOpen() const {
		if (_kfile != nullptr) {
			return true;
		}
		return false;
	}

	I32 KFOStream::close() {
		if (_kfile != nullptr) {
			return fclose(_kfile);
			_kfile = nullptr;
		}
		return 0;
	}

	void KFOStream::getFileInfoStr(const std::string &Address, KFileInfo &FInfo) {
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

	KMETA_KFOSTREAM_SOURCE();
}