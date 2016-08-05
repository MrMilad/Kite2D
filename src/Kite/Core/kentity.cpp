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
#include "Kite/core/kprefab.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KEntity::KEntity(const std::string &Name):
#ifdef KITE_EDITOR
		 _kaddCallb(nullptr),
		_kopaqPtr(nullptr),
#endif
		_kplistid(0), _kname(Name),
		_kcstorage(nullptr),
		_kestorage(nullptr),
		_kisPrefab(false)
	{}

	KEntity::~KEntity() {}

	RecieveTypes KEntity::onMessage(KMessage *Message, MessageScope Scope) {
		// redirect message
		// to self
		if (Scope == MessageScope::SELF) {
			if (_kcstorage != nullptr) {
				auto found = _kcstorage->find("Logic");
				if (found != _kcstorage->end()) {
					for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
						auto comp = found->second->get((*it));
						if (comp != nullptr) {
							comp->onMessage(Message, Scope);
						}
					}
				}
			}

		// to childs
		} else if (Scope == MessageScope::CHILDREN) {
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				auto ent = _kestorage->get((*it));
				if (ent != nullptr) {
					ent->onMessage(Message, MessageScope::SELF);
				}
			}

		// all
		} else if (Scope == MessageScope::ALL) {
			if (_kcstorage != nullptr) {
				auto found = _kcstorage->find("Logic");
				if (found != _kcstorage->end()) {
					for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
						auto comp = found->second->get((*it));
						if (comp != nullptr) {
							comp->onMessage(Message, Scope);
						}
					}
				}
			}

			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				auto ent = _kestorage->get((*it));
				if (ent != nullptr) {
					ent->onMessage(Message, Scope);
				}
			}
		}

		return RecieveTypes::RECEIVED;
	}

	void KEntity::setActive(bool Active) {
		_kactive = Active;

		if (_kestorage != nullptr) {
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				_kestorage->get((*it))->setActive(Active);
			}
		}
	}

	void KEntity::setComponent(const std::string &CType, const std::string &Name, KComponent *Comp) {
		if (CType == "Logic") {
			_klogicOrder.push_back(Comp->getHandle());
			_klogicComp[Name] = Comp->getHandle();
		} else {
			_kfixedComp[CType] = Comp->getHandle();
		}
	}

	void KEntity::remChildIndex(U32 CIndex) {
		if (CIndex != (_kchilds.size() - 1)) {
			_kestorage->get(_kchilds.back())->_kplistid = CIndex;
			_kchilds[CIndex] = _kchilds.back();
		}

		_kchilds.pop_back();
	}

	void KEntity::setHandle(const KHandle Handle) {
		_kluatable = "ent" + std::to_string(Handle.index);
		_khandle = Handle;
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

		} else if (Child->getParentHandle() == getHandle()) {
			return;

		} else if (EHandle == getHandle()) {
			KD_PRINT("an entity cant be a child of himself");
			return;
		}

		// deattach from old parents
		auto phandle = Child->getParentHandle();
		if (phandle != KHandle()) {
			auto parent = _kestorage->get(phandle);
			if (parent != nullptr) {
				parent->remChildIndex(Child->_kplistid);
			}
		}

		// attach to new parent
		Child->_kphandle = getHandle();
		Child->_kplistid = _kchilds.size();
		_kchilds.push_back(EHandle);
	}

	bool KEntity::hasChild() const {
		if (_kchilds.empty()) {
			return false;
		}

		return true;
	}

	KComponent *KEntity::addComponent(const std::string &CType, const std::string &CName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		// check component type
		}
		
		auto found = _kcstorage->find(CType);
		if (_kcstorage->find(CType) == _kcstorage->end()) {
			KD_FPRINT("unregistered component types. ctype: %s   cname: %s", CType.c_str(), CName.c_str());
			return nullptr;

		// this component is already exist
		// we just return it
		} else if (hasComponent(CType, CName)) {
			return getComponentByName(CType, CName);
		}

		// create component
		auto handle = found->second->add(CName);

		// set its handle
		auto com = found->second->get(handle);
		com->_khandle = handle;

		// attach the created component to the entity
		setComponent(CType, CName, com);
		com->setOwnerHandle(getHandle());

		// call attach functon
		com->attached(this);

		// check component's dependency (recursive).
		// circular-dependency is allowed but not recommended.
		auto depList = com->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			addComponent((*it));
		}

		// call editor callback
#ifdef KITE_EDITOR
		if (_kaddCallb) {
			(*_kaddCallb)(com, _kopaqPtr);
		}
