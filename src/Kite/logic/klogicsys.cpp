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
#include "Kite/logic/klogicdef.h"
#include "Kite/logic/klogicsys.h"
#include <vector>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/logic/kreglogiccom.h"
#include "Kite/logic/klogiccom.h"
#include "Kite/engine/kengine.h"
#include <luaintf/LuaIntf.h>
#ifdef KITE_DEV_DEBUG
#include <exception>
#endif

namespace Kite {
	bool KLogicSys::update(F64 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		// iterate over objects with at least 1 Logic component
		auto continer = EManager->getComponentStorage<KRegLogicCom>(CTypes::RegisterLogic);
		bool haveReg = false;
		for (auto it = continer->begin(); it != continer->end(); ++it) {
			haveReg = true;
			auto ent = EManager->getEntity(it->getOwnerHandle());
			// iterate over all new logic components and inite them
			for (auto comp = it->_kiniteList.begin(); comp != it->_kiniteList.end(); ++comp) {
				auto lcomp = (KLogicCom *)ent->getComponentByHandle((*comp));

				// inite component and bind it to lua vm (only one time when current script changed with a new script)
				if (lcomp->getResNeedUpdate()) {
					if (!catchAndRegist(lcomp, RManager)) return false;
				}

				// inite component (calling inite, only 1 time befor start)
				if (!initeComp(ent, lcomp)) return false;
			}
		}

		// clear storage after registration
		if (haveReg) EManager->clearComponentStorage<KRegLogicCom>(CTypes::RegisterLogic);

		// update components (per frame)
		if (!updateAll(Delta)) return false;

		// performs a full garbage-collection cycle. 
		// lua garbage collection will eat memory so we shuld use lua_gc() in a period
		lua_gc(_klvm, LUA_GCCOLLECT, 0);

		// clear trash list and post works
		EManager->postWork();
		return true;
	}

	bool KLogicSys::inite(void *Data) {
		auto engien = static_cast<KEngine *>(Data);
		if (Data != nullptr && !isInite()) {
			_klvm = static_cast<lua_State *>(engien->getLuaState());

			auto table = LuaIntf::LuaRef::createTable(_klvm);
			LuaIntf::LuaRef(_klvm, "_G").set("ENT", table);

			// implement hook system
			const std::string updateFunc(KLUA_HOOK);
			if (luaL_dostring(_klvm, updateFunc.c_str()) != 0) {
				const char *out = lua_tostring(_klvm, -1);
				lua_pop(_klvm, 1);
				KD_FPRINT("lua load string error: %s", out);
				return false;
			}

			// create logic hooks
			table = LuaIntf::LuaRef::createTable(_klvm);
			LuaIntf::LuaRef(_klvm, "_G.hooks.funcs").set("onGameMessage", table);

			table = LuaIntf::LuaRef::createTable(_klvm);
			LuaIntf::LuaRef(_klvm, "_G.hooks.funcs").set("onUpdate", table);

			setInite(true);
			return true;
		}

		return isInite();
	}

	void KLogicSys::destroy() { setInite(false); }

	bool KLogicSys::catchAndRegist(KLogicCom *Component, KResourceManager *RManager) {
		// retrive script rsource from resource manager
		Component->updateRes();
		if (!Component->_kscript) {
			KD_FPRINT("can't load script resource. cname: %s", Component->getName().c_str());
			return false;
		}

		Component->setLuaState(_klvm);

		// bind it to lua with its environment
		if (!Component->_kscript->getCode().empty()) {

			// first check entiti table 
			std::string code("_G.ENT." + Component->getTName());
			LuaIntf::LuaRef etable(_klvm, code.c_str());

			// there isn't a table for entity
			// we create it
			if (!etable.isTable()) {
				etable = LuaIntf::LuaRef::createTable(_klvm);
				LuaIntf::LuaRef(_klvm, "_G.ENT").set(Component->getTName().c_str(), etable);
			}

			// create a table for its component
			code.clear();
			code = "_G.ENT." + Component->getTName();
			auto ctable = LuaIntf::LuaRef::createTable(_klvm);
			LuaIntf::LuaRef(_klvm, code.c_str()).set(Component->getName(), ctable);

			// set envirunmet values
			ctable["global"] = LuaIntf::LuaRef::globals(_klvm);
			ctable["kite"] = LuaIntf::LuaRef(_klvm, "_G.kite");
			ctable["self"] = Component->getOwnerHandle();

			// load chunk and set its environment
			code.clear();
			code.reserve(Component->_kscript->getCode().size());
			code = Component->_kscript->getCode();

			std::string address("ENT." + Component->getTName() + "." + Component->getName());

			if (luaL_loadstring(_klvm, code.c_str()) != 0) {
				const char *out = lua_tostring(_klvm, -1);
				lua_pop(_klvm, 1);
				KD_FPRINT("lua load string error. cname: %s. %s", Component->getTName().c_str(), out);
				return false;
			}

			// set chunk envirounment
			auto chunk = lua_gettop(_klvm);
			LuaIntf::Lua::pushGlobal(_klvm, address.c_str());
			if (lua_setfenv(_klvm, chunk) == 0) {
				const char *out = lua_tostring(_klvm, -1);
				lua_pop(_klvm, 1);
				KD_FPRINT("lua set envirounment error. cname: %s. %s", Component->getTName().c_str(), out);
				return false;
			}

			// execute and register chunk
			if (lua_pcall(_klvm, 0, 0, 0)) {
				const char *out = lua_tostring(_klvm, -1);
				lua_pop(_klvm, 1);
				KD_FPRINT("lua set envirounment error. cname: %s. %s", Component->getTName().c_str(), out);
				return false;
			}
		}
		return true;
	}

	bool KLogicSys::initeComp(KEntity *Self, KLogicCom *Comp) {
		// call inite function of component
		std::string address("ENT." + Comp->getTName() + "." + Comp->getName());
		address.append(".inite");

#ifdef KITE_DEV_DEBUG
		std::string tname(Comp->getTName());	// copy tname because we need it for showing message in the catch section
												// and we can't use a string refrence in that section
		std::string ename(Self->getName());
#endif

		LuaIntf::LuaRef ctable(_klvm, address.c_str());
		if (ctable.isFunction()) {
#ifdef KITE_DEV_DEBUG
		try{
			ctable();
		} catch (std::exception& e) {
			KD_FPRINT("inite function failed. ename: %s. cname: %s. %s", ename.c_str(), tname.c_str(), e.what());
			return false;
		}
#else
			ctable(Self);
#endif
		}
		return true;
	}

	bool KLogicSys::updateAll(F64 Delta) {
		LuaIntf::LuaRef ctable(_klvm, "_G.hooks.call");
		if (ctable.isFunction()) {
#ifdef KITE_DEV_DEBUG
			try {
				ctable("onUpdate", Delta);
			} catch (std::exception& e) {
				KD_FPRINT("update function failed: %s", e.what());
				return false;
			}
#else
			_kupdate(Delta);
#endif
		} else {
			KD_PRINT("engine hooks system is corrupted");
			return false;
		}

		return true;
	}

	KMETA_KLOGICSYS_SOURCE();
}