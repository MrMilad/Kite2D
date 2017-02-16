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
	class KGLWindow;
	struct KSysInite {
		lua_State *_klstate;
		KGLWindow *_kwindow;
		KMetaManager *_kmman;

		KSysInite() :
			_klstate(nullptr), _kwindow(nullptr), _kmman(nullptr) {}

	};

	KM_CLASS(POD)
	struct KHandle {
		KMETA_KHANDLE_BODY();

	private:
		KM_VAR() CTypes type;

	public:
		KM_VAR() U16 signature; // 0 reserved for invalid handles
		KM_VAR() U32 index;		// 0 reserved for invalid handles

		// use this constructure for components
		KHandle(CTypes Type) :
			signature(0), index(0), type(Type) {}

		// use this constructure for entities
		KHandle() :
			signature(0), index(0), type(CTypes::maxSize){} 

		inline bool operator==(const KHandle& right) const {
			return (signature == right.signature) &&
				(index == right.index) &&
				(type == right.type);
		}

		inline bool operator!=(const KHandle& right) const  {
			return !((*this) == right);
		}

		KM_PRO_GET(KP_NAME = "componentType", KP_TYPE = CTypes, KP_CM = "type of the handled component")
		inline CTypes getCType() const { return type; }

	private:
		KM_OPE(KO_EQ)
		bool luaEq(const KHandle& right) const {
			(index == right.index) &&
				(type == right.type);
		}
	};

	KM_CLASS(POD)
	struct KTarckItem {
		KMETA_KTARCKITEM_BODY();

		KM_VAR() std::string name;
		KM_VAR() U32 loadCount;
		KM_VAR() U32 unloadCount;
		KM_VAR() U32 getCount;

		KTarckItem():
		loadCount(0), unloadCount(0), getCount(0){}
	};

	KM_CLASS(POD)
	struct KResourceInfo {
		KMETA_KRESOURCEINFO_BODY();

		KM_VAR() std::string name;
		KM_VAR() std::string address;
		KM_VAR() RTypes rtype;
		KM_VAR() IStreamTypes stype;

		KResourceInfo(const std::string &Name, const std::string &Address, RTypes RType, IStreamTypes SType) :
			name(Name), address(Address), rtype(RType), stype(SType) {}
	};
}

#endif // KECSSTRUCTS_H
