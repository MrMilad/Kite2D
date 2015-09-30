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
#include "Kite/Core/utility/kmetadef.h"
#include "Kite/Core/utility/kmeta.h"
#include "Kite/Core/utility/kmetapod.h"
#include "Kite/Core/utility/kmetamanager.h"
#include "Kite/Core/utility/kbytesarray.h"
#include <string>

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

		struct KRegistPOD {
			static const KMetaPOD * registerMetaData(U8 Index = 0) {
				static bool registered = false;
				static KMetaPOD instances[] =
				{
					KMetaPOD("I8", 0, (U32)sizeof(I8), KPO_IINTEGER),
					KMetaPOD("I16", 0, (U32)sizeof(I16), KPO_IINTEGER),
					KMetaPOD("I32", 0, (U32)sizeof(I32), KPO_IINTEGER),
					KMetaPOD("I64", 0, (U32)sizeof(I64), KPO_IINTEGER),
					KMetaPOD("U8", 0, (U32)sizeof(U8), KPO_UINTEGER),
					KMetaPOD("U16", 0, (U32)sizeof(U16), KPO_UINTEGER),
					KMetaPOD("U32", 0, (U32)sizeof(U32), KPO_UINTEGER),
					KMetaPOD("U64", 0, (U32)sizeof(U64), KPO_UINTEGER),
					KMetaPOD("F32", 0, (U32)sizeof(F32), KPO_FLOAT),
					KMetaPOD("F64", 0, (U32)sizeof(F64), KPO_FLOAT),
					KMetaPOD("bool", 0, (U32)sizeof(bool), KPO_BOOL),
					KMetaPOD("string", 0, 0, KPO_STRING)
				};

				if (!registered) {
					registered = true;
					KMetaManager::setMeta(std::type_index(typeid(I8)), &instances[0]);
					KMetaManager::setMeta(std::type_index(typeid(I16)), &instances[1]);
					KMetaManager::setMeta(std::type_index(typeid(I32)), &instances[2]);
					KMetaManager::setMeta(std::type_index(typeid(I64)), &instances[3]);
					KMetaManager::setMeta(std::type_index(typeid(U8)), &instances[4]);
					KMetaManager::setMeta(std::type_index(typeid(U16)), &instances[5]);
					KMetaManager::setMeta(std::type_index(typeid(U32)), &instances[6]);
					KMetaManager::setMeta(std::type_index(typeid(U64)), &instances[7]);
					KMetaManager::setMeta(std::type_index(typeid(F32)), &instances[8]);
					KMetaManager::setMeta(std::type_index(typeid(F64)), &instances[9]);
					KMetaManager::setMeta(std::type_index(typeid(bool)), &instances[10]);
					KMetaManager::setMeta(std::type_index(typeid(std::string)), &instances[11]);
				}

				return &instances[Index];
			}
		};

	}

}

#endif // KUTILITYSTRUCTS_H