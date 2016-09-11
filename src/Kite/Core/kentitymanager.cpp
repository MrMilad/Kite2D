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

#include "Kite/core/kentitymanager.h"
#include <algorithm>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/core/kcoreutil.h"
#include "kmeta.khgen.h"
#include <luaintf\LuaIntf.h>
#ifdef KITE_DEV_DEBUG
	#include <exception>
#endif

namespace Kite {
	lua_State *KEntityManager::_klstate = nullptr;
	bool KEntityManager::initeLua() {
		static bool isInite = false;
		if (!isInite) {
			_klstate = luaL_newstate();
			static const luaL_Reg lualibs[] =
			{
				{ "base", luaopen_base },
				{ "string", luaopen_string },
				{ NULL, NULL }
			};

			const luaL_Reg *lib = lualibs;
			for (; lib->func != NULL; lib++) {
				lib->func(_klstate);
				lua_settop(_klstate, 0);
			}
			registerKiteMeta(nullptr, _klstate);
			isInite = true;
		}
		assert(isInite == true);
		return isInite;
	}

	KEntityManager::KEntityManager() :
		_knum(0),
		_kzorder(0)
		{
		initeLua();
		_kentmap.reserve(KCFSTORAGE_CHUNK_SIZE);
		initeCStorages();
		registerCTypes(this);
		initeRoot();
	}

	KEntityManager::~KEntityManager() {
		for (auto i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			delete _kcstorage[i];
		}
		_kestorage.clear();
	}

	void KEntityManager::initeCStorages() {
		for (U16 i = 0; i < (U16)CTypes::maxSize; ++i) {
			_kcstorage[i] = nullptr;
		}
	}

	void KEntityManager::initeRoot() {
		// create root
		_kroot = _kestorage.add(KEntity("Root"));
		_kestorage.get(_kroot)->setHandle(_kroot);

		// set storages
		_kestorage.get(_kroot)->_kcstorage = _kcstorage;
		_kestorage.get(_kroot)->_kestorage = &_kestorage;
		_kestorage.get(_kroot)->setActive(false);

		// register it to map
		_kentmap.insert({ "Root", _kroot });

		// do not add root to default layer
	}

	KEntity *KEntityManager::createEntity(const std::string &Name) {
		// we generate a name for nameless entites
		std::string ename = Name;
		if (Name.empty()) {
			while (true) {
				ename = "ent" + std::to_string(_knum++);
				if (_kentmap.find(ename) == _kentmap.end()) {
					break;
				}
			}

		// or check name for an existing entity
		} else {
			// entity is exist, so we return it
			auto found = _kentmap.find(Name);
			if (found != _kentmap.end()) {
				KD_FPRINT("this name is exist. ename: %s", Name.c_str());
				return getEntity(found->second);
			}
		}

		// create new entity and set its id
		auto hndl = _kestorage.add(KEntity(ename));
		auto ent = _kestorage.get(hndl);
		ent->setHandle(hndl);
		ent->_kzorder = _kzorder; /// order will increased by 1
		++_kzorder;

		// set storages
		ent->_kcstorage = _kcstorage;
		ent->_kestorage = &_kestorage;

		// added it to root by default
		getEntity(getRoot())->addChild(hndl);

		// register it's to map
		_kentmap.insert({ ename, hndl });
		
		// post a message about new entity
		KMessage msg;
		msg.setType("ENTITY_CREATED");
		msg.setData((void *)&hndl, sizeof(U32));
		postMessage(&msg, MessageScope::ALL);
	
		return ent;
	}

	bool KEntityManager::renameEntity(const KHandle &EHandle, const std::string &NewName) {
		auto ent = _kestorage.get(EHandle);
		if (ent == nullptr) {
			KD_PRINT("invalid handle");
			return false;
		}

		if (EHandle == _kroot) {
			KD_PRINT("renaming root is not allowed.");
			return false;
		}

		// check new name is available
		if (NewName.empty()) {
			KD_PRINT("empty name as new name is not allowed.");
			return false;
		}

		auto found = _kentmap.find(NewName);
		// entiti is already registered, so we return it
		if (found != _kentmap.end()) {
			KD_FPRINT("this name is already exist. ename: %s", NewName.c_str());
			return false;
		}

		// insert new name to map
		_kentmap.insert({ NewName, ent->getHandle() });

		// remove old name
		_kentmap.erase(ent->getName());

		// change entity name
		ent->_kname = NewName;

		return true;
	}

