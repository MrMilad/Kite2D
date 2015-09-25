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
#ifndef KMETAOBJECT_H
#define KMETAOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitytypes.h"
#include "Kite/Core/utility/kbytesarray.h"
#include <string>

namespace Kite {

	class KMetaObject;
	//! The KMemeber class is a meta data class that store information about members.
	/*!
	This class can stores information (name and offset of member) about a data member of a specific class. Multiple
	member objects can be stored in KMetaData objects within a std::vector.
	*/
	struct KMetaMember {
		std::string name;			//!< Name of member
		U32 offset;					//!< Offset of memeber in bytes
		const KMetaObject *meta;	//!< Registered Data (KMetaObject *)
		KMetaMember *next;			//!< Pointer to the next member

		KMetaMember(const std::string &Name, U32 Offset, const KMetaObject *Meta) :
			name(Name), offset(Offset), meta(Meta), next(NULL) {}
	};

	struct KMetaProperty {
		std::string name;			//!< Name of property
		KMetaPropertyType type;		//!< Available properties
		const KMetaObject *meta;	//!< Registered Data (KMetaObject *)
		KMetaProperty *next;		//!< Pointer to the next member

		KMetaProperty(const std::string &Name, KMetaPropertyType Type, const KMetaObject *Meta) :
			name(Name), type(Type), meta(Meta), next(NULL) {}
	};

	struct KMetaBase {
		std::string name;			//!< Name of base class
		KMetaBaseInheriType type;	//!< public, private or protected
		const KMetaObject *meta;	//!< Registered Data (KMetaObject *)
		KMetaBase *next;			//!< Pointer to the next member

		KMetaBase(const std::string &Name, KMetaBaseInheriType BaseType, const KMetaObject *Meta) :
			name(Name), type(BaseType), meta(Meta), next(NULL) {}
	};

	//! Object for holding various info about any C++ type for the MetaData reflection system.
	/*!
	*/
	class KITE_FUNC_EXPORT KMetaObject {
	public:
		KMetaObject(const std::string &Name = "", U32 Size = 0, U32 Flag = 0);
		~KMetaObject();

		void addMember(const KMetaMember *Member);

		void addProperty(const KMetaProperty *Property);

		void addBase(const KMetaBase *Base);

		inline const std::string& getName() const { return _kname; }

		inline U32 getSize() const { return _ksize; }
		inline U32 getFlag() const { return _kflag; }
		inline const std::string &getName() { return _kname; }

		inline const KMetaMember *getMembers() const { return _kmembers; }
		inline bool hasMembers() const { return (_kmembers) ? true : false;}

		inline const KMetaProperty *getProperties() const { return _kproperties; }
		inline bool hasProperties() const { return (_kproperties) ? true : false; }

		inline const KMetaBase *getBase() const { return _kbases; }
		inline bool hasBase() const { return (_kbases) ? true : false; }

	private:
		KMetaMember *_kmembers;
		KMetaMember *_klastMember;
		KMetaProperty *_kproperties;
		KMetaProperty *_klastProperties;
		KMetaBase *_kbases;
		KMetaBase *_klastBase;
		std::string _kname;
		U32 _ksize;
		U32 _kflag;
	};
}
#endif // KMETAOBJECT_H