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

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/meta/kmetabase.h"
#include <string>
#include <list>
#include <utility>

namespace Kite {
	struct KMetaProperty {
		std::string name;			//!< Name of property
		std::string typeName;		//!< Name of property type
		std::string comment;		//!< A Short comment about the property
		std::string resType;		//!< Type of inpu resource
		I32 min;					//!< Minimum value of the property
		I32 max;					//!< Maximum value of the property
		KMetaPropertyTypes type;	//!< Available properties

		KMetaProperty(const std::string &Name, const std::string &TypeName,
					  const std::string &Comment, KMetaPropertyTypes Type,
					  I32 Min, I32 Max, const std::string &ResType) :
			name(Name), typeName(TypeName), comment(Comment),
			type(Type), min(Min), max(Max),
			resType(ResType){}

		KMetaProperty():
			type(KMP_GETTER), min(0), max(0) {}
	};

	struct KMetaFunction {
		std::string name;			//!< Name of the function
		bool isStatic;

		KMetaFunction(const std::string &Name, bool IsStatic) :
			name(Name), isStatic(IsStatic)
		{}
	};

	//! Object for holding various info about any C++ class\struct for the MetaData reflection system.
	/*!
	*/
	class KITE_FUNC_EXPORT KMetaClass : public KMetaBase {
	public:
		KMetaClass(const std::string &Name, U32 Flag, U32 Size);

		void addProperty(const KMetaProperty &Property);

		void addFunction(const KMetaFunction &Base);

		inline U32 getFlags() const { return _kflags; }

		inline const auto getProperties() const { return &_kprop; }

		inline const auto getFunctions() const { return &_kfunc; }

	private:
		std::list<KMetaProperty> _kprop;
		std::list<KMetaFunction> _kfunc;
		
		U32 _kflags;
	};
}
#endif // KMETACLASS_H