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
#include "Kite/Core/utility/kfileinputstream.h"

namespace Kite{
	KFileInputStream::KFileInputStream(const std::string &FileName){
		_kfile = fopen(FileName.c_str(), "rb");
		if (_kfile == NULL){
			KDEBUG_PRINT("cant open file");
		}
	}

	KFileInputStream::~KFileInputStream(){
		if (_kfile)
			fclose(_kfile);
	}

	U64 KFileInputStream::read(void *Data, U64 DataSize){
		if (_kfile)
			return fread(Data, 1, DataSize, _kfile);
		return 0;
	}

	I32 KFileInputStream::seek(I64 Offset, I32 Origin){
		if (_kfile)
			return fseek(_kfile, Offset, Origin);
		return -1;
	}

	I64 KFileInputStream::tell(){
		if (_kfile)
			return ftell(_kfile);
		return -1;
	}

	bool KFileInputStream::isOpen(){
		if (_kfile)
			return true;
		return false;
	}

	U64 KFileInputStream::getSize(){
		if (_kfile){
			I64 cur = ftell(_kfile);
			fseek(_kfile, 0, SEEK_END);
			U64 size = ftell(_kfile);
			fseek(_kfile, cur, SEEK_SET);
			return size;
		}
		return 0;
	}

	I32 KFileInputStream::eof(){
		if (_kfile)
			return feof(_kfile);
		return 0;
	}

	I32 KFileInputStream::close(){
		if (_kfile)
			return fclose(_kfile);
		return 0;
	}
}