	void KEntityManager::removeEntity(KHandle Handle) {
		auto ent = _kestorage.get(Handle);
		if (ent == nullptr) {
			KD_PRINT("invalid handle");
			return;
		}

		if (Handle == _kroot) {
			KD_PRINT("removing root is not allowed.");
			return;
		}

		// deattach entity from its parent
		auto parent = _kestorage.get(ent->getParentHandle());
		parent->remChildIndex(ent->_kplistid);

		recursiveDeleter(Handle);
	}

	void KEntityManager::removeEntityByName(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			removeEntity(found->second);
		}
	}

	KEntity *KEntityManager::getEntity(const KHandle &Handle) {
		auto ent = _kestorage.get(Handle);
		if (ent != nullptr) {
			ent->_kcstorage = _kcstorage;
			ent->_kestorage = &_kestorage;
		}
		return ent;
	}

	KEntity *KEntityManager::getEntityByName(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			return getEntity(found->second);
		}
		KD_FPRINT("an entity with the given name does not exist. ename: %s", Name.c_str());
		return nullptr;
	}

	// we dont need unregistering an components type (error prone)
	/*void KEntityManager::unregisterComponent(KCTypes Type) {
		delete _kcstorage[(SIZE)Type];
		_kcstorage[(SIZE)Type] = nullptr;
	}*/

	bool KEntityManager::isRegisteredComponent(CTypes Type) {
		if (_kcstorage[(SIZE)Type] != nullptr){
			return true;
		}
		return false;
	}

	void KEntityManager::postWork() {
		// iterarte over all handle in our trash list
		for (auto it = _ktrash.begin(); it != _ktrash.end(); ++it) {
			auto ent = _kestorage.get((*it));

			if (ent != nullptr) {
				ent->clearComponents(); 
				_kestorage.remove((*it));
			}
		}

		// clear trash list
		_ktrash.clear();
	}

	U32 KEntityManager::recursiveSaveChilds(KEntity *Entity, KPrefab *Prefab, U32 Level, bool Name, bool CopyPrefab) {
		auto id = Level;
		// entity information
		if (Name) {
			Prefab->_kcode.append("local ent = eman:createEntity(\"" + Entity->getName() + "\")\n");
		} else {
			Prefab->_kcode.append("local ent = eman:createEntity(\"\")\n");
		}

		if (CopyPrefab && Entity->isPrefab()) {
			Prefab->_kcode.append("ent:setPrefabName(\"" + Entity->getPrefab() + "\")\n");
		}

		Prefab->_kcode.append("local hndl" + std::to_string(id) + " = ent.handle\n");

		// logic components
		std::vector<KHandle> lclist;
		Entity->getScriptComponents(lclist);
		for (auto it = lclist.begin(); it != lclist.end(); ++it) {
			auto lcomp = Entity->getComponentByHandle((*it));
			// components
			Prefab->_kcode.append("local comp = kite.Logic.toLogic(ent:addComponent(kite.CTypes.Logic, \"" + lcomp->getName() + "\"))\n");
			Prefab->_kcode.append("comp:deserial(ser, false)\n");
			lcomp->serial(Prefab->_kdata, false);
		}

		// other components
		std::vector<KHandle> clist;
		Entity->getFixedComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			auto com = _kcstorage[it->type]->get((*it));
			// components
			Prefab->_kcode.append("comp = kite." + com->getTypeName() + ".to" + com->getTypeName() +
								  "(ent:addComponent(kite.CTypes." + com->getTypeName() + ", \"" + com->getName() + "\"))\n");

			// transform data not saved with prefab
			if (!CopyPrefab) {
				if (com->getType() == CTypes::Transform) continue;
			}

			Prefab->_kcode.append("comp:deserial(ser, false)\n");
			com->serial(Prefab->_kdata, false);
		}

		for (auto it = Entity->childList()->cbegin(); it != Entity->childList()->cend(); ++it) {
			auto cid = recursiveSaveChilds(getEntity((*it)), Prefab, Level + 1, Name, CopyPrefab);
			Prefab->_kcode.append("local par = eman:getEntity(hndl" + std::to_string(id) + ")\n");
			Prefab->_kcode.append("par:addChild(hndl" + std::to_string(cid) + ")\n");
		}

		return id;
	}

	bool KEntityManager::createPrefab(const KHandle &EHandle, KPrefab *Prefab, bool SaveName){
		Prefab->clear();
		auto ent = _kestorage.get(EHandle);
		if (ent == nullptr) {
			KD_PRINT("an entity with the given handle does not exist.");
			return false;
		}

		// lua function 
		Prefab->_kcode.append("function execute(eman, ser)\n");

		// recursive save
		auto root = recursiveSaveChilds(ent, Prefab, 0, SaveName, false);

		// end function
		Prefab->_kcode.append("return hndl" + std::to_string(root) + "\nend\n");
		Prefab->_kisempty = false;
		return true;
	}

	KHandle KEntityManager::loadPrefabRaw(KPrefab *Prefab, bool isPaste) {
		// check prefab
		if (Prefab->_kcode.empty()) {
			KD_PRINT("prefab is empty or not loaded yet.");
			return KHandle();
		}

		// inite read position for reading data
		Prefab->initeLoad();

		// setup script with entity name
		if (luaL_dostring(_klstate, Prefab->_kcode.c_str())) {
			const char *out = lua_tostring(_klstate, -1);
			lua_pop(_klstate, 1);
			KD_FPRINT("prefab script error on load. %s", out);
			return KHandle();
		}
		LuaIntf::LuaRef lref(_klstate, "_G.execute");
		KHandle handle;
		if (lref.isFunction()) {
#if defined(KITE_DEV_DEBUG)
			try {
				handle = lref.call<KHandle>(this, (KBaseSerial *)&Prefab->_kdata);
			} catch (std::exception& e) {
				KD_FPRINT("prefab execute function failed. %s", e.what());
				return KHandle();
			}
#else
			handle = lref.call<KHandle>(this, (KBaseSerial *)&Prefab->_kdata);
#endif
		} else {
			KD_PRINT("invalid execte function in prefab script.");
			return KHandle();
		}

		// set as prefab instance
		if (!isPaste) {
			auto ent = getEntity(handle);
			ent->setPrefabName(Prefab->getName());
		}

		return handle;
	}

	KHandle KEntityManager::loadPrefab(KPrefab *Prefab) {
		return loadPrefabRaw(Prefab, false);
	}

