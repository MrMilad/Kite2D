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
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KLogicCom::KLogicCom(const std::string &Name) :
		KComponent(Name),
		_kinite(false), _kstart(false), _klstate(nullptr)
		{}

	void KLogicCom::attached() {
		_ktname = "ENT" + std::to_string(getOwnerHandle().index);
	}

	void KLogicCom::deattached() {
		removeLuaEnv();
	}

	KRecieveTypes KLogicCom::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		 std::string ctable = "_G.ENTITIES." + _ktname + "." + getName() + ".onMessage";
		if (_klstate != nullptr) {
			LuaIntf::LuaRef lref(_klstate, ctable.c_str());
			if (lref.isFunction()) {
				lref(Message);
				return KRecieveTypes::KMR_RECEIVED;
			}
		}
		return KRecieveTypes::KMR_IGNORED;
	}

	void KLogicCom::setScript(const std::string &ResName) {
		if (!ResName.empty()) {
			if (ResName != _kresName) {
				removeLuaEnv();
				_kresName = ResName;
				setNeedUpdate(true);
			}
		}
	}

	void KLogicCom::removeLuaEnv() {
		// remove environment table from lua
		std::string ctable = "_G.ENTITIES." + _ktname;
		if (_klstate != nullptr) {
			LuaIntf::LuaRef lref(_klstate, ctable.c_str());
			if (lref.isTable()) {
				lref.remove(this->getName().c_str());
			}
		}
	}

	void KLogicCom::setLuaState(lua_State *L) {
		_klstate = L;
	}

	KMETA_KLOGICCOM_SOURCE();
}