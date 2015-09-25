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
#include "Kite/Core/utility/kutilitydef.h"
#include "Kite/Core/utility/kmetaobject.h"
#include "Kite/Core/utility/kmetamanager.h"
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

	//! Used to check if a type is a pointer or not
	template<typename T>
	struct isPointer { static const bool value = false; };
	template<typename T>
	struct isPointer<T *> { static const bool value = true; };
	template<typename T>
	struct isPointer<T&> { static const bool value = false; };

	//! Strips down qualified types/references/pointers to a single unqualified type, for passing into a templated type as a typename parameter.
	/*!
	*/
	template <typename T>
	struct KUnqual {
		typedef T type;
	};

	template <typename T>
	struct KUnqual<const T> {
		typedef typename KUnqual<T>::type type;
	};

	template <typename T>
	struct KUnqual<T&> {
		typedef typename KUnqual<T>::type type;
	};

	template <typename T>
	struct KUnqual<const T&> {
		typedef typename KUnqual<T>::type type;
	};

	template <typename T>
	struct KUnqual<T&&> {
		typedef typename KUnqual<T>::type type;
	};

	template <typename T>
	struct KUnqual<const T *> {
		typedef typename KUnqual<T *>::type type;
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
			static const KMetaObject * registerMetaData(U8 RetIndex) {
				static bool registered = false;
				static KMetaObject instances[] =
				{
					KMetaObject("I8", (U32)sizeof(I8), KMFLAG_POD | KMFLAG_POD_I8),
					KMetaObject("I16", (U32)sizeof(I16), KMFLAG_POD | KMFLAG_POD_I16),
					KMetaObject("I32", (U32)sizeof(I32), KMFLAG_POD | KMFLAG_POD_I32),
					KMetaObject("I64", (U32)sizeof(I64), KMFLAG_POD | KMFLAG_POD_I64),
					KMetaObject("U8", (U32)sizeof(U8), KMFLAG_POD | KMFLAG_POD_U8),
					KMetaObject("U16", (U32)sizeof(U16), KMFLAG_POD | KMFLAG_POD_U16),
					KMetaObject("U32", (U32)sizeof(U32), KMFLAG_POD | KMFLAG_POD_U32),
					KMetaObject("U64", (U32)sizeof(U64), KMFLAG_POD | KMFLAG_POD_U64),
					KMetaObject("F32", (U32)sizeof(F32), KMFLAG_POD | KMFLAG_POD_F32),
					KMetaObject("F64", (U32)sizeof(F64), KMFLAG_POD | KMFLAG_POD_F64),
					KMetaObject("bool", (U32)sizeof(bool), KMFLAG_POD | KMFLAG_POD_BOOL),
					KMetaObject("string", 0, KMFLAG_POD | KMFLAG_POD_STRING)
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

				return &instances[RetIndex];
			}
		};

		template <typename T>
		struct getMeta {
			static const KMetaObject *getTMeta() { return KUnqual<T>::type::registerMetaData(); }
		};

		template <>
		struct getMeta<I8> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(0); }
		};

		template <>
		struct getMeta<I16> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(1); }
		};

		template <>
		struct getMeta<I32> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(2); }
		};

		template <>
		struct getMeta<I64> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(3); }
		};

		template <>
		struct getMeta<U8> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(4); }
		};

		template <>
		struct getMeta<U16> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(5); }
		};

		template <>
		struct getMeta<U32> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(6); }
		};

		template <>
		struct getMeta<U64> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(7); }
		};

		template <>
		struct getMeta<F32> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(8); }
		};

		template <>
		struct getMeta<F64> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(9); }
		};

		template <>
		struct getMeta<bool> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(10); }
		};

		template <>
		struct getMeta<std::string> {
			static const KMetaObject *getTMeta() { return KRegistPOD::registerMetaData(11); }
		};

	}

}

#endif // KUTILITYSTRUCTS_H