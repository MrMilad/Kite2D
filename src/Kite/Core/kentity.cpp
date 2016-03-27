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
#include "Kite/core/kentity.h"
#include "Kite/core/kcoreutil.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KEntity::KEntity(const std::string &Name):
		_kpid(0), _kplistid(0), _kid(0),_kname(Name), _khparrent(false),
		_kcstorage(nullptr), _kestorage(nullptr)
	{
		for (U8 i = 0; i < (U8)KComTypes::KCT_MAX_COMP_SIZE; ++i) {
			_kfixedComp[i] = -1;
		}
	}

	KEntity::~KEntity() {}

	KRecieveTypes KEntity::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		// redirect message
		postMessage(Message, Scope);

		return KRecieveTypes::KMR_RECEIVED;
	}

	void KEntity::setComponent(KComTypes Type, const std::string &Name, U32 Index) {
		if (Type == KComTypes::KCT_LOGIC) {
			_kscriptComp[Name] = Index;
		} else {
			_kfixedComp[(U8)Type] = Index;
		}
	}

	void KEntity::remChildIndex(U32 CIndex) {
		if (CIndex != (_kchilds.size() - 1)) {
			_kestorage->get(_kchilds.back())->setPListID(CIndex);
			_kchilds[CIndex] = _kchilds.back();
		}

		_kchilds.pop_back();
	}

	void KEntity::addChild(U32 EntityID) {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}

		auto Child = _kestorage->get(EntityID);
		if (Child == nullptr) {
			KD_FPRINT("wrong entity id. eid: %i", EntityID);
			return;

		} else if (Child->getParrentID() == getID()) {
			return;

		} else if (EntityID == getID()) {
			KD_FPRINT("an entity cant be a child of himself. eid: %i", EntityID);
			return;
		}

		// deattach from old parrents
		if (Child->hasParrent()) {
			auto parrent = _kestorage->get(Child->getParrentID());
			parrent->remChildIndex(Child->getPListID());
		}

		// attach to new parrent
		Child->setPID(getID());
		Child->setPListID(_kchilds.size());
		Child->setHParrent(true);
		_kchilds.push_back(EntityID);
	}

	void KEntity::removeChild(U32 EntityID) {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}
		
		auto Child = _kestorage->get(EntityID);
		if (Child == nullptr) {
			KD_FPRINT("wrong entity id. eid: %i", EntityID);
			return;

		} else if (Child->getParrentID() != getID()) {
			KD_FPRINT("this entity is not in the children list. eid: %i", EntityID);
			return;
		}

		// add it to root
		_kestorage->get(0)->addChild(EntityID);
	}

	U32 KEntity::addComponent(KComTypes Type, const std::string &ComponentName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return 0;

		// check component type
		} else if (_kcstorage[(U8)Type] == nullptr) {
			KD_FPRINT("unregistered component types. cname: %s\tctype: %i", ComponentName.c_str(), (int)Type);
			return 0;

		// this component is already exist
		// we just return it
		} else if (hasComponent(Type, ComponentName)) {
			return getComponentIndex(Type, ComponentName);
		}

		// create component
		U32 ind = _kcstorage[(U8)Type]->add(ComponentName);

		// set its index
		auto com = _kcstorage[(U8)Type]->get(ind);
		com->setID(ind);

		// attach the created component to the entity and call attach functon
		setComponent(Type, ComponentName, ind);
		com->attached(getID());

		return ind;
	}

	void KEntity::clearChilds() {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}

		for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
			_kestorage->get(0)->addChild((*it));
		}
	}

	bool KEntity::hasChild() const {
		if (_kchilds.empty()) {
			return false;
		}

		return true;
	}

	KComponent *KEntity::getComponent(KComTypes Type, const std::string &ComponentName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		}else if (_kcstorage[(U8)Type] == nullptr) {
			KD_FPRINT("unregistered component types. cname: %s\tctype: %i", ComponentName.c_str(), (int)Type);
			return nullptr;

		} else if (!hasComponent(Type, ComponentName)) {
			KD_FPRINT("there is no component of this type in the given entity. cname: %s\tctype: %i",
					  ComponentName.c_str(), (int)Type);
			return nullptr;
		}

		auto ptr = _kcstorage[(U8)Type]->get(getComponentIndex(Type, ComponentName));
		return ptr;
	}

	void KEntity::getScriptComponents(std::vector<KComponent *> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		} else if (_kcstorage[(U8)KComTypes::KCT_LOGIC] == nullptr) {
			KD_FPRINT("unregistered component types. ctype: %i", (int)KComTypes::KCT_LOGIC);
			return;
		}

		Output.reserve(_kscriptComp.size());
		for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {
			auto ptr = _kcstorage[(U8)KComTypes::KCT_LOGIC]->get(it->second);
			Output.push_back(static_cast<KComponent *>(ptr));
		}
	}

	void KEntity::removeComponent(KComTypes Type, const std::string &Name) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;

		// check component type
		} else if (_kcstorage[(U8)Type] == nullptr) {
			KD_FPRINT("unregistered component types. cname: %s\tctype: %i", Name.c_str(), (int)Type);
			return;

		} else if (!hasComponent(Type, Name)) {
			return;
		}

		// deattach component from entity and call deattach function
		void *comPtr = _kcstorage[(U8)Type]->get(getComponentIndex(Type, Name));
		KComponent *bPtr = static_cast<KComponent *>(comPtr);
		U32 cindex = bPtr->getID();
		bPtr->deattached(getID());
		
		// remove components id from entity
		if (Type == KComTypes::KCT_LOGIC) {
			_kscriptComp.erase(Name);
		} else {
			_kfixedComp[(U8)Type] = -1;
		}

		// and remove component itself from storage
		_kcstorage[(U8)Type]->remove(cindex);
	}

	void KEntity::clearComponents() {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		} 

		// first remove all script components
		if (_kcstorage[(U8)KComTypes::KCT_LOGIC] != nullptr) {
			for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {

				// call deattach on all components
				KComponent *comPtr = static_cast<KComponent *>(_kcstorage[(U8)KComTypes::KCT_LOGIC]->get(it->second));
				comPtr->deattached(_kid);

				_kcstorage[(U8)KComTypes::KCT_LOGIC]->remove(it->second);
			}
		}
		_kscriptComp.clear();

		// remove all fixed components
		for (U8 i = 0; i < (U8)KComTypes::KCT_MAX_COMP_SIZE; i++) {
			if (i == (U8)KComTypes::KCT_LOGIC) {
				continue;
			}
			if (_kcstorage[i] != nullptr) {
				if (hasComponent((KComTypes)i, "")) {

					// call remove on all components
					U32 ind = _kfixedComp[i];
					KComponent *comPtr = static_cast<KComponent *>(_kcstorage[i]->get(ind));
					comPtr->deattached(_kid);

					_kcstorage[i]->remove(ind);

					_kfixedComp[i] = -1;
				}
			}
		}
	}

	I32 KEntity::getComponentIndex(KComTypes Type, const std::string &Name) {
		// script component
		if (Type == KComTypes::KCT_LOGIC) {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
				return found->second;
			}

		// non-script component
		} else {
			return _kfixedComp[(U8)Type];
		}

		return -1;
	}
	
	bool KEntity::hasComponent(KComTypes Type, const std::string &Name) {
		if (Type == KComTypes::KCT_LOGIC) {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
				return true;
			}
		} else {
			if (_kfixedComp[(U8)Type] >= 0) {
				return true;
			}
		}

		return false;
	}

	bool KEntity::hasComponentType(KComTypes Type) {
		if (Type == KComTypes::KCT_LOGIC && !_kscriptComp.empty()) {
			return true;
		} else {
			if (_kfixedComp[(U8)Type] >= 0) {
				return true;
			}
		}

		return false;
	}

	KMETA_KENTITY_SOURCE();
}