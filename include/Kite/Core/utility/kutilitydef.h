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
#include <cstddef>

namespace Kite {
#define KINFLAT_BUFF_SIZE 32768

// meta flags
#define KMFLAG_POD			1
#define KMFLAG_POD_I8		2
#define KMFLAG_POD_I16		4
#define KMFLAG_POD_I32		8
#define KMFLAG_POD_I64		16
#define KMFLAG_POD_U8		32
#define KMFLAG_POD_U16		64
#define KMFLAG_POD_U32		128
#define KMFLAG_POD_U64		256
#define KMFLAG_POD_F32		512
#define KMFLAG_POD_F64		1024
#define KMFLAG_POD_BOOL		2048
#define KMFLAG_POD_STRING	4096
#define KMFLAG_CLASS		8192

// POD register macro
/*#define POD_I8 Internal::KRegistPOD::registerMetaData(0)
#define POD_I16 Internal::KRegistPOD::registerMetaData(1)
#define POD_I32 Internal::KRegistPOD::registerMetaData(2)
#define POD_I64 Internal::KRegistPOD::registerMetaData(3)
#define POD_U8 Internal::KRegistPOD::registerMetaData(4)
#define POD_U16 Internal::KRegistPOD::registerMetaData(5)
#define POD_U32 Internal::KRegistPOD::registerMetaData(6)
#define POD_U64 Internal::KRegistPOD::registerMetaData(7)
#define POD_F32 Internal::KRegistPOD::registerMetaData(8)
#define POD_F64 Internal::KRegistPOD::registerMetaData(9)
#define POD_BOOL Internal::KRegistPOD::registerMetaData(10)
#define POD_STRING Internal::KRegistPOD::registerMetaData(11)*/


#define nullCast(T) (reinterpret_cast<T *>(NULL))

	// takes a pointer and returns a pointer offset in bytes
#define KPTR_GET_ADD( PTR, OFFSET ) \
	((void *)(((char *)(PTR)) + (OFFSET)))

	//
	// KMETA_GET_OBJECT
	// Purpose: Retrieves the proper meta-data instance of an object by an object's type.
	//
#define KMETA_GET_OBJECT( OBJECT ) OBJECT.registerMetaData()

#define KMETA_GET_TYPE( TYPE ) Internal::getMeta<TYPE>::getTMeta()

#define KMETA_DEF(TYPE, FLAG) \
public:\
	static const KMetaObject * registerMetaData() {\
	static bool registered = false; \
	static KMetaObject instance(#TYPE, sizeof(TYPE), FLAG); \
	if (!registered) {\
		registered = true; \
		KMetaManager::setMeta(std::type_index(typeid(TYPE)), &instance);

#define KMETA_DEF_END } return &instance; }


	// define plain old data types (U8, I8, U32, I32, ...)
#define KMETA_DEF_POD( TYPE ) \
	KMetaManager::setMeta(std::type_index(typeid(TYPE)), new KMetaObject(#TYPE, sizeof(TYPE), KMFLAG_POD))

#define KMETA_PROPERTY_SET(NAME, TYPE, SET)\
private:\
	inline void _prpset##NAME(KRefVariant Value) {\
	SET(Value.getValue<TYPE>());}

#define KMETA_PROPERTY_GET(NAME, TYPE, GET)\
private:\
	inline KRefVariant _prpget##NAME() const {\
	return KRefVariant(GET());}

#define KMETA_PROPERTY_SG(NAME, TYPE, SET, GET)\
	KMETA_PROPERTY_SET(NAME, TYPE, SET)\
	KMETA_PROPERTY_GET(NAME, TYPE, GET)

#define KMETA_ADD_MEMBER(PARRENT, NAME, TYPE) \
	instance.addMember(new KMetaMember(#NAME, (U32)offsetof(PARRENT, NAME), Internal::getMeta<TYPE>::getTMeta()));

//#define KMETA_ADD_POD_MEMBER(PARRENT, NAME, TYPE) \
//	instance.addMember(new KMetaMember(#NAME, (U32)offsetof(PARRENT, NAME), TYPE));

#define KMETA_ADD_SETTER(PARRENT, NAME, TYPE)\
	instance.addProperty(new KMetaProperty(#NAME,KMP_SETTER, KMETA_GET_TYPE(KUnqual<TYPE>::type)));\
	setterMap()->insert({ #NAME, (void (KProperty::*)()) &PARRENT::_prpset##NAME });

#define KMETA_ADD_GETTER(PARRENT, NAME, TYPE)\
	instance.addProperty(new KMetaProperty(#NAME, KMP_GETTER, KMETA_GET_TYPE(KUnqual<TYPE>::type))); \
	getterMap()->insert({ #NAME, (void (KProperty::*)()) &PARRENT::_prpget##NAME });

#define KMETA_ADD_BASE(TYPE, ACCESS)\
	instance.addBase(new KMetaBase(#TYPE, ACCESS, KMETA_GET_TYPE(KUnqual<TYPE>::type)));

#define PASTE_TOKENS_2( _, __ ) _##__
#define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )


	//
	// KMETA_GET_STR
	// Purpose : Finds a MetaData instance by string name
	//
#define KMETA_GET_STR( STRING ) (MetaManager::get( STRING ))

}

#endif // KUTILITYDEF_H