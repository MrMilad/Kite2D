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
		_kplistid(0), _kname(Name), _khparrent(false),
		_kcstorage(nullptr), _kestorage(nullptr)
	{}

	KEntity::~KEntity() {}

	KRecieveTypes KEntity::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		// redirect message
		postMessage(Message, Scope);

		return KRecieveTypes::KMR_RECEIVED;
	}

	void KEntity::setComponent(KComTypes Type, const std::string &Name, const KHandle &Index) {
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

	void KEntity::addChild(const KHandle &EHandle) {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}

		auto Child = _kestorage->get(EHandle);
		if (Child == nullptr) {
			KD_PRINT("invalid entity handle");
			return;

		} else if (Child->getParrentHandle() == getHandle()) {
			return;

		} else if (EHandle == getHandle()) {
			KD_PRINT("an entity cant be a child of himself");
			return;
		}

		// deattach from old parrents
		if (Child->hasParrent()) {
			auto parrent = _kestorage->get(Child->getParrentHandle());
			parrent->remChildIndex(Child->getPListID());
		}

		// attach to new parrent
		Child->setPHandle(getHandle());
		Child->setPListID(_kchilds.size());
		Child->setHParrent(true);
		_kchilds.push_back(EHandle);
	}

	void KEntity::removeChild(const KHandle &EHandle) {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}
		
		auto Child = _kestorage->get(EHandle);
		if (Child == nullptr) {
			KD_PRINT("invalid handle");
			return;

		} else if (Child->getParrentHandle() != getHandle()) {
			KD_PRINT("this entity is not in the children list");
			return;
		}

		// add it to root
		KHandle root;
		root.index = 0;
		root.signature = 0;
		_kestorage->get(root)->addChild(EHandle);
	}

	void KEntity::clearChilds() {
		if (_kestorage == nullptr) {
			KD_PRINT("set entity storage at first");
			return;
		}

		for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
			KHandle root;
			root.index = 0;
			root.signature = 1;
			_kestorage->get(root)->addChild((*it));
		}
	}

	bool KEntity::hasChild() const {
		if (_kchilds.empty()) {
			return false;
		}

		return true;
	}

	KHandle KEntity::addComponent(KComTypes Type, const std::string &ComponentName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return KHandle();

		// check component type
		} else if (_kcstorage[(U8)Type] == nullptr) {
			KD_FPRINT("unregistered component types. cname: %s\tctype: %i", ComponentName.c_str(), (int)Type);
			return KHandle();

		// this component is already exist
		// we just return it
		} else if (hasComponent(Type, ComponentName)) {
			return getComponentByName(Type, ComponentName)->getHandle();
		}

		// create component
		auto handle = _kcstorage[(U8)Type]->add(ComponentName);

		// set its index
		auto com = _kcstorage[(U8)Type]->get(handle);
		com->setHandle(handle);

		// attach the created component to the entity and call attach functon
		setComponent(Type, ComponentName, handle);
		com->setOwnerHandle(getHandle());
		com->attached();

		return handle;
	}

	KComponent *KEntity::getComponent(KComTypes Type, const KHandle &Handle) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		} else if (_kcstorage[(U8)Type] == nullptr) {
			KD_FPRINT("unregistered component types. ctype: %i", (int)Type);
			return nullptr;

		} else if (!hasComponentType(Type)) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %i", (int)Type);
			return nullptr;
		}

		return _kcstorage[(U8)Type]->get(Handle);
	}

	KComponent *KEntity::getComponentByName(KComTypes Type, const std::string &ComponentName) {
		return getComponent(Type, getComponentHandle(Type, ComponentName));
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
		auto hndl = getComponentHandle(Type, Name);
		auto comPtr = _kcstorage[(U8)Type]->get(hndl);
		comPtr->setOwnerHandle(getHandle());
		comPtr->deattached();
		
		// remove components id from entity
		if (Type == KComTypes::KCT_LOGIC) {
			_kscriptComp.erase(Name);
		} else {
			_kfixedComp[(U8)Type] = KHandle();
		}

		// and remove component itself from storage
		_kcstorage[(U8)Type]->remove(hndl);
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
				auto comPtr = _kcstorage[(U8)KComTypes::KCT_LOGIC]->get(it->second);
				comPtr->deattached();

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
				if (hasComponentType((KComTypes)i)) {

					// call remove on all components
					auto hndl = _kfixedComp[i];
					auto comPtr = _kcstorage[i]->get(hndl);
					comPtr->deattached();

					_kcstorage[i]->remove(hndl);

					_kfixedComp[i] = KHandle();
				}
			}
		}
	}

	KHandle KEntity::getComponentHandle(KComTypes Type, const std::string &Name) {
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

		return KHandle();
	}
	
	bool KEntity::hasComponent(KComTypes Type, const std::string &Name) {
		if (Type == KComTypes::KCT_LOGIC) {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
				return true;
			}
		} else {
			if (_kfixedComp[(U8)Type].signature > 0) {
				return true;
			}
		}

		return false;
	}

	bool KEntity::hasComponentType(KComTypes Type) {
		if (Type == KComTypes::KCT_LOGIC && !_kscriptComp.empty()) {
			return true;
		} else {
			if (_kfixedComp[(U8)Type].signature > 0) {
				return true;
			}
		}

		return false;
	}

	KMETA_KENTITY_SOURCE();
}