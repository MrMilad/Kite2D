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
#include "Kite/serialization/types/kstdbitset.h"
#include <luaintf\LuaIntf.h>

namespace Kite {

	std::vector<KHandle> KEntity::_ktrashList;
	KEntity::KEntity(const std::string &Name):
#ifdef KITE_EDITOR
		 _kaddCallb(nullptr),
		_kremCallb(nullptr),
		_kaddOpaque(nullptr),
		_kremOpaque(nullptr),
#endif
		_kstatic(false),
		_kdeleted(false),
		_kplistid(0),
		_kname(Name),
		_kisPrefab(false),
		_klayerid(0),
		_kzorder(0),
		_kestorage(nullptr),
		_kcstorage(nullptr)
	{}

	KEntity::~KEntity() {}

	RecieveTypes KEntity::onMessage(KMessage *Message, MessageScope Scope) {
		// redirect message
		// to self
		if (Scope == MessageScope::SELF) {
			if (_kcstorage != nullptr) {
				for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
					auto comp = _kcstorage[(SIZE)CTypes::Logic]->get((*it));
					if (comp != nullptr) {
						comp->onMessage(Message, Scope);
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
				for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
					auto comp = _kcstorage[(SIZE)CTypes::Logic]->get((*it));
					if (comp != nullptr) {
						comp->onMessage(Message, Scope);
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

		// set activation of childs
		if (_kestorage != nullptr) {
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				_kestorage->get((*it))->setActive(Active);
			}
		}
	}

	void KEntity::setComponent(KComponent *Comp) {
		if (Comp->getType() == CTypes::Logic) {
			_klogicOrder.push_back(Comp->getHandle());
			_klogicComp[Comp->getName()] = Comp->getHandle();
		} else {
			_kfixedComp[(SIZE)Comp->getType()] = Comp->getHandle();
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

	void KEntity::forceRemoveCom(CTypes Type) {
		if (!hasComponent(Type, "") || Type == CTypes::Logic) {
			return;
		}

		// check ref counter
		auto hndl = getComponentHandle(Type, "");
		auto comPtr = _kcstorage[(SIZE)Type]->get(hndl);

		// deattach component from entity and call deattach function
		comPtr->setOwnerHandle(getHandle());
		comPtr->deattached(this);

		// call editor callback (we must call editor callback befor removing dependency to avoid poniter dangling)
#ifdef KITE_EDITOR
		if (_kremCallb) {
			(*_kremCallb)(comPtr, _kremOpaque);
		}
#endif

		// remove components id from entity
		_kfixedComp[(SIZE)Type] = KHandle();

		// dec ref counter
		auto depList = comPtr->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			auto dep = getComponent((*it), "");
			if (dep) {
				--dep->_krefcounter;

				// remove if on zero dep (only removable zero dep components)
				if (dep->_kremoveNoDep && dep->_krefcounter == 0) {
					removeComponent(dep->getType(), dep->getName());
				}
			}
		}

		// and remove component itself from storage
		_kcstorage[(SIZE)Type]->remove(hndl);
	}

	void KEntity::initeComponents() {
		std::vector<KHandle> clist;

		// inite fixed components
		getFixedComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			com->attached(this);
		}

		// inite logic components
		getScriptComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			com->attached(this);
		}
	}

	void KEntity::postClearComponents() {
		// cehck storage
		if (_kcstorage != nullptr) {
			// first remove all script components
			for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {

				// call deattach on all components
				auto comPtr = _kcstorage[(SIZE)CTypes::Logic]->get((*it));
				comPtr->deattached(this);

				_kcstorage[(SIZE)CTypes::Logic]->remove((*it));
			}
			_klogicComp.clear();
			_klogicOrder.clear();

			// fixed components
			for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
				if (_kfixedComp[i] == KHandle()) {
					continue;
				}

				auto storage = _kcstorage[i];

				// call deattach on all components
				auto comPtr = storage->get(_kfixedComp[i]);
				comPtr->deattached(this);

				// remove from storage
				storage->remove(_kfixedComp[i]);
				_kfixedComp[i] = KHandle();
			}
		}

	}

	void KEntity::postWork(Internal::BaseCHolder<KComponent> **Storage) {
		for (auto it = _ktrashList.begin(); it != _ktrashList.end(); ++it) {
			Storage[(SIZE)it->type]->remove((*it));
		}

		_ktrashList.clear();
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

	void KEntity::setLayer(U8 LayerID) {
		if (LayerID < KENTITY_LAYER_SIZE) {
			_klayerid = LayerID;
			return;
		}

		KD_PRINT("layer id is out of range");
	}

	void KEntity::setStatic(bool Static) {
		if (_kstatic != Static) {
			_kstatic = Static;

			// send message about this change
			KMessage msg;
			msg.setType("STATIC_CHANGED");
			msg.setData(this, sizeof(this));

			postMessage(&msg, MessageScope::ALL);
		}
	}

	KComponent *KEntity::addComponent(CTypes Type, const std::string &CName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;
		}
		
		// this component is already exist
		// we just return it
		if (hasComponent(Type, CName)) {
			return getComponent(Type, CName);
		}

		// create component
		auto handle = _kcstorage[(SIZE)Type]->add(CName);
		handle.type = (SIZE)Type;

		// set its handle
		auto com = _kcstorage[(SIZE)Type]->get(handle);
		com->_khandle = handle;

		// attach the created component to the entity
		setComponent(com);
		com->setOwnerHandle(getHandle());

		// check component's dependency (recursive).
		// circular-dependency is allowed but not recommended.
		auto depList = com->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			auto dep = addComponent((*it));
			if (dep) {
				++dep->_krefcounter;
			}
		}

		// call attach functon
		com->attached(this);

		// call editor callback
#ifdef KITE_EDITOR
		if (_kaddCallb) {
			(*_kaddCallb)(com, _kaddOpaque);
		}
#endif

		return com;
	}

