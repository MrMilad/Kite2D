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
#include "Kite/logic/klogiccom.h"
#include "Kite/logic/klogicsys.h"
#include "Kite/logic/kscript.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/ecs/knode.h"
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>
#ifdef KITE_DEV_DEBUG
#include <exception>
#endif

namespace Kite {
	void KLogicCom::inite() {
		_ksys = nullptr;
		_klid = 0;
	}

	void KLogicCom::attached() {}

	void KLogicCom::deattached() {}

	void KLogicCom::setScript(const KSharedResource &Script) {
		if (_kscript != Script) {
			_kscript = Script;
			if (getOwnerNode()->isOnActiveHierarchy()) {
				(_ksys->*_kreloadScriptCallb)(this);
			}
		}
	}

	int KLogicCom::getTable(lua_State *L){
		if (getOwnerNode()->isOnActiveHierarchy()) {
			auto table = LuaIntf::LuaRef::globals(L).get("coms").get(getTableIndex());
			LuaIntf::Lua::push(L, table);
			return 1;
		}
		
		KD_FPRINT("owner node of this component is not active: component name: %s", getName().c_str());
		LuaIntf::Lua::push(L, nullptr);
		return 1;
	}

	KLOGICCOM_SOURCE();
}