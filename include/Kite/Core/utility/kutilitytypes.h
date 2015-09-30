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
#ifndef KUTILITYTYPES_H
#define KUTILITYTYPES_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite {
	/*! \typedef KCallCellQuery
	\brief Function handle for cell objects
	*/
	typedef void(*KCallCellQuery)(void *Parameter, void *Sender);

	typedef void(*KCallCellTrigger)(U32 CellID, void *Sender);

	typedef void(*KCallMetaInite)();

	/*! \enum KTileMapTypes
	\brief Types of tile-based maps
	*/
	enum KGridBSPTypes : U8 {
		KGT_ISOMETRIC = 0,	//!< Isometric map
		KGT_FLAT			//!< Flat (rectangular) map
	};

	/*! \enum KTileBitmapTypes
	\brief Isometric tiles - mouse detection (Internally use)
	*/
	enum KCellBitmapTypes : U8 {
		KCB_CENTER = 0,
		KCB_BOTTOM_LEFT,
		KCB_BOTTOM_RIGHT,
		KCB_TOP_LEFT,
		KCB_TOP_RIGHT
	};

	enum KMetaTypes : U8{
		KMT_POD = 0,	//!< Is POD (plain old data types)
		KMT_CLASS,		//!< Is Class
		KMT_ENUM,		//!< Is Enum
		KMT_OTHER		//!< Other types (we dont care about other types because we dont need them)
	};

	enum KSerialStateTypes : U8 {
		KST_SERIALIZE = 0,
		KST_DESERIALIZE
	};

	enum KMetaTypeInfoTypes : U8 {
		KPT_POINTER = 0,	//!< Is pointer
		KPT_ARRAY,			//!< Is Array
		KPT_REFERENCE,		//!< Is Refrence
		KPT_OTHER			//!< Other types (we dont care about other types because we dont need them)
	};

	enum KMetaBaseInheriTypes : U8 {
		KMB_PUBLIC = 0,
		KMB_PRIVATE,
		KMB_PROTECTED
	};

	enum KMetaPropertyTypes : U8 {
		KMP_SETTER = 0,
		KMP_GETTER,
		KMP_BOTH
	};

	enum KMetaPODTypes : U8 {
		KPO_UINTEGER = 0,
		KPO_IINTEGER,
		KPO_FLOAT,
		KPO_BOOL,
		KPO_STRING
	};

}

#endif // KUTILITYTYPES_H