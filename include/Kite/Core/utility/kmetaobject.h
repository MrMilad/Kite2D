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
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kbytesarray.h"
#include <string>

namespace Kite {
	//! Object for holding various info about any C++ type for the MetaData reflection system.
	/*!
	*/
	class KITE_FUNC_EXPORT KMetaObject {
	public:
		KMetaObject(const std::string &Name = "", U32 Size = 0, U32 Flag = 0);

		void init(const std::string &string, U32 Size, U32 Flag);

		void addMember(const KMember *member);

		inline const std::string& getName() const { return _kname; }
		inline U32 getSize() const { return _ksize; }
		inline U32 getFlag() const { return _kflag; }
		inline const KMember *getMembers() const { return _kmembers; }
		inline bool hasMembers() const { return (_kmembers) ? true : false;}

	private:
		KMember *_kmembers;
		KMember *_klastMember;
		std::string _kname;
		U32 _ksize;
		U32 _kflag;
	};
}
#endif // KMETAOBJECT_H