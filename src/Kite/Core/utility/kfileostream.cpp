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
#include "Kite/Core/utility/kfileostream.h"

namespace Kite {
	KFileOStream::KFileOStream(const std::string &FileName) {
		_kfile = fopen(FileName.c_str(), "wb");
		if (_kfile == NULL) {
			KDEBUG_PRINT("cant open file");
		}
	}

	KFileOStream::~KFileOStream() {
		if (_kfile)
			fclose(_kfile);
	}

	bool KFileOStream::write(void *Data, U64 DataSize) {
		if (_kfile)
		if (fwrite(Data, (size_t)DataSize, 1, _kfile) == 1)
			return true;
		return false;
	}


	bool KFileOStream::isOpen() {
		if (_kfile)
			return true;
		return false;
	}

	I32 KFileOStream::close() {
		if (_kfile)
			return fclose(_kfile);
		return 0;
	}
}