	KComponent *KEntity::getComponent(CTypes Type, const std::string &CName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		}

		if (!hasComponentType(Type)) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %s", getCTypesName(Type).c_str());
			return nullptr;
		}

		auto chandle = getComponentHandle(Type, CName);
		return _kcstorage[chandle.type]->get(chandle);
	}

	KComponent *KEntity::getComponentByHandle(const KHandle &CHandle) {
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;
		}

		if (!hasComponentType((CTypes)CHandle.type)) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %s", getCTypesName((CTypes)CHandle.type).c_str());
			return nullptr;
		}

		return _kcstorage[CHandle.type]->get(CHandle);
	}

	void KEntity::getFixedComponents(std::vector<KHandle> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}

		Output.reserve((SIZE)CTypes::maxSize);
		for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			if (_kfixedComp[i] != KHandle()) {
				Output.push_back(_kfixedComp[i]);
			}
		}
	}

	void KEntity::getScriptComponents(std::vector<KHandle> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}
		
		Output.reserve(_klogicComp.size());
		for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
			Output.push_back((*it));
		}
	}

	void KEntity::removeComponent(CTypes Type, const std::string &Name) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}
		
		if (!hasComponent(Type, Name)) {
			return;
		}

		// check ref counter
		auto hndl = getComponentHandle(Type, Name);
		auto comPtr = _kcstorage[(SIZE)Type]->get(hndl);
		if (comPtr->_krefcounter > 0) {
			KD_FPRINT("this component is required by another component. you shuld remove them first. ctype: %s"
					  , getCTypesName(Type).c_str());
			return;
		}

		// deattach component from entity and call deattach function
		comPtr->setOwnerHandle(getHandle());
		comPtr->deattached(this);

		// call editor callback (we must call editor callback befor removing dependency to avoid poniter dangling)
#ifdef KITE_EDITOR
		if (_kremCallb) {
			(*_kremCallb)(comPtr, _kremOpaque);
		}
#endif
		
		// remove components id from entity
		if (Type == CTypes::Logic) {
			_klogicComp.erase(Name);

			// remove from logic order list (linear search)
			for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {
				if ((*it) == hndl) {
					_klogicOrder.erase(it);
					break;
				}
			}
		} else {
			_kfixedComp[(SIZE)Type] = KHandle();
		}

		// dec ref counter
		auto depList = comPtr->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			auto dep = getComponent((*it), "");
			if (dep) {
				--dep->_krefcounter;

				// remove if on zero dep (only removable zero dep components)
				if (dep->_kremoveNoDep && dep->_krefcounter == 0) {
					removeComponent(dep->getType(), dep->getName());
				}
			}
		}

		// and remove component itself from storage (add to trash list for avoiding pointer dangling)
		_ktrashList.push_back(hndl);
	}

	void KEntity::clearComponents() {
		// cehck storage
		if (_kcstorage != nullptr) {
			// first remove all script components
			for (auto it = _klogicOrder.begin(); it != _klogicOrder.end(); ++it) {

				// call deattach on all components
				auto comPtr = _kcstorage[(SIZE)CTypes::Logic]->get((*it));
				comPtr->deattached(this);

				// add to trash list
				_ktrashList.push_back((*it));
			}
			_klogicComp.clear();
			_klogicOrder.clear();

			// fixed components
			for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
				if (_kfixedComp[i] == KHandle()) {
					continue;
				}

				auto storage = _kcstorage[i];

				// call deattach on all components
				auto comPtr = storage->get(_kfixedComp[i]);
				comPtr->deattached(this);

				// add to trash list
				_ktrashList.push_back(_kfixedComp[i]);

				// remove from storage
				_kfixedComp[i] = KHandle();
			}
		}

	}

	KHandle KEntity::getComponentHandle(CTypes Type, const std::string &Name) {
		// script component
		if (Type == CTypes::Logic) {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return found->second;
			}

		// non-script component
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return _kfixedComp[(SIZE)Type];
			}
		}

		KD_FPRINT("there is no component of this type in the given entity. ctype: %s", getCTypesName(Type).c_str());
		return KHandle();
	}

	void KEntity::reorderScriptComponent(const KHandle &CHandle, U32 NewOrder) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		}

		if (CHandle.type != (SIZE)CTypes::Logic) {
			KD_FPRINT("invalid handle type. handle type is: %s", getCTypesName((CTypes) CHandle.type).c_str());
			return;
		}
		auto comp = _kcstorage[CHandle.type]->get(CHandle);
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
		if (!hasComponent(CTypes::Logic, CName)) {
			KD_FPRINT("there is no logic component with the given name. cname: %s", CName.c_str());
			return;
		}
		
		reorderScriptComponent(getComponentHandle(CTypes::Logic, CName), NewOrder);
	}
	
	bool KEntity::hasComponent(CTypes Type, const std::string &Name) {
		if (Type == CTypes::Logic) {
			if (_klogicComp.find(Name) != _klogicComp.end()) {
				return true;
			}
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return true;
			}
		}

		return false;
	}

	bool KEntity::hasComponentType(CTypes Type) {
		if (Type == CTypes::Logic && !_klogicComp.empty()) {
			return true;
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return true;
			}
		}

		return false;
	}

	KMETA_KENTITY_SOURCE();
}