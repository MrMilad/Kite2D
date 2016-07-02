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

#include "Kite\core\kentitymanager.h"
#include <algorithm>
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "kmeta.khgen.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KEntityManager::KEntityManager(){
		registerCTypes(this);
		initeRoot();
	}

	KEntityManager::~KEntityManager() {
		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			it->second->clear();
			delete it->second;
		}
		_kcstorage.clear();
	}

	void KEntityManager::initeRoot() {
		// create root
		_kroot = _kestorage.add(KEntity("Root"));
		_kestorage.get(_kroot)->setHandle(_kroot);

		// set storages
		_kestorage.get(_kroot)->_kcstorage = &_kcstorage;
		_kestorage.get(_kroot)->_kestorage = &_kestorage;
		_kestorage.get(_kroot)->setActive(false);

		// register it to map
		_kentmap.insert({ "Root", _kroot });
	}

	KEntity *KEntityManager::createEntity(const std::string &Name) {
		// check entity name if there is a name
		if (!Name.empty()) {
			auto found = _kentmap.find(Name);

			// entiti is exist, so we return it
			if (found != _kentmap.end()) {
				KD_FPRINT("this name is exist. ename: %s", Name.c_str());
				return getEntity(found->second);
			}
		}

		// create new entity and set its id
		auto hndl = _kestorage.add(KEntity(Name));
		auto ent = _kestorage.get(hndl);
		ent->setHandle(hndl);

		// set storages
		ent->_kcstorage = &_kcstorage;
		ent->_kestorage = &_kestorage;

		// added it to root by default
		getEntity(getRoot())->addChild(hndl);

		// register it's to map
		_kentmap.insert({ Name, hndl });
		
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

		// mark removed entity as deactive and store it in trash list
		ent->setActive(false);
		auto parent = _kestorage.get(ent->getParentHandle());
		parent->remChildIndex(ent->_kplistid);
		_kentmap.erase(ent->getName());
		_ktrash.push_back(Handle);
		recursiveDeleter(Handle);

		// post a message about this action
		KMessage msg;
		msg.setType("ENTITY_REMOVED");
		msg.setData((void *)&Handle, sizeof(U32));
		postMessage(&msg, MessageScope::ALL);
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

	void KEntityManager::recursiveSaveChilds(KEntity *Entity, KPrefab *Prefab) {
		/*for (auto it = Entity->beginChild(); it != Entity->endChild(); ++it) {
			auto ent = _kestorage.get(*it);
			// save entity and its components after that
			std::vector<KComponent *> comps;
			Prefab->_kentities.push_back(*ent);
			Prefab->_kcomponents.push_back(std::vector<Internal::PrefabComp>());
			auto cvec = &Prefab->_kcomponents.back();

			// fixed components
			ent->getFixedComponents(comps);
			for (auto it = comps.begin(); it != comps.end(); ++it) {
				cvec->push_back(Internal::PrefabComp());
				cvec->back().type = (*it)->getType();
				cvec->back().name = (*it)->getName();
				cvec->back().data = (*it);
			}

			// logic components
			ent->getScriptComponents(comps);
			cvec->reserve(cvec->size() + comps.size());
			for (auto it = comps.begin(); it != comps.end(); ++it) {
				cvec->push_back(Internal::PrefabComp());
				cvec->back().type = (*it)->getType();
				cvec->back().name = (*it)->getName();
				cvec->back().data = (*it);
			}
			if (ent->hasChild()) {
				recursiveSaveChilds(ent, Prefab);
			}
		}*/
	}

	bool KEntityManager::createPrefab(const KHandle &EHandle, KPrefab *Prefab){
		Prefab->clear();
		auto ent = _kestorage.get(EHandle);
		if (ent == nullptr) {
			KD_PRINT("an entity with the given handle does not exist.");
			return false;
		}

		// entity information
		Prefab->_kdata.append("local parrent = param.createEntity(""" + ent->getName() + """)\n");

		// components
		Prefab->_kdata.append("local.addCompponents");

		return true;
	}

	bool KEntityManager::loadPrefab(const KPrefab *Prefab, const std::string &Name) {
		return false;
	}

	void KEntityManager::serial(KBaseSerial &Out) const {
		Out << _kroot;
		Out << _kestorage;

		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			it->second->serial(Out);
		}
		Out << _kentmap;
	}

	void KEntityManager::deserial(KBaseSerial &In) {
		In >> _kroot;
		In >> _kestorage;
		for (auto it = _kcstorage.begin(); it != _kcstorage.end(); ++it) {
			it->second->deserial(In);
		}
		In >> _kentmap;

		// inite entities
		for (auto it = _kestorage.begin(); it != _kestorage.end(); ++it) {
			it->_kcstorage = &_kcstorage;
			it->_kestorage = &_kestorage;
		}
	}

	void KEntityManager::recursiveDeleter(KHandle EHandle) {
		auto ent = _kestorage.get(EHandle);

		for (auto it = ent->beginChild(); it != ent->endChild(); ++it) {
			auto child = _kestorage.get((*it));
			if (child->hasChild()) {
				recursiveDeleter((*it));
			}

			_kentmap.erase(child->getName());
			child->setActive(false);
			_ktrash.push_back((*it));
		}
	}

	KMETA_KENTITYMANAGER_SOURCE();
}