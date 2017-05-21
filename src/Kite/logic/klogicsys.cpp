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
#include "Kite/engine/kengine.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/logic/klogiccom.h"
#include "Kite/logic/kscript.h"
#include "Kite/ecs/knode.h"
#include "Kite/engine/kengine.h"
#include <vector>
#ifdef KITE_DEV_DEBUG
#include <exception>
#endif

namespace Kite {
	KLogicSys::KLogicSys() :
		_kupPerFrame(true),
		_kcgarbage(true)
	{}

	void KLogicSys::reset(KEngine *Engine) {
		_klvm = Engine->getLuaState();

		// create refrence to kite events system
		_kevents = LuaIntf::LuaRef(_klvm, "events");
		if (!_kevents.isTable()) {
			throw std::string("events table is corrupted.");
		}

		_ktrigger = LuaIntf::LuaRef(_klvm, "events.trigger");
		if (!_ktrigger.isFunction()) {
			throw std::string("events:trigger function is corrupted.");
		}

		_kremove = LuaIntf::LuaRef(_klvm, "events.removeListener");
		if (!_kremove.isFunction()) {
			throw std::string("events:removeListener function is corrupted.");
		}

		// create nodes table 
		// listeners will added to this table
		auto ntable = LuaIntf::LuaRef::createTable(_klvm, KLOGIC_NODE_ARRAY);
		LuaIntf::LuaRef::globals(_klvm).set("nodes", ntable);
		if (!ntable.isTable()) {
			throw std::string("couldn't create nodes table.");
		}

		// create components table (array)
		_kcoms = LuaIntf::LuaRef::createTable(_klvm, KLOGIC_COM_ARRAY);
		LuaIntf::LuaRef::globals(_klvm).set("coms", _kcoms);
		if (!_kcoms.isTable()) {
			throw std::string("couldn't create components table.");
		}

		// create global variable table (weak table)
		_kglobalVars = LuaIntf::LuaRef::createTable(_klvm);
		auto weakMeta = LuaIntf::LuaRef::createTable(_klvm);
		weakMeta.set("__mode", "v"); // weak table by variable
		_kglobalVars.setMetaTable(weakMeta);
		LuaIntf::LuaRef::globals(_klvm).set("global", _kglobalVars);

		if (!_kglobalVars.isTable()) {
			throw std::string("couldn't create global variables table.");
		}
	}

	bool KLogicSys::update(F64 Delta) {

		// update logic components
		if (_kupPerFrame) {
#ifdef KITE_DEV_DEBUG
			try {
				_ktrigger(nullptr, "onUpdate", Delta);		// calling all update fuctions
				_ktrigger(nullptr, "onPostWork", Delta);	// calling all post-work functions
			}
			catch (std::exception& e) {
				KD_FPRINT("update\\postWork function failed: %s", e.what());
				return false;
			}
#else
			_ktrigger(nullptr, "onUpdate", Delta);
			_ktrigger(nullptr, "onPostWork", Delta);
#endif
		}

		// performs a full garbage-collection cycle. 
		// lua garbage collection will eat memory so we shuld use lua_gc() in a period
		if (_kcgarbage) {
			lua_gc(_klvm, LUA_GCCOLLECT, 0); //> luajit (not lua) seems better at this job
		}

		return true;
	}

	void KLogicSys::componentAdded(KComponent *Com) {
		if (Com->getDrivedType() == Component::LOGIC) {
			auto lcom = static_cast<KLogicCom *>(Com);

			// set callbacks
			lcom->_ksys = this;
			lcom->_kreloadScriptCallb = &KLogicSys::reloadChunk;

			// create/load chunk and set its environment
			reloadChunk(lcom);
		}
	}

	void KLogicSys::componentRemoved(KComponent *Com) {
		if (Com->getDrivedType() == Component::LOGIC) {
			auto lcom = static_cast<KLogicCom *>(Com);

			auto ctable = _kcoms.get(lcom->getTableIndex());
			if (ctable.isTable()) {
#ifdef KITE_DEV_DEBUG
				try {
					if (ctable.has("onRemove")) {
						ctable.get("onRemove")();
					}
				}
				catch (std::exception& e) {
					KD_FPRINT("update\\postWork function failed: %s", e.what());
					return;
				}
#else
				if (ctable.has("onRemove")) {
					ctable.get("onRemove")();
				}
#endif
				ctable = nullptr;

				// remove from array
				_kcoms.remove(lcom->getTableIndex());
			}
		}
	}

	void KLogicSys::nodeAdded(KNode *Node) {
		// remove all logic componentsfrom lua
		std::vector<KComponent *> clist;
		Node->getLogicComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			componentAdded((*it));
		}
	}

	void KLogicSys::nodeRemoved(KNode *Node) {
		// remove all logic components 
		std::vector<KComponent *> clist;
		Node->getLogicComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			componentRemoved((*it));
		}
	}

	void KLogicSys::reloadChunk(KLogicCom *Com) {
		// delete current env
		componentRemoved(Com);

		// reload
		if (!Com->getScript().isNull()) {
			auto script = (KScript *)Com->getScript().operator->();
			if (!script->getCode().empty()) {
				// create component table 
				auto ctable = LuaIntf::LuaRef::createTable(_klvm);
				auto index = _kcoms.rawlen() + 1;
				_kcoms.set(index, ctable);
				Com->_klid = index;

				// set envirunmet values of component table
				auto global = LuaIntf::LuaRef::globals(_klvm);
				ctable.set("print", global.get("print"));
				ctable.set("tostring", global.get("tostring"));
				ctable.set("type", global.get("type"));
				ctable.set("pairs", global.get("pairs"));
				ctable.set("math", global.get("math"));
				ctable.set("string", global.get("string"));
				ctable.set("collectgarbage", global.get("collectgarbage"));// use gc by hand
				ctable.set("setmetatable", global.get("setmetatable"));	// creating weak tables

				ctable.set("global", _kglobalVars);
				ctable.set("k", global.get("k"));
				ctable.set("events", _kevents);
				ctable.set("object", Com);

				// load script code
				if (luaL_loadstring(_klvm, script->getCode().c_str()) != 0) {
					const char *out = lua_tostring(_klvm, -1);
					lua_pop(_klvm, 1);
					KD_FPRINT("lua load string error. cname: %s. %s", Com->getName().c_str(), out);
					return;
				}

				// set chunk envirounment
				int chunkIndex = lua_gettop(_klvm);
				LuaIntf::Lua::push(_klvm, ctable);
				if (lua_setfenv(_klvm, chunkIndex) == 0) {
					const char *out = lua_tostring(_klvm, -1);
					lua_pop(_klvm, 1);
					KD_FPRINT("lua set envirounment error. cname: %s. %s", Com->getName().c_str(), out);
					return;
				}

				// execute and register chunk
#ifdef KITE_DEV_DEBUG
				if (lua_pcall(_klvm, 0, 0, 0)) {
					const char *out = lua_tostring(_klvm, -1);
					lua_pop(_klvm, 1);
					KD_FPRINT("lua set envirounment error. cname: %s. %s", Com->getName().c_str(), out);
					return;
				}
#else
				lua_call(_klvm, 0, 0);
#endif
#ifdef KITE_DEV_DEBUG
				try {
					if (ctable.has("onAdd")) {
						ctable.get("onAdd")();
					}
				}
				catch (std::exception& e) {
					KD_FPRINT("update\\postWork function failed: %s", e.what());
					return;
				}
#else
				if (ctable.has("onAdd")) {
					ctable.get("onAdd")();
				}
#endif
			}
		}
	}

	KLOGICSYS_SOURCE();
}