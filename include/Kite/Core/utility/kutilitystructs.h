/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KUTILITYSTRUCTS_H
#define KUTILITYSTRUCTS_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
	struct KArchiveFileInfo{
		U32 index;
		U64 compressedSize;
		U64 uncompressedSize;
		char name[260];
		char comment[256];
		bool isCompressed;
	};

	//! Common array-based file header.
	/*!
		KArrayHeader is a simple and compact struct for save/load arrays of any type.
	*/
	struct KArrayHeader{
		char format[8];	//!< Null-terminated File format
		U32 objCount;	//!< Number of array elements
		U64 objSize;	//!< Size of array elements in byte
	};
}

#endif // KUTILITYSTRUCTS_H