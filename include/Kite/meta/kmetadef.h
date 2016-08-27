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
#define ISTREAM		128
#define OSTREAM		256
#define SCRIPTABLE	512
#define SERIALIZER	1024

// supported operators
#define KO_ADD
#define KO_MUL
#define KO_SUB
#define KO_DIV

// property tags
#define KP_NAME // name of the property
#define KP_TYPE // type of the property
#define KP_CM	// comment of the property (optional)
#define KP_MIN	// minimum value of the property (optional)
#define KP_MAX	// maximum value of the property (optional)
#define KP_RES	// resource type (need only property is a resource holder)
#define KP_SHOW // show in components editor (optional, true by default)

// info tags
#define KI_NAME		// component name
#define KI_SHOW		// show component in editor (true by default if not defined)

// variable modifiers
#define UNBIND  // add this flag for unbindable pod's variables. eg: arrays or private variables

// property flags
#define KMFLAG_FINAL	1024

#define nullCast(T) (reinterpret_cast<T *>(NULL))

#define MERGE_STR(w1, w2) #w1#w2

#define TOSTR(S) #S

#define KREM_QUAL(KP_TYPE) std::remove_pointer<std::remove_all_extents<std::remove_reference<std::remove_cv<KP_TYPE>::type>::type>::type>::type

// takes a pointer and returns a pointer offset in bytes 
#define KPTR_GET_ADDR( PTR, OFFSET ) \
	((void *)(((char *)(PTR)) + (OFFSET)))

#define KMETA \
	struct lua_State;\
	namespace Kite{\
	class KMetaObject;\
	class KMetaClass;\
	class KMetaEnum;\
	class KMetaManager;}
	

#define KM_IGNORED			/// ignored files
#define KM_ENUM(...)		/// (name)
#define KM_CLASS(...)		/// (FLAG)
#define KM_CON(...)			///	(ptype1, ptype2, ...);
#define KM_PRO_SET(...)		///	(KP_NAME = "name")
#define KM_PRO_GET(...)		///	(KP_NAME = "name", KP_TYPE = type, KP_CM = "comment", KP_MIN = number, KP_MAX = number, KP_RES = "name")
#define KM_FUN(...)			/// (KP_NAME = "name" (optional))
#define KM_VAR(...)			/// ()
#define KM_OPE(...)			/// (OPERATOR)
#define KM_INFO(...)		/// ("key", "info")
#define KM_TEM_PARAM(...)	/// (param1, param1, ...)
#define KM_TEM_DEF(...)		/// ("name", val1, val2, ...)

#endif // KMETADEF_H