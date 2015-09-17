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
#ifndef KUTILITYDEF_H
#define KUTILITYDEF_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kmetaobject.h"
#include "Kite/Core/utility/kmetacreator.h"

namespace Kite {
#define KINFLAT_BUFF_SIZE 32768

// meta flags
#define KMFLAG_POD 1
#define KMFLAG_CLASS 2


// takes a pointer and returns a pointer offset in bytes
#define KPTR_GET_ADD( PTR, OFFSET ) \
	((void *)(((char *)(PTR)) + (OFFSET)))

#define KMETA_DEF(TYPE, FLAG) \
	KMetaCreator<KUnqual<TYPE>::type> NAME_GENERATOR()(#TYPE, sizeof(TYPE), FLAG); \
	KUnqual<TYPE>::type *TYPE::nullCast() { return reinterpret_cast<KUnqual<TYPE>::type *>(NULL); } \
	void TYPE::addMember(const std::string &Name, U32 Offset, KMetaObject *Data) { return KMetaCreator<KUnqual<TYPE>::type>::addMember(Name, Offset, Data); } \
	void KMetaCreator<KUnqual<TYPE>::type>::registerMetaData() { TYPE::registerMetaData(); } \
	void TYPE::registerMetaData()

	// define plain old data types (U8, I8, U32, I32, ...)
#define KMETA_DEF_POD( TYPE ) \
	KMetaCreator<KUnqual<TYPE>::type> NAME_GENERATOR()(#TYPE, sizeof(TYPE), KMFLAG_POD); \
	void KMetaCreator<KUnqual<TYPE>::type>::registerMetaData() \
	{ \
}

	// KMETA_DATA
	// Purpose : This macro goes on the inside of a class within the public section. It declares
	//           a few member functions for use by the MetaData system to retrieve information about
	//           the class.
#define KMETA_DATA( TYPE ) \
	static void addMember(const std::string &Name, U32 Offset, KMetaObject *Data); \
	static KUnqual<TYPE>::type *nullCast(); \
	static void registerMetaData()

#define KMETA_ADD_MEMBER( MEMBER ) \
	addMember(#MEMBER, (unsigned)(&(nullCast()->MEMBER)), KMETA_GET_OBJECT(nullCast()->MEMBER))

#define PASTE_TOKENS_2( _, __ ) _##__
#define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

	//
	// KMETA_GET_TYPE
	// Purpose: Retrieves the proper meta-data instance of an object by type.
	//
#define KMETA_GET_TYPE( TYPE ) (KMetaCreator<KUnqual<TYPE>::type>::get( ))

	//
	// KMETA_GET_OBJECT
	// Purpose: Retrieves the proper meta-data instance of an object by an object's type.
	//
#define KMETA_GET_OBJECT( OBJECT ) (KMetaCreator<KUnqual<decltype( OBJECT )>::type>::get( ))

	//
	// KMETA_GET_STR
	// Purpose : Finds a MetaData instance by string name
	//
#define KMETA_GET_STR( STRING ) (MetaManager::get( STRING ))
}

#endif // KUTILITYDEF_H