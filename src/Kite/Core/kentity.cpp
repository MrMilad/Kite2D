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
		_kplistid(0), _kname(Name), _kcstorage(nullptr),
		_kestorage(nullptr), _kctypes(nullptr)
	{}

	KEntity::~KEntity() {}

	KRecieveTypes KEntity::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		// redirect message
		// to self
		if (Scope == KMessageScopeTypes::KMS_SELF) {
			if (_kctypes != nullptr) {
				auto found = _kctypes->find("Logic");
				if (found != _kctypes->end()) {
					for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {
						auto comp = _kcstorage[found->second]->get(it->second);
						if (comp != nullptr) {
							comp->onMessage(Message, Scope);
						}
					}
				}
			}

		// to childs
		} else if (Scope == KMessageScopeTypes::KMS_CHILDREN) {
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				auto ent = _kestorage->get((*it));
				if (ent != nullptr) {
					ent->onMessage(Message, KMessageScopeTypes::KMS_SELF);
				}
			}

		// all
		} else if (Scope == KMessageScopeTypes::KMS_ALL) {
			if (_kctypes != nullptr) {
				auto found = _kctypes->find("Logic");
				if (found != _kctypes->end()) {
					for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {
						auto comp = _kcstorage[found->second]->get(it->second);
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

		return KRecieveTypes::KMR_RECEIVED;
	}

	void KEntity::setComponent(const std::string &CType, const std::string &Name, const KHandle &Handle) {
		if (CType == "Logic") {
			_kscriptComp[Name] = Handle;
		} else {
			_kfixedComp[CType] = Handle;
		}
	}

	void KEntity::remChildIndex(U32 CIndex) {
		if (CIndex != (_kchilds.size() - 1)) {
			_kestorage->get(_kchilds.back())->_kplistid = CIndex;
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

	KHandle KEntity::addComponent(const std::string &CType, const std::string &CName) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return KHandle();

		// check component type
		} else if (_kctypes->find(CType) == _kctypes->end()) {
			KD_FPRINT("unregistered component types. ctype: %s   cname: %s", CType.c_str(), CName.c_str());
			return KHandle();

		// this component is already exist
		// we just return it
		} else if (hasComponent(CType, CName)) {
			return getComponentByName(CType, CName)->getHandle();
		}

		// create component
		auto found = _kctypes->find(CType);
		auto handle = _kcstorage[found->second]->add(CName);

		// set its handle
		auto com = _kcstorage[found->second]->get(handle);
		com->_khandle = handle;

		// attach the created component to the entity
		setComponent(CType, CName, handle);
		com->setOwnerHandle(getHandle());

		// call attach functon
		com->attached();

		return handle;
	}

	KComponent *KEntity::getComponent(const std::string &CType, const KHandle &Handle) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return nullptr;

		} else if (_kctypes->find(CType) == _kctypes->end()) {
			KD_FPRINT("unregistered component types. ctype: %s", CType.c_str());
			return nullptr;

		} else if (!hasComponentType(CType)) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %s", CType.c_str());
			return nullptr;
		}

		return _kcstorage[_kctypes->find(CType)->second]->get(Handle);
	}

	KComponent *KEntity::getComponentByName(const std::string &CType, const std::string &CName) {
		return getComponent(CType, getComponentHandle(CType, CName));
	}

	void KEntity::getScriptComponents(std::vector<KComponent *> &Output) {
		Output.clear();
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;
		} else if (_kctypes->find("Logic") == _kctypes->end()) {
			KD_PRINT("unregistered component types. ctype: Logic");
			return;
		}

		Output.reserve(_kscriptComp.size());
		auto found = _kctypes->find("Logic");
		for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {
			auto ptr = _kcstorage[found->second]->get(it->second);
			Output.push_back(static_cast<KComponent *>(ptr));
		}
	}

	void KEntity::removeComponent(const std::string &CType, const std::string &Name) {
		// cehck storage
		if (_kcstorage == nullptr) {
			KD_PRINT("set component storage at first");
			return;

		// check component type
		} else if (_kctypes->find(CType) == _kctypes->end()) {
			KD_FPRINT("unregistered component types. ctype: %s   cname: %s", CType.c_str(), Name.c_str());
			return;

		} else if (!hasComponent(CType, Name)) {
			return;
		}

		// deattach component from entity and call deattach function
		auto found = _kctypes->find(CType);
		auto hndl = getComponentHandle(CType, Name);
		auto comPtr = _kcstorage[found->second]->get(hndl);
		comPtr->setOwnerHandle(getHandle());
		comPtr->deattached();
		
		// remove components id from entity
		if (CType == "Logic") {
			_kscriptComp.erase(Name);
		} else {
			_kfixedComp.erase(CType);
		}

		// and remove component itself from storage
		_kcstorage[found->second]->remove(hndl);
	}

	void KEntity::clearComponents() {
		// cehck storage
		if (_kcstorage != nullptr) {
			// first remove all script components
			auto found = _kctypes->find("Logic");
			if (found != _kctypes->end()) {
				for (auto it = _kscriptComp.begin(); it != _kscriptComp.end(); ++it) {

					// call deattach on all components
					auto comPtr = _kcstorage[found->second]->get(it->second);
					comPtr->deattached();

					_kcstorage[found->second]->remove(it->second);
				}
			}
			_kscriptComp.clear();

			for (auto it = _kfixedComp.begin(); it != _kfixedComp.end(); ++it) {

				// call deattach on all components
				auto comPtr = _kcstorage[_kctypes->find(it->first)->second]->get(it->second);
				comPtr->deattached();

				_kcstorage[_kctypes->find(it->first)->second]->remove(it->second);
			}
			_kfixedComp.clear();
		}

	}

	KHandle KEntity::getComponentHandle(const std::string &CType, const std::string &Name) {
		// script component
		if (CType == "Logic") {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
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
	
	bool KEntity::hasComponent(const std::string &CType, const std::string &Name) {
		if (CType == "Logic") {
			auto found = _kscriptComp.find(Name);
			if (found != _kscriptComp.end()) {
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
		if (CType == "Logic" && !_kscriptComp.empty()) {
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