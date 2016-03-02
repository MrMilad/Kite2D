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
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; i++) {
			_kcontiners[i] = nullptr;
		}
	}

	KEntityManager::~KEntityManager() {
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; i++) {
			if (_kcontiners[i] != nullptr) {
				delete _kcontiners[i];
				_kcontiners[i] = nullptr;
			}
		}
	}

	KEntity *KEntityManager::createEntity(const std::string &Name) {
		// check entity name
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			KDEBUG_PRINT("this name has already been registered.");
			return &found->second;
		}

		// create new entity 
		auto ent = &_kentmap.insert({ Name, KEntity(Name) }).first->second;
		
		// post a message about new entity
		KMessage msg;
		msg.setType("ENTITY_CREATED");
		msg.setData((void *)&ent->getName(), ent->getName().size());
		postMessage(msg, KMessageScopeTypes::KMS_ALL);

		return ent;
	}

	void KEntityManager::removeEntity(KEntity *Entity) {
		// at first remove all associated components 
		removeAllComponent(Entity);

		// post a message about this action
		KMessage msg;
		msg.setType("ENTITY_REMOVED");
		msg.setData((void *)&Entity->getName(), Entity->getName().size());
		postMessage(msg, KMessageScopeTypes::KMS_ALL);

		// remove it from map
		_kentmap.erase(Entity->getName());
	}

	void KEntityManager::removeEntity(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			removeEntity(&found->second);
		}
	}

	KEntity *KEntityManager::getEntity(const std::string &Name) {
		auto found = _kentmap.find(Name);
		if (found != _kentmap.end()) {
			return &found->second;
		}
		KDEBUG_PRINT("there is not an entity with the given name");
		return nullptr;
	}

	void KEntityManager::removeAllComponent(KEntity *Entity) {
		// remove all script components
		if (_kcontiners[(U8)KComponentTypes::KCT_LOGIC] != nullptr) {
			const std::vector<U32> &sind = Entity->getScriptComponentIndex();
			for (U32 i = 0; i < sind.size(); i++) {

				// call remove on all components
				KComponent *comPtr = static_cast<KComponent *>(_kcontiners[(U8)KComponentTypes::KCT_LOGIC]->get(sind[i]));
				comPtr->remove(Entity->getName());

				_kcontiners[(U8)KComponentTypes::KCT_LOGIC]->remove(sind[i]);
			}
		}

		// remove fixed components
		for (U8 i = 0; i < (U8)KComponentTypes::KCT_MAX_COMP_SIZE; i++) {
			if (i == (U8)KComponentTypes::KCT_LOGIC) {
				continue;
			}
			if (_kcontiners[i] != nullptr) {
				if (Entity->hasComponent((KComponentTypes)i, "")) {
					U32 ind = Entity->getComponentIndex((KComponentTypes)i, "");

					// call remove on all components
					KComponent *comPtr = static_cast<KComponent *>(_kcontiners[i]->get(ind));
					comPtr->remove(Entity->getName());

					_kcontiners[i]->remove(ind);
				}
			}
		}

		Entity->clear();
	}

	void KEntityManager::removeAllComponent(const std::string &EntityName) {
		auto found = _kentmap.find(EntityName);
		if (found != _kentmap.end()) {
			removeAllComponent(&found->second);
		}
	}
}