#ifdef KITE_EDITOR
	bool KEntityManager::copyPrefab(const KHandle &EHandle, KPrefab *Prefab, bool Name) {
		Prefab->clear();
		auto ent = _kestorage.get(EHandle);
		if (ent == nullptr) {
			KD_PRINT("an entity with the given handle does not exist.");
			return false;
		}

		// lua function 
		Prefab->_kcode.append("function execute(eman, ser)\n");

		// recursive save
		auto root = recursiveSaveChilds(ent, Prefab, 0, Name, true);

		// end function
		Prefab->_kcode.append("return hndl" + std::to_string(root) + "\nend\n");
		Prefab->_kisempty = false;
		return true;
	}

	KHandle KEntityManager::pastePrefab(KPrefab *Prefab) {
		return loadPrefabRaw(Prefab, true);
	}
#endif

	void KEntityManager::serial(KBaseSerial &Out) const {
		Out << _kzorder;
		Out << _knum;
		Out << _kroot;
		Out << _kestorage;

		for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			_kcstorage[i]->serial(Out);
		}
		Out << _kentmap;
	}

	void KEntityManager::deserial(KBaseSerial &In) {
		In >> _kzorder;
		In >> _knum;
		In >> _kroot;
		In >> _kestorage;
		for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			_kcstorage[i]->deserial(In);
		}
		In >> _kentmap;

		// inite entities 
		for (auto it = _kestorage.getContiner()->begin(); it != _kestorage.getContiner()->end(); ++it) {
			it->_kcstorage = _kcstorage;
			it->_kestorage = &_kestorage;
			it->initeComponents();
		}
	}

	void KEntityManager::recursiveDeleter(KHandle EHandle) {
		auto ent = _kestorage.get(EHandle);

		// mark removed entity as deactive and store it in trash list
		ent->_kdeleted = true; // we use direct access because we are in recursive mode and all childs will be marked
		ent->_kactive = false; // we use direct access because we are in recursive mode and all childs will be marked
		_kentmap.erase(ent->getName());
		_ktrash.push_back(EHandle);

		// post a message about this action
		KMessage msg;
		msg.setType("ENTITY_REMOVED");
		msg.setData((void *)&EHandle, sizeof(U32));
		postMessage(&msg, MessageScope::ALL);

		for (auto it = ent->childList()->cbegin(); it != ent->childList()->cend(); ++it) {
			recursiveDeleter((*it));
		}
	}

	KMETA_KENTITYMANAGER_SOURCE();
}