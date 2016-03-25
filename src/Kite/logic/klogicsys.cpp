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
#include "Kite/logic/klogicsys.h"
#include <vector>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	void KLogicSys::update(F32 Delta, KEntityManager &EManager, KResourceManager &RManager) {
		if (EManager.isRegistered(KComTypes::KCT_LOGIC)) {
			for (auto it = EManager.beginEntity(); it != EManager.endEntity(); ++it) {
				if (it->hasComponent(KComTypes::KCT_LOGIC) &&
					it->getActive()) {

					// retrive all script components from entity
					static std::vector<KComponent *> components;
					it->getScriptComponents(components);
					// iterate over all logic components and update them
					for (auto comp = components.begin(); comp != components.end(); ++comp) {

						// inite component and bind it to lua vm (only one time when current script changed with a new script)
						if ((*comp)->getNeedUpdateRes()) {
							cathcAndRegist((KLogicCom *)(*comp), RManager);
						}
					}
				}
			}
		}
	}

	bool KLogicSys::inite(void *Data) {
		if (Data != nullptr && !getInite()) {
			_klvm = static_cast<lua_State *>(Data);

			// creata entiti table in global
			LuaIntf::LuaRef glob(_klvm, "_G");
			LuaIntf::LuaRef ent = LuaIntf::LuaRef::createTable(_klvm);
			glob.set("ENTITIES", ent);

			setInite(true);
			return true;
		}

		return getInite();
	}

	void KLogicSys::cathcAndRegist(KLogicCom *Component, KResourceManager &RManager) {
		auto script = RManager.get<KScript>(Component->getScript());
		Component->setScriptPtr(script);

		// bind it to lua with its environment
		if (script != nullptr && !script->getCode().empty()) {
			// first check entiti table 
			std::string code("_G.ENTITIES." + Component->getCName());
			LuaIntf::LuaRef etable(_klvm, code.c_str());

			// there isn't a table for entity
			// we create it
			if (!etable.isTable()) {
				etable = LuaIntf::LuaRef(_klvm, "_G.ENTITIES");
				etable.set(Component->getCName().c_str(), LuaIntf::LuaRef::createTable(_klvm));
			}

			// create a table for its component
			code.clear();
			code = "_G.ENTITIES." + Component->getCName();
			etable = LuaIntf::LuaRef(_klvm, code.c_str());
			etable.set(Component->getName(), LuaIntf::LuaRef::createTable(_klvm));

			// load chunk and set its environment
			code.clear();
			code.reserve(script->getCode().size());
			code = script->getCode();

			std::string address("ENTITIES." + Component->getCName() + "." + Component->getName());
			code.insert(0, "_ENV = " + address + " \n");
			code.insert(0, address + ".Global = _G \n");
			code.insert(0, address + ".Kite = _G.Kite \n");
			code.insert(0, address + ".Entities = _G.ENTITIES \n");
			code.insert(0, address + ".print = print \n");

			luaL_dostring(_klvm, code.c_str());

			// call inite function of chunk
			address.append(".inite");
			LuaIntf::LuaRef ctable(_klvm, address.c_str());
			if (ctable.isFunction()) {
				ctable();
			}

			Component->setNeedUpdateRes(false);
		}
	}

	KMETA_KLOGICSYS_SOURCE();
}