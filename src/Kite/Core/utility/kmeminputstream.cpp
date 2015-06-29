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
#include "Kite/Core/utility/kmeminputstream.h"

namespace Kite{
	KMemInputStream::KMemInputStream(const void *Data, U64 DataSize) :
		_kdata(Data),
		_ksize(DataSize),
		_kcurOfst(0)
	{}

	U64 KMemInputStream::read(void *Data, U64 DataSize){
		if (_kdata == 0 || _ksize == 0)
			return 0;

		if ((DataSize + _kcurOfst) > _ksize){
			DataSize = _ksize - _kcurOfst;
		}

		memcpy(Data, (U8 *)_kdata + _kcurOfst, DataSize);
		_kcurOfst += DataSize;
		return DataSize;
	}

	I32 KMemInputStream::seek(I64 Offset, I32 Origin){
		if (!_kdata)
			return -1;

		switch (Origin){
		case SEEK_SET:
			// calibration new offset
			if (Offset > _ksize)
				Offset = _ksize;

			// set new offset
			_kcurOfst = Offset;

			return 0;

		case SEEK_CUR:
			// calibration new offset
			if ((Offset + _kcurOfst) > _ksize)
				Offset = _ksize - _kcurOfst;

			// set new offset
			_kcurOfst += Offset;

			return 0;

		case SEEK_END:
			// calibration new offset
			if (Offset > _ksize)
				Offset = 0;

			// set new offset
			_kcurOfst = _ksize - Offset;

			return 0;

		default:
			break;
		}

		KDEBUG_PRINT("Invalid param");
		return -1;
	}

	I64 KMemInputStream::tell(){
		if (!_kdata)
			return -1;

		return _kcurOfst;
	}

	bool KMemInputStream::isOpen(){
		if (_kdata && _ksize > 0)
			return true;

		return false;
	}

	U64 KMemInputStream::getSize(){
		return _ksize;
	}

	I32 KMemInputStream::eof(){
		if (!_kdata)
			return -1;

		if (_ksize == _kcurOfst)
			return 1;

		return 0;
	}

	I32 KMemInputStream::close(){
		_kdata = 0;
		_ksize = 0;
		_kcurOfst = 0;
		return 0;
	}
}