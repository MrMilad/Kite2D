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
#ifndef KMETADEF_H
#define KMETADEF_H

#include "Kite/Core/system/ksystemdef.h"
#include <cstddef>
#include <type_traits>

namespace Kite {
	// general meta flags 
#define KMFLAG_EDITABLE		1
#define KMFLAG_SCRIPTABLE	2

	// property flags
#define KMFLAG_FINAL		1024

#define nullCast(T) (reinterpret_cast<T *>(NULL))

#define MERGE_STR(w1, w2) #w1#w2

#define KOPT(TYPE) LuaIntf::_opt<TYPE>
#define KDEF(TYPE, VAL) LuaIntf::_def<TYPE, VAL>

#define KREM_QUAL(TYPE) std::remove_pointer<std::remove_all_extents<std::remove_reference<std::remove_cv<TYPE>::type>::type>::type>::type

	// takes a pointer and returns a pointer offset in bytes
#define KPTR_GET_ADDR( PTR, OFFSET ) \
	((void *)(((char *)(PTR)) + (OFFSET)))

	//
	// KMETA_GET_OBJECT
	// Purpose: Retrieves the proper meta-data instance of an object by an object's type.
	//
#define KMETA_GET_TYPE(TYPE) KMetaManager::getMeta(typeid(KREM_QUAL(TYPE)))

#define KMETA_GET_OBJECT(OBJECT) KMetaManager::getMeta(typeid(KREM_QUAL(decltype(OBJECT))))

#define KMETA \
	namespace Kite{\
	class KMeta;\
	class KMetaClass;\
	class KMetaEnum;\
		}\
	struct lua_State;


#define KMETACLASS(TYPE)\
public:\
	KITE_FUNC_EXPORT static const KMetaClass *registerMetaData(TYPE *thisPtr = nullptr, bool serial = false, \
	KSerialStateTypes State = KST_SERIALIZE, KBytesArray *Serializer = nullptr, lua_State *Lua = nullptr); \
	friend KBytesArray &operator<<(KBytesArray &Out, TYPE &Value) {\
	registerMetaData(&Value, true, KST_SERIALIZE, &Out); return Out;}\
	friend KBytesArray &operator>>(KBytesArray &In, TYPE &Value) {\
	registerMetaData(&Value, true, KST_DESERIALIZE, &In); return In;}\
	friend KBytesArray &operator<<(KBytesArray &Out, TYPE *Value) {\
	registerMetaData(Value, true, KST_SERIALIZE, &Out); return Out;}\
	friend KBytesArray &operator>>(KBytesArray &In, TYPE *Value) {\
	registerMetaData(Value, true, KST_DESERIALIZE, &In); return In;}

#define KMETACLASS_DEF(TYPE, FLAG, ...) \
	const KMetaClass *TYPE::registerMetaData(TYPE *thisPtr, bool serial,\
	KSerialStateTypes State, KBytesArray *Serializer, lua_State *Lua) {\
	typedef TYPE parrent;\
	static const char pname[] = #TYPE;\
	static bool registered = false; \
	static KMetaClass instance(#TYPE, FLAG, sizeof(TYPE)); \
	KMetaTypeInfoTypes metaTypeInfo; \
	U32 arraySize = 0; \
	const U32 flag = FLAG;\
	if (!registered) { KMetaManager::setMeta(std::type_index(typeid(TYPE)), (KMeta *)&instance); }\
	if (!registered && (flag & KMFLAG_SCRIPTABLE) && Lua != nullptr) { LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<TYPE>(#TYPE)\
	.addConstructor(LUA_ARGS(##__VA_ARGS__)).endClass().endModule(); }

#define KMETACLASS_END(TYPE)\
	registered = true; return &instance; }

#define KMETACLASS_PROPERTY(TYPE, NAME, GET, SET)\
private:\
	typedef TYPE _prptype##NAME;\
	inline static decltype(&SET) _prpsetPtr##NAME(){ return  &SET; }\
	inline static decltype(&GET) _prpgetPtr##NAME(){ return  &GET; }\
	inline void _prpset##NAME(KRefVariant Value) { SET(Value.getValue<TYPE>()); }\
	inline void _prpget##NAME(KRefVariant Value) const { Value.copyByVal(GET()); }

#define KMETACLASS_ADD_BASE(TYPE, ACCESS)\
	if (serial) { TYPE *basePtr = (TYPE *)thisPtr;\
	if (State == KST_SERIALIZE) { (*Serializer) << (*basePtr); } else { (*Serializer) >> (*basePtr); } }\
	if (!registered) { instance.addBase(new KMetaBase(#TYPE, ACCESS, typeid(KREM_QUAL(TYPE)))); }

#define KMETACLASS_ADD_MEMBER(NAME) \
	if (serial && thisPtr != nullptr) {\
	if (State == KST_SERIALIZE) { (*Serializer) << thisPtr->NAME; } else { (*Serializer) >> thisPtr->NAME; }}\
	if (!registered) {\
	if (std::is_array<decltype(parrent::NAME)>::value) { metaTypeInfo = KPT_ARRAY; }\
	else if (std::is_pointer<decltype(parrent::NAME)>::value) { metaTypeInfo = KPT_POINTER; }\
	else if (std::is_reference<decltype(parrent::NAME)>::value) { metaTypeInfo = KPT_REFERENCE; }\
	else { metaTypeInfo = KPT_OTHER; }\
	arraySize = std::extent<decltype(parrent::NAME)>::value; \
	instance.addMember(new KMetaMember(#NAME, (U32)offsetof(parrent, NAME), metaTypeInfo, arraySize, \
	typeid(KREM_QUAL(decltype(parrent::NAME)))));}

#define KMETACLASS_ADD_PROPERTY(NAME, COMMENT)\
	if (!registered) {\
	instance.addProperty(new KMetaProperty(#NAME, #COMMENT, KMP_BOTH, typeid(parrent::_prptype##NAME))); \
	getPrpMap()->insert({ MERGE_STR(NAME, set), (void (KProperty::*)(KRefVariant)) &parrent::_prpset##NAME }); \
	getPrpMap()->insert({ MERGE_STR(NAME, get), (void (KProperty::*)(KRefVariant)) &parrent::_prpget##NAME });}\
	if (!registered && (flag & KMFLAG_SCRIPTABLE) && Lua != nullptr) { LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<parrent>(pname)\
	.addProperty(#NAME, _prpgetPtr##NAME(), _prpsetPtr##NAME()).endClass().endModule(); }

#define KMETACLASS_ADD_METHODE(NAME) 

#define ENUMELEM(NAME, VALUE) NAME = VALUE,

#define ENUMDEF(NAME, ELEM) enum class NAME : I32 { ELEM(ENUMELEM) };

#define ENUMMETAELEM(NAME, VALUE)\
	instance.addMember(new KMetaEnumMember(#NAME, (I32)VALUE, ++number));

#define ENUMMETA(NAME, FLAG, ELEM) \
	struct Enum##NAME{\
	static const KMetaEnum *registerMetaData(){\
	static bool registered = false; \
	static KMetaEnum instance(#NAME, FLAG, sizeof(I32), typeid(I32)); \
	U32 number = 0;\
	if (!registered) {\
	KMetaManager::setMeta(std::type_index(typeid(NAME)), (KMeta *)&instance);\
	ELEM(ENUMMETAELEM)
	

#define KMETAENUM_DEF(NAME, FLAG, ELEM) \
	ENUMDEF(NAME, ELEM)\
	ENUMMETA(NAME, FLAG, ELEM)\
	}registered = true; return &instance; }};

#define PASTE_TOKENS_2( _, __ ) _##__
#define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )
}

#endif // KMETADEF_H