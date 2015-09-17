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
#ifndef KMETACREATOR_H
#define KMETACREATOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitydef.h"
#include "Kite/Core/utility/kmetaobject.h"
#include <string>

namespace Kite{

	template <typename Metatype>
	class KITE_FUNC_EXPORT KMetaCreator {
	public:
		KMetaCreator(const std::string &Name, U32 Size, U32 Flag) {
			init(Name, Size, Flag);
		}

		static void init(const std::string &Name, U32 Size, U32 Flag) {
			get()->init(Name, Size, Flag);
			registerMetaData();
		}

		static void addMember(const std::string &Name, U32 Offset, KMetaObject *Object) {
			get()->addMember(new KMember(Name, Offset, Object));
		}

		static Metatype *nullCast() {
			return reinterpret_cast<Metatype *>(NULL);
		}

		static void registerMetaData();

		// ensure a single instance can exist for this class type
		static KMetaObject *get() {
			static KMetaObject instance;
			return &instance;
		}
	};

}

#endif // KMETACREATOR_H