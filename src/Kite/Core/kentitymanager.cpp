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
#include <luaintf\LuaIntf.h>

namespace Kite {
	KEntityManager::KEntityManager():
		_kcompCount(0)
	{
		for (U8 i = 0; i < KCOMP_MAX_SIZE; ++i) {
			_kcstorage[i] = nullptr;
		}

		// create root
		_kroot = _kestorage.add(KEntity("Root"));
		_kestorage.get(_kroot)->_khandle = _kroot;

		// set storages
		_kestorage.get(_kroot)->_kcstorage = _kcstorage;
		_kestorage.get(_kroot)->_kestorage = &_kestorage;
		_kestorage.get(_kroot)->setActive(false);

		// register it's to map
		_kentmap.insert({ "Root", _kroot });
	}

	KEntityManager::~KEntityManager() {
		for (U8 i = 0; i < KCOMP_MAX_SIZE; ++i) {
			if (_kcstorage[i] != nullptr) {
				_kcstorage[i]->clear();
				delete _kcstorage[i];
				_kcstorage[i] = nullptr;
			}
		}
	}

	KHandle KEntityManager::createEntity(const std::string &Name) {
		// check entity name if there is a name
		if (!Name.empty()) {
			auto found = _kentmap.find(Name);

			// entiti is already registered, so we return it
			if (found != _kentmap.end()) {
				KD_FPRINT("this name has already been registered. ename: %s", Name.c_str());
				return found->second;
			}
		}

		// create new entity and set its id
		auto hndl = _kestorage.add(KEntity(Name));
		auto ent = _kestorage.get(hndl);
		ent->_khandle = hndl;

		// set storages
		ent->_kcstorage = _kcstorage;
		ent->_kestorage = &_kestorage;
		ent->_kctypes = &_kctypes;

		// added it to root by default
		getEntity(getRoot())->addChild(hndl);

		// register it's to map
		_kentmap.insert({ Name, hndl });
		
		// post a message about new entity
		KMessage msg;
		msg.setType("ENTITY_CREATED");
		msg.setData((void *)&hndl, sizeof(U32));
		postMessage(msg, KMessageScopeTypes::KMS_ALL);
	
		return hndl;
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
		auto parrent = _kestorage.get(ent->getParrentHandle());
		parrent->remChildIndex(ent->_kplistid);
		_kentmap.erase(ent->getName());
		_ktrash.push_back(Handle);
		recursiveDeleter(Handle);

		// post a message about this action
		KMessage msg;
		msg.setType("ENTITY_REMOVED");
		msg.setData((void *)&Handle, sizeof(U32));
		postMessage(msg, KMessageScopeTypes::KMS_ALL);
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
			ent->_kctypes = &_kctypes;
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
		auto found = _kctypes.find(CType);
		if (found != _kctypes.end()) {
			if (_kcstorage[found->second] != nullptr) {
				_kcstorage[found->second]->clear();
				delete _kcstorage[found->second];
				_kcstorage[found->second] = nullptr;
				_kctypes.erase(CType);
			}
		}
	}

	bool KEntityManager::isModified() {
		if (_kestorage.getModified()) {
			return true;
		}

		for (SIZE i = 0; i < KCOMP_MAX_SIZE; ++i) {
			if (_kcstorage[i] != nullptr) {
				if (_kcstorage[i]->getModified()) {
					return true;
				}
			}
		}

		return false;
	}

	bool KEntityManager::isRegisteredComponent(const std::string &CType) {
		auto found = _kctypes.find(CType);
		if (found != _kctypes.end()){
			if (_kcstorage[found->second] != nullptr) {
				return true;
			}
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

	void KEntityManager::serial(KBaseSerial &Out) const {
		Out << _kroot;
		Out << _kestorage;

		for (U32 i = 0; i < KCOMP_MAX_SIZE; ++i) {
			if (_kcstorage[i] != nullptr) {
				_kcstorage[i]->serial(Out);
			}
		}
		Out << _kentmap;
		Out << _kctypes;
		Out << _kcompCount;
	}

	void KEntityManager::deserial(KBaseSerial &In) {
		In >> _kroot;
		In >> _kestorage;

		for (U32 i = 0; i < KCOMP_MAX_SIZE; ++i) {
			if (_kcstorage[i] != nullptr) {
				_kcstorage[i]->deserial(In);
			}
		}
		In >> _kentmap;
		In >> _kctypes;
		In >> _kcompCount;
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