#endif

		return com;
	}

	KComponent *KEntity::getComponent(const std::string &CType, const KHandle &Handle) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		}
		
		auto found = _kcstorage->find(CType);
		if (found == _kcstorage->end()) {
			KD_FPRINT("unregistered component types. ctype: %s", CType.c_str());
			return nullptr;

		} else if (!hasComponentType(CType)) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %s", CType.c_str());
			return nullptr;
		}

		return found->second->get(Handle);
	}

	KComponent *KEntity::getComponentByName(const std::string &CType, const std::string &CName) {
		return getComponent(CType, getComponentHandle(CType, CName));
	}

	void KEntity::getFixedComponents(std::vector<KComponent *> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}

		Output.reserve(_kfixedComp.size());
		for (auto it = _kfixedComp.begin(); it != _kfixedComp.end(); ++it) {
			auto ptr = _kcstorage->find(it->first)->second->get(it->second);
			Output.push_back(ptr);
		}
	}

	void KEntity::getScriptComponents(std::vector<KHandle> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}
		
		auto found = _kcstorage->find("Logic");
		if (found == _kcstorage->end()) {
			KD_PRINT("unregistered component types. ctype: Logic");
			return;
		}

		Output.reserve(_klogicComp.size());
		for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
			Output.push_back((*it));
		}
	}

	void KEntity::removeComponent(const std::string &CType, const std::string &Name) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}
		
		// check component type
		auto found = _kcstorage->find(CType);
		if (found == _kcstorage->end()) {
			KD_FPRINT("unregistered component types. ctype: %s   cname: %s", CType.c_str(), Name.c_str());
			return;

		} else if (!hasComponent(CType, Name)) {
			return;
		}

		// deattach component from entity and call deattach function
		auto hndl = getComponentHandle(CType, Name);
		auto comPtr = found->second->get(hndl);
		comPtr->setOwnerHandle(getHandle());
		comPtr->deattached(this);
		
		// remove components id from entity
		if (CType == "Logic") {
			_klogicComp.erase(Name);
			// linear search
			for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
				if ((*it) == hndl) {
					_klogicOrder.erase(it);
					break;
				}
			}
		} else {
			_kfixedComp.erase(CType);
		}

		// and remove component itself from storage
		found->second->remove(hndl);
	}

	void KEntity::clearComponents() {
		// cehck storage
		if (_kcstorage != nullptr) {
			// first remove all script components
			auto found = _kcstorage->find("Logic");
			if (found != _kcstorage->end()) {
				for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {

					// call deattach on all components
					auto comPtr = found->second->get((*it));
					comPtr->deattached(this);

					found->second->remove((*it));
				}
			}
			_klogicComp.clear();
			_klogicOrder.clear();

			for (auto it = _kfixedComp.begin(); it != _kfixedComp.end(); ++it) {
				auto storage = _kcstorage->find(it->first);

				// call deattach on all components
				auto comPtr = storage->second->get(it->second);
				comPtr->deattached(this);

				// remove from storage
				storage->second->remove(it->second);
			}
			_kfixedComp.clear();
		}

	}

	KHandle KEntity::getComponentHandle(const std::string &CType, const std::string &Name) {
		// script component
		if (CType == "Logic") {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return found->second;
			}

		// non-script component
		} else {
			auto found = _kfixedComp.find(CType);
			if (found != _kfixedComp.end()) {
				return found->second;
			}
		}

		return KHandle();
	}

	void KEntity::reorderScriptComponent(const KHandle &CHandle, U32 NewOrder) {
		auto comp = getComponent("Logic", CHandle);
		if (comp == nullptr) {
			KD_PRINT("there is no logic component with the given handle.");
			return;
		}

		// if new order is out of range we set it at max
		if (NewOrder > _klogicComp.size()) {
			NewOrder = _klogicComp.size() - 1;
		}

		SIZE dst = 0;
		for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
			if ((*it) == CHandle) {
				_klogicOrder.erase(it);
				break; 
			}
			++dst;
		}

		_klogicOrder.insert(_klogicOrder.begin() + NewOrder, CHandle);
	}

	void KEntity::reorderScriptComponentByName(const std::string &CName, U32 NewOrder) {
		if (!hasComponent("Logic", CName)) {
			KD_FPRINT("there is no logic component with the given name. cname: %s", CName.c_str());
			return;
		}
		
		reorderScriptComponent(getComponentHandle("Logic", CName), NewOrder);
	}
	
	bool KEntity::hasComponent(const std::string &CType, const std::string &Name) {
		if (CType == "Logic") {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return true;
			}
		} else {
			auto found = _kfixedComp.find(CType);
			if (found != _kfixedComp.end()) {
				return true;
			}
		}

		return false;
	}

	bool KEntity::hasComponentType(const std::string &CType) {
		if (CType == "Logic" && !_klogicComp.empty()) {
			return true;
		} else {
			if (_kfixedComp.find(CType) != _kfixedComp.end()) {
				return true;
			}
		}

		return false;
	}

	KMETA_KENTITY_SOURCE();
}