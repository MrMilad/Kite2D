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
#ifndef KMETACLASS_H
#define KMETACLASS_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/meta/kmetatypes.h"
#include "Kite/Core/meta/kmetaobject.h"
#include <string>
#include <vector>

namespace Kite {

	//! The KMemeber class is a meta data class that store information about members.
	/*!
	This class can stores information (name and offset of member) about a data member of a specific class. Multiple
	member objects can be stored in KMetaData objects within a std::vector.
	*/
	/*struct KMetaMember {
		std::string name;			//!< Name of member
		// U32 offset;				// its not safe and not portable!
		KMetaTypeInfoTypes typeInfo;
		U32 arraySize;				//!< Size of array
		std::type_index typeHandle;	//!< Type handle
		KMetaMember *next;			//!< Pointer to the next member

		KMetaMember(const std::string &Name, KMetaTypeInfoTypes TypeInfo,
					U32 ArraySize, const std::type_info& TypeHandle) :
			name(Name), typeInfo(TypeInfo), arraySize(ArraySize),
			typeHandle(TypeHandle), next(nullptr)
			 {}
	};*/

	struct KMetaProperty {
		std::string name;			//!< Name of property
		std::string typeName;		//!< Name of property type
		std::string comment;		//!< A Short comment about the property
		KMetaPropertyTypes type;	//!< Available properties
		KMetaProperty *next;		//!< Pointer to the next member

		KMetaProperty(const std::string &Name, const std::string &TypeName,
					  const std::string &Comment, KMetaPropertyTypes Type) :
			name(Name), typeName(TypeName), comment(Comment), type(Type), next(nullptr) {}
	};

	struct KMetaFunction {
		std::string name;							//!< Name of the function
		std::string returnName;						//!< Name of the return type
		bool isStatic;
		//std::vector<std::string> paramsName;		//!< Name of the function parameters
		std::vector<std::string> paramsType;		//!< Type of the function parameters
		KMetaFunction *next;						//!< Pointer to the next member

		KMetaFunction(const std::string &Name, const std::string& ReturnName, bool IsStatic) :
			name(Name), returnName(ReturnName), isStatic(IsStatic), next(nullptr)
		{}
	};

	struct KMetaBase {
		std::string name;			//!< Name of base class
		KMetaBaseInheriTypes type;	//!< public, private or protected
		KMetaBase *next;			//!< Pointer to the next member

		KMetaBase(const std::string &Name, KMetaBaseInheriTypes BaseType) :
			name(Name), type(BaseType), next(nullptr) {}
	};

	//! Object for holding various info about any C++ type for the MetaData reflection system.
	/*!
	*/
	class KITE_FUNC_EXPORT KMetaClass : public KMetaObject {
	public:
		KMetaClass(const std::string &Name, U32 Flag, U32 Size);

		~KMetaClass();

		//void addMember(const KMetaMember *Member);

		void addProperty(const KMetaProperty *Property);

		void addBase(const KMetaBase *Base);

		void addFunction(const KMetaFunction *Base);

		//inline const KMetaMember *getMembers() const { return _kmembers; }
		//inline bool hasMembers() const { return (_kmembers) ? true : false;}

		inline const KMetaProperty *getProperties() const { return _kproperties; }
		inline bool hasProperties() const { return (_kproperties) ? true : false; }

		inline const KMetaBase *getBase() const { return _kbases; }
		inline bool hasBase() const { return (_kbases) ? true : false; }

		inline const KMetaFunction *getFunctions() const { return _kfunctions; }
		inline bool hasFunction() const { return (_kfunctions) ? true : false; }

	private:
		//KMetaMember *_kmembers;
		//KMetaMember *_klastMember;
		KMetaProperty *_kproperties;
		KMetaProperty *_klastProperties;
		KMetaBase *_kbases;
		KMetaBase *_klastBase;
		KMetaFunction *_kfunctions;
		KMetaFunction *_klastFunc;
	};
}
#endif // KMETACLASS_H