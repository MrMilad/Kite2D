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
#ifndef KECSSTRUCTS_H
#define KECSSTRUCTS_H

#include "Kite/ecs/kecstypes.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include "kecsstructs.khgen.h"

KMETA
namespace Kite{
	/*KM_CLASS(POD)
	struct KHandle {
		KHANDLE_BODY();

	private:
		KM_VAR() ctypes type;

	public:
		KM_VAR() U16 signature; // 0 reserved for invalid handles
		KM_VAR() U32 index;		// 0 reserved for invalid handles

		// use this constructure for components
		KHandle(ctypes Type) :
			signature(0), index(0), type(Type) {}

		// use this constructure for entities
		KHandle() :
			signature(0), index(0), type(ctypes::maxSize){} 

		inline bool operator==(const KHandle& right) const {
			return (signature == right.signature) &&
				(index == right.index) &&
				(type == right.type);
		}

		inline bool operator!=(const KHandle& right) const {
			return !((*this) == right);
		}

		KM_PRO_GET(KP_NAME = "componentType", KP_TYPE = ctypes, KP_CM = "type of the handled component")
		inline ctypes getCType() const { return type; }

	private:
		KM_OPE(KO_EQ)
		bool luaEq(const KHandle& right) const {
			return ((index == right.index) && (type == right.type));
		}
	};*/

	KM_CLASS(POD)
	struct KResourceInfo {
		KM_INFO(KI_NAME = "ResourceInfo");
		KRESOURCEINFO_BODY();

		KM_VAR() std::string name;
		KM_VAR() std::string address;
		KM_VAR() Resource rtype;
		KM_VAR() InStream stype;

		KResourceInfo(const std::string &Name, const std::string &Address, Resource RType, InStream SType) :
			name(Name), address(Address), rtype(RType), stype(SType) {}
	};
}

#endif // KECSSTRUCTS_H
