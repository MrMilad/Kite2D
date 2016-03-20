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

#include "Kite/core/kcoredef.h"
#include <cstddef>
#include <type_traits>

// supported flags
#define POD			1
#define COMPONENT	2
#define RESOURCE	4
#define ENTITY		8
#define SYSTEM		16
#define ABSTRACT	32
#define CONTINER	64
#define SCRIPTABLE	128

// property flags
#define KMFLAG_FINAL	1024


#define nullCast(T) (reinterpret_cast<T *>(NULL))

#define MERGE_STR(w1, w2) #w1#w2

#define TOSTR(S) #S

#define KREM_QUAL(TYPE) std::remove_pointer<std::remove_all_extents<std::remove_reference<std::remove_cv<TYPE>::type>::type>::type>::type

// takes a pointer and returns a pointer offset in bytes
#define KPTR_GET_ADDR( PTR, OFFSET ) \
	((void *)(((char *)(PTR)) + (OFFSET)))

#define KMETA \
	struct lua_State;\
	namespace Kite{\
	class KMetaObject;\
	class KMetaClass;\
	class KMetaEnum;}
	

#define KMETA_IGNORED
#define KMETA_ENUM(...)
#define KMETA_CLASS(...)
#define KMETA_CONSTRUCTURE(...)
#define KMETA_PROPERTY(...)
#define KMETA_FUNCTION(...)
#define KMETA_VARIABLE(...)

#endif // KMETADEF_H