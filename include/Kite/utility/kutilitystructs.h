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
#ifndef KUTILITYSTRUCTS_H
#define KUTILITYSTRUCTS_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/utility/kutilitytypes.h"

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

	struct KGridBSPInfo {
		KGridBSPTypes gridType;	//!< type of grid (KGridBSPTypes)
		KVector2F32 cellSize;	//!< x = width, y = height
		KVector2U32 gridSize;	//!< x = number of cells in row, y = number of cells in column

		KGridBSPInfo() :
			gridType(KGT_FLAT), cellSize(32, 32), gridSize(100, 100) {}

		KGridBSPInfo(KGridBSPTypes Type, const KVector2F32 &TileSize, const KVector2U32 &MapSize) :
			gridType(Type), cellSize(TileSize), gridSize(MapSize) {}

	};

	namespace Internal {
		struct KLinkedPoint {
			KVector2F32 point;
			void *next;
			void *prev;
			KLinkedPoint() :
				point(),
				next(0),
				prev(0) {}
		};

		struct KCell {
			void *first;
			void *second;
			void *sender;
			KCallCellTrigger trigger;
			KCell() :
				first(0),
				second(0),
				sender(0),
				trigger(0) {}
		};
	}

}

#endif // KUTILITYSTRUCTS_H