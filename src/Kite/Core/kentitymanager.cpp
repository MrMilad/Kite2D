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

namespace Kite {
	KEntityManager::KEntityManager() {
		for (U8 i = 0; i < (U8)KComTypes::KCT_MAX_COMP_SIZE; i++) {
			_kcstorage[i] = nullptr;
		}

		// create root
		getEntity(createEntity())->setActive(false);
		getEntity(0)->setCStorage(_kcstorage);
		getEntity(0)->setEStorage(&_kestorage);
	}

	KEntityManager::~KEntityManager() {
		for (U8 i = 0; i < (U8)KComTypes::KCT_MAX_COMP_SIZE; i++) {
			if (_kcstorage[i] != nullptr) {
				delete _kcstorage[i];
				_kcstorage[i] = nullptr;
			}
		}
	}

	U32 KEntityManager::createEntity(const std::string &Name) {
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
		U32 ind = _kestorage.add(KEntity(Name));
		auto ent = _kestorage.get(ind);
		ent->setID(ind);

		// set storages
		ent->setCStorage(_kcstorage);
		ent->setEStorage(&_kestorage);

		// added it to root by default
		ent->setHParrent(false);
		ent->setPID(1); // dummb parrent id
		getEntity(0)->addChild(ind);

		// register it's to map
		_kentmap.insert({ Name, ind });
		
		// post a message about new entity
		KMessage msg;
		msg.setType("ENTITY_CREATED");
		msg.setData((void *)&ind, sizeof(U32));
		postMessage(msg, KMessageScopeTypes::KMS_ALL);

		return ind;
	}

	void KEntityManager::removeEntity(U32 ID) {
		auto ent = _kestorage.get(ID);
		if (ent == nullptr) {
			KD_FPRINT("wrong entity id. eid: %i", ID);
		}

		// remove all childs
		if (ent->hasChild()) {
			for (auto it = ent->beginChild(); it != ent->endChild(); ++it) {
				_kestorage.remove((*it));
			}
		}
		_kestorage.remove(ID);

		// post a message about this action
		KMessage msg;
		msg.setType("ENTITY_REMOVED");
		msg.setData((void *)&ID, sizeof(U32));
		postMessage(msg, KMessageScopeTypes::KMS_ALL);
	}

	void KEntityManager::removeEntity(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			removeEntity(found->second);
		}
	}

	KEntity *KEntityManager::getEntity(U32 ID) {
		auto ent = _kestorage.get(ID);
		if (ent != nullptr) {
			ent->setCStorage(_kcstorage);
			ent->setEStorage(&_kestorage);
		}
		return ent;
	}

	KEntity *KEntityManager::getEntity(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			return getEntity(found->second);
		}
		KD_FPRINT("an entity with the given name does not exist. ename: %s", Name.c_str());
		return nullptr;
	}

	void KEntityManager::unregisterComponent(KComTypes Type) {
		if (_kcstorage[(U8)Type] != nullptr) {
			_kcstorage[(U8)Type]->clear();
			delete _kcstorage[(U8)Type];
			_kcstorage[(U8)Type] = nullptr;
		}
	}

	bool KEntityManager::isRegistered(KComTypes Type) {
		if (_kcstorage[(U8)Type] == nullptr) {
			return false;
		}

		return true;
	}
}