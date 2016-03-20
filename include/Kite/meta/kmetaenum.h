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
#ifndef KMETAENUM_H
#define KMETAENUM_H

#include "Kite/core/kcoredef.h"
#include "Kite/utility/kutilitytypes.h"
#include "Kite/meta/kmetabase.h"
#include <string>

namespace Kite {
	struct KMetaEnumMember {
		std::string name;		//!< Name of the member
		I32 value;				//!< Value of the member
		U32 number;				//!< Number of the member in given order
		KMetaEnumMember *next;	//!< Pointer to next member

		KMetaEnumMember(const std::string &Name, I32 Value, U32 Number) :
			name(Name), value(Value), number(Number), next(nullptr)
		{}
	};
	class KITE_FUNC_EXPORT KMetaEnum : public KMetaBase {
	public:
		KMetaEnum(const std::string &Name, U32 Flag, U32 Size, const std::string& TypeName);
		~KMetaEnum();

		void addMember(const KMetaEnumMember *Member);

		inline const KMetaEnumMember *getMembers() const { return _kmembers; }
		inline bool hasMembers() const { return (_kmembers) ? true : false; }

		inline const std::string &getTypeHandle() const { return _ktypeName; }

	private:
		KMetaEnumMember *_kmembers;
		KMetaEnumMember *_klastMember;
		std::string _ktypeName;
	};
}

#endif // KMETAENUM_H