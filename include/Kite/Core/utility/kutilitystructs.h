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

	struct KVoidStream {
		U32 size;
		void *pointer;

		KVoidStream(U32 Size = 0, void *Pointer = 0) :
			size(Size), pointer(Pointer) 
		{}
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


	class KMetaObject;
	//! The KMemeber class is a meta data class that store information about members.
	/*!
	This class can stores information (name and offset of member) about a data member of a specific class. Multiple
	member objects can be stored in KMetaData objects within a std::vector.
	*/
	struct KMember {
		std::string name;			//!< Name of member
		U32 offset;					//!< Offset of memeber in bytes
		const KMetaObject *meta;	//!< Registered Data
		KMember *next;				//!< Pointer to the next member

		KMember(const std::string &Name, U32 Offset, KMetaObject *Meta) :
			name(Name), offset(Offset), meta(Meta), next(NULL) {}
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
}

#endif // KUTILITYSTRUCTS_H