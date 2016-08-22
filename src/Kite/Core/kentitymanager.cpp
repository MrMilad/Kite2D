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
		return isInite;
	}

	KEntityManager::KEntityManager() :
		_knum(0),
		_kzorder(0)
		{
		KD_ASSERT(initeLua());
		_kentmap.reserve(KCFSTORAGE_CHUNK_SIZE);
		registerCTypes(this);
		initeRoot();
	}

	KEntityManager::~KEntityManager() {
		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			//it->second->clear();
			delete it->second;
		}
		_kcstorage.clear();
		_kestorage.clear();
	}

	void KEntityManager::initeRoot() {
		// built-in layers
		auto layer = &_klayermap.insert({ "unlayered",{ KLayerInfo("unlayered", true), std::vector<KHandle>() } }).first->second.second;
		layer->reserve(KCFSTORAGE_CHUNK_SIZE);

		// create root
		_kroot = _kestorage.add(KEntity("Root"));
		_kestorage.get(_kroot)->setHandle(_kroot);

		// set storages
		_kestorage.get(_kroot)->_kcstorage = &_kcstorage;
		_kestorage.get(_kroot)->_kestorage = &_kestorage;
		_kestorage.get(_kroot)->setActive(false);

		// register it to map
		_kentmap.insert({ "Root", _kroot });

		// do not add root to default layer
	}

	bool KEntityManager::createLayer(const std::string &Name) {
		if (Name.empty()) {
			KD_PRINT("empty name is not allowed for layers");
			return false;
		}

		// layer is exist?
		auto found = _klayermap.find(Name);
		if (found != _klayermap.end()) {
			KD_FPRINT("this layer is exist. name: %s", Name.c_str());
			return false;
		}

		// create layer
		_klayermap.insert({ Name, {KLayerInfo(Name, false), std::vector<KHandle>() } }).first->second.first;

		return true;
	}

	void KEntityManager::removeLayer(const std::string &Name) {
		auto found = _klayermap.find(Name);
		auto unlayered = _klayermap.find("unlayered");
		if (found != _klayermap.end()) {
			if (found->second.first.isBuiltin) {
				KD_FPRINT("removing built-it layers is not allowed. name: %s", Name.c_str());
				return;
			}

			// move all objects to 'unlayered' layer
			unlayered->second.second.reserve(unlayered->second.second.size() + found->second.second.size());
			for (auto it = found->second.second.begin(); it != found->second.second.end(); ++it) {
				_kestorage.get((*it))->_klayerName = unlayered->second.first.name;
				unlayered->second.second.push_back((*it));
			}
			_klayermap.erase(found);
		}
	}

	const std::vector<KLayerInfo> &KEntityManager::getLayersInfo(){
		static std::vector<KLayerInfo> ilist;
		ilist.clear();
		ilist.reserve(_klayermap.size());
		
		for (auto it = _klayermap.begin(); it != _klayermap.end(); ++it) {
			ilist.push_back(it->second.first);
		}

		return ilist;
	}

	void KEntityManager::addEntityToLayer(const KHandle &Entity, const std::string &Layer) {
		auto newlayer = _klayermap.find(Layer);
		if (newlayer == _klayermap.end()) {
			KD_FPRINT("there is no layer with the given name. layer name: %s", Layer.c_str());
			return;
		}

		auto ent = _kestorage.get(Entity);

		// equal layer
		if (newlayer->second.first.name.hash == ent->_klayerName.hash) {
			KD_FPRINT("this entity is currently inside the given layer. entity name: %s", ent->getName().c_str());
			return;
		}

		// deattach from current layer
		if (!ent->_klayerName.str.empty()) {
			removeCurrentLayer(ent);
		}

		// attach to new layer
		newlayer->second.second.push_back(Entity);
		ent->_klayerName = newlayer->second.first.name;
		ent->_klayerid = newlayer->second.second.size() - 1;
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
		ent->_kcstorage = &_kcstorage;
		ent->_kestorage = &_kestorage;

		// added it to root by default
		getEntity(getRoot())->addChild(hndl);

		// register it's to map
		_kentmap.insert({ ename, hndl });

		// add it to default layer
		addEntityToLayer(hndl, "unlayered");
		
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
			ent->_kcstorage = &_kcstorage;
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

	void KEntityManager::unregisterComponent(const std::string &CType) {
		auto found = _kcstorage.find(CType);
		if (found != _kcstorage.end()) {
			found->second->clear();
			delete found->second;
			_kcstorage.erase(found);
		}
	}

	bool KEntityManager::isModified() const {
		if (_kestorage.getModified()) {
			return true;
		}

		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			if (it->second->getModified()) {
				return true;
			}
		}

		return false;
	}

	bool KEntityManager::isRegisteredComponent(const std::string &CType) {
		if (_kcstorage.find(CType) != _kcstorage.end()){
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
			auto lcomp = Entity->getComponent("Logic", (*it));
			// components
			Prefab->_kcode.append("local comp = kite.Logic.toLogic(ent:addComponent(\"Logic\", \"" + lcomp->getName() + "\"))\n");
			Prefab->_kcode.append("comp:deserial(ser, false)\n");
			lcomp->serial(Prefab->_kdata, false);
		}

		// other components
		std::vector<KComponent *> clist;
		Entity->getFixedComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			// components
			Prefab->_kcode.append("comp = kite." + (*it)->getType() + ".to" + (*it)->getType() +
								  "(ent:addComponent(\"" + (*it)->getType() + "\", \"" + (*it)->getName() + "\"))\n");

			// transform data not saved with prefab
			if (!CopyPrefab) {
				if ((*it)->getType() == "Transform") continue;
			}

			Prefab->_kcode.append("comp:deserial(ser, false)\n");
			(*it)->serial(Prefab->_kdata, false);
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
		luaL_dostring(_klstate, Prefab->_kcode.c_str());
		LuaIntf::LuaRef lref(_klstate, "_G.execute");
		KHandle handle;
		if (lref.isFunction()) {
#if defined(KITE_DEV_DEBU)
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

		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			it->second->serial(Out);
		}
		Out << _kentmap;
		Out << _klayermap;
	}

	void KEntityManager::deserial(KBaseSerial &In) {
		In >> _kzorder;
		In >> _knum;
		In >> _kroot;
		In >> _kestorage;
		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			it->second->deserial(In);
		}
		In >> _kentmap;
		In >> _klayermap;

		// inite entities 
		for (auto it = _kestorage.getContiner()->begin(); it != _kestorage.getContiner()->end(); ++it) {
			it->_kcstorage = &_kcstorage;
			it->_kestorage = &_kestorage;
		}
	}

	void KEntityManager::removeCurrentLayer(KEntity *Entity) {
		auto oldlayer = _klayermap.find(Entity->_klayerName.str);
		auto oldlist = &oldlayer->second.second;

		// swap with last item in continer and remove
		auto lastEnt = _kestorage.get(oldlist->back());
		lastEnt->_klayerid = Entity->_klayerid;

		(*oldlist)[Entity->_klayerid] = oldlist->back();
		oldlist->pop_back();

		Entity->_klayerid = 0;
		Entity->_klayerName.str.clear();
		Entity->_klayerName.hash = 0;
	}

	void KEntityManager::recursiveDeleter(KHandle EHandle) {
		auto ent = _kestorage.get(EHandle);

		// remove entity from its layer
		removeCurrentLayer(ent);

		// mark removed entity as deactive and store it in trash list
		ent->setActive(false);
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