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
	void KLogicSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		// check component registration
		if (EManager->isRegisteredComponent("Logic")) {

			// iterate over components
			for (auto it = EManager->beginComponent<KLogicCom>("Logic");
			it != EManager->endComponent<KLogicCom>("Logic"); ++it) {
				auto ehandle = it->getOwnerHandle();

				// get associated entity for each component
				auto entity = EManager->getEntity(ehandle);
				if (entity->getActive()) {

					// retrive all script components from entity
					static std::vector<KComponent *> components;
					entity->getScriptComponents(components);

					// iterate over all logic components and inite/update them
					for (auto comp = components.begin(); comp != components.end(); ++comp) {
						auto lcomp = static_cast<KLogicCom *>((*comp));

						// inite component and bind it to lua vm (only one time when current script changed with a new script)
						if (lcomp->getNeedUpdate()) {
							cathcAndRegist((KLogicCom *)(*comp), RManager);
						}

						// inite component (calling inite, only 1 time befor start)
						if (!lcomp->_kinite){
							initeComp(entity, lcomp);
							lcomp->_kinite = true;
							continue;
						}

						// start component (calling start, only 1 time befor update)
						if (!lcomp->_kstart){
							startComp(entity, lcomp);
							lcomp->_kstart = true;
							continue;
						}

						// update component (calling update, per frame)
						updateComp(Delta, entity, lcomp);
					}
				}
			}
		}
	}

	bool KLogicSys::inite(void *Data) {
		if (Data != nullptr && !isInite()) {
			_klvm = static_cast<lua_State *>(Data);

			// creata entiti table in global
			LuaIntf::LuaRef glob(_klvm, "_G");
			LuaIntf::LuaRef ent = LuaIntf::LuaRef::createTable(_klvm);
			glob.set("ENTITIES", ent);

			setInite(true);
			return true;
		}

		return isInite();
	}

	void KLogicSys::updateComp(F32 Delta, KEntity *Self, KLogicCom *Component) {
		std::string address("ENTITIES." + Component->getTName() + "." + Component->getName() + ".update");

		// call update function
		LuaIntf::LuaRef ctable(_klvm, address.c_str());
		if (ctable.isFunction()) {
			ctable(&Self, Delta);
		}
	}

	void KLogicSys::cathcAndRegist(KLogicCom *Component, KResourceManager *RManager) {
		// retrive script rsource from resource manager
		KScript *script = (KScript *)RManager->get(Component->getScript());
		Component->_kscript = script;

		Component->setLuaState(_klvm);

		// bind it to lua with its environment
		if (script != nullptr && !script->getCode().empty()) {

			// first check entiti table 
			std::string code("_G.ENTITIES." + Component->getTName());
			LuaIntf::LuaRef etable(_klvm, code.c_str());

			// there isn't a table for entity
			// we create it
			if (!etable.isTable()) {
				etable = LuaIntf::LuaRef(_klvm, "_G.ENTITIES");
				etable.set(Component->getTName().c_str(), LuaIntf::LuaRef::createTable(_klvm));
			}

			// create a table for its component
			code.clear();
			code = "_G.ENTITIES." + Component->getTName();
			etable = LuaIntf::LuaRef(_klvm, code.c_str());
			etable.set(Component->getName(), LuaIntf::LuaRef::createTable(_klvm));

			// load chunk and set its environment
			code.clear();
			code.reserve(script->getCode().size());
			code = script->getCode();

			std::string address("ENTITIES." + Component->getTName() + "." + Component->getName());
			code.insert(0, "_ENV = " + address + " \n");
			code.insert(0, address + ".Global = _G \n");
			code.insert(0, address + ".Kite = _G.Kite \n");
			code.insert(0, address + ".Entities = _G.ENTITIES \n");
			code.insert(0, address + ".print = print \n");

			luaL_dostring(_klvm, code.c_str());

			Component->setNeedUpdate(false);
		}
	}

	void KLogicSys::initeComp(KEntity *Self, KLogicCom *Component) {
		// call inite function of component
		std::string address("ENTITIES." + Component->getTName() + "." + Component->getName());
		address.append(".inite");
		LuaIntf::LuaRef ctable(_klvm, address.c_str());
		if (ctable.isFunction()) {
			ctable(&Self);
		}
	}

	void KLogicSys::startComp(KEntity *Self, KLogicCom *Component) {
		// call inite function of component
		std::string address("ENTITIES." + Component->getTName() + "." + Component->getName());
		address.append(".start");
		LuaIntf::LuaRef ctable(_klvm, address.c_str());
		if (ctable.isFunction()) {
			ctable(&Self);
		}
	}

	KMETA_KLOGICSYS_SOURCE();
}