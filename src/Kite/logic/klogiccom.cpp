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
#include "Kite/logic/kreglogiccom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/core/kresourcemanager.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>
#ifdef KITE_DEV_DEBUG
#include <exception>
#endif

namespace Kite {
	KLogicCom::KLogicCom(const std::string &Name) :
		KComponent(Name),
		_klstate(nullptr)
	{
		addDependency(CTypes::RegisterLogic);
	}

	void KLogicCom::attached(KEntity *Entity) {
		_ktname = Entity->getLuaTName();

		// entity manager will not create 'register components' on deserial because they are removed by systems after registration phase
		// so we will add it at runtime
		if (!Entity->hasComponentType(CTypes::RegisterLogic)) {
			Entity->addComponent(CTypes::RegisterLogic);
		}

		// add to inite list for initialization
		auto regCom = (KRegLogicCom *)Entity->getComponent(CTypes::RegisterLogic);
		regCom->addToIniteList(getHandle());
	}

	void KLogicCom::deattached(KEntity *Entity) {
		removeLuaEnv();
	}

	RecieveTypes KLogicCom::onMessage(KMessage *Message, MessageScope Scope) {
		if (_klstate != nullptr) {
			LuaIntf::LuaRef lref(_klstate, "_G.hooks.callDirect");
			if (lref.isFunction()) {
#ifdef KITE_DEV_DEBUG
				try {
					lref("onGameMessage", getHandle(), Message);
				} catch (std::exception& e) {
					KD_FPRINT("onGameMessage function failed. %s", e.what());
					return RecieveTypes::IGNORED;
				}
#else
				lref("onGameMessage", getHandle(), Message);
#endif
				return RecieveTypes::RECEIVED;
			}
		}
		return RecieveTypes::IGNORED;
	}

	void KLogicCom::setScript(const KStringID &ResName) {
		if (ResName.hash != _kscriptName.hash) {
			removeLuaEnv();
			_kscriptName = ResName;
			resNeedUpdate();
		}
	}

	void KLogicCom::removeLuaEnv() {
		std::string ctable = "_G.ENT." + _ktname;
		if (_klstate != nullptr) {
			// unhook
			LuaIntf::LuaRef hooks(_klstate, "_G.hooks.remove");
			hooks(getHandle());

			// remove environment table from lua
			LuaIntf::LuaRef lref(_klstate, ctable.c_str());
			if (lref.isTable()) {
				lref.remove(this->getName().c_str());
			}
		}
	}

	bool KLogicCom::updateRes() {
		if (!getResNeedUpdate()) {
			return true;
		}

		// load resources
		if (getRMan()) {
			_kscript = (KScript *)getRMan()->get(_kscriptName.str);
			resUpdated();
			return true;
		}

		return false;
	}

	void KLogicCom::setLuaState(lua_State *L) {
		_klstate = L;
	}

	KMETA_KLOGICCOM_SOURCE();
}