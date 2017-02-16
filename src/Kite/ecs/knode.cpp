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
#include "Kite/ecs/knode.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdarray.h"
#include "Kite/serialization/types/kstdbitset.h"
#include <luaintf\LuaIntf.h>
#include "kmeta.khgen.h"

namespace Kite {

	KNode::KNode() :
#ifdef KITE_EDITOR
		_kaddCallb(nullptr),
		_kremCallb(nullptr),
		_kaddOpaque(nullptr),
		_kremOpaque(nullptr),
#endif
		_kparent(nullptr),
		_kselfIter(nullptr),
		_kshared(new KNode::SharedData())
	{
		registerCTypes(this);
		_kshared->root = this;
	}

	KNode::KNode(KNode *Parent, std::list<KNode>::iterator *SelfIt):
#ifdef KITE_EDITOR
		_kaddCallb(nullptr),
		_kremCallb(nullptr),
		_kaddOpaque(nullptr),
		_kremOpaque(nullptr),
#endif
		_kparent(Parent),
		_kselfIter(SelfIt),
		_kshared(Parent->_kshared)
	{}

	KNode::~KNode() {
		// clear all components
		clearComponents(CClearMode::ALL);

		// clear all childs
		_kchilds.clear();

		// only root can create/destroy storages
		if (isRoot()) {
			delete _kshared;
			_kshared = nullptr;
		}
	}

	KNode &KNode::operator=(const KNode &RightNode) {
		// first clear childs
		_kchilds.clear();

		// copy node itself
		_kisCopy = true;
		_kstatic = RightNode._kstatic;
		_kactive = RightNode._kactive;
		_kname = RightNode._kname;		
		_klayerid = RightNode._klayerid;

		// clear all components
		clearComponents(CClearMode::ALL);

		// create fixed components and copy them
		for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			if ((CTypes)i == CTypes::Logic) continue; // ignore logic here
			if (RightNode.hasComponent((CTypes)i)) {
				auto com = addComponent((CTypes)i);
				(*com) = (*RightNode.getComponent((CTypes)i));	// base copy by value
			}
		}

		// create logic components
		for (auto it = RightNode._klogicComp.begin(); it != RightNode._klogicComp.end(); ++it) {
			(*addComponent(CTypes::Logic, it->first)) = (*RightNode.getComponentByHandle(it->second));
		}

		return *this;
	}

	bool KNode::loadStream(KIStream &Stream, const std::string &Address) {
		clearComponents(CClearMode::ALL);
		// TODO ....


		// baraie iad avari
		initeOnDeserial();
	}

	void KNode::initeOnDeserial() {
		// inite fixed components
		std::vector<KHandle> clist;
		getFixedComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			com->loadRes();
			com->attached(this);
		}

		// inite logic components
		getLogicComponents(clist);
		for (auto it = clist.begin(); it != clist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			com->loadRes();
			com->attached(this);
		}

		for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
			it->initeOnDeserial();
		}
	}

	KComponent *KNode::_addComponent(CTypes Type, const std::string &Name = "") {
		// this component is already exist
		// we just return it
		if (hasComponent(Type, Name)) {
			KD_FPRINT("component with this type is already exist. type name: %s", getCTypesName(Type).c_str());
			return getComponent(Type, Name);
		}

		// create component
		auto handle = _kshared->cstorage[(U16)Type]->add(Name);

		// set its handle
		auto com = _kshared->cstorage[(SIZE)Type]->get(handle);
		com->_khandle = handle;

		// attach the created component to the entity
		if (Type == CTypes::Logic) {
			_klogicComp[Name] = handle;
		} else {
			_kfixedComp[(SIZE)Type] = handle;
		}
		com->setOwnerNode(this);

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

	KComponent *KNode::addComponent(CTypes Type, const std::string &CName) {
		auto com = _addComponent(Type, CName);
	}

	KComponent *KNode::getComponent(CTypes Type, const std::string &Name) const {
		// script component
		if (Type == CTypes::Logic) {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return _kshared->cstorage[(U16)Type]->get(found->second);
			}

			// non-script component
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return _kshared->cstorage[(U16)Type]->get(_kfixedComp[(SIZE)Type]);
			}
		}

		KD_FPRINT("there is no component of this type in the given entity. ctype: %s", getCTypesName(Type).c_str());
		return nullptr;
	}

	KComponent *KNode::getComponentByHandle(const KHandle &CHandle) const {
		if (!hasComponentType((CTypes)CHandle.getCType())) {
			KD_FPRINT("there is no component of this type in the given entity. ctype: %s", getCTypesName(CHandle.getCType()).c_str());
			return nullptr;
		}

		return _kshared->cstorage[(U16)CHandle.getCType()]->get(CHandle);
	}

	void KNode::getFixedComponents(std::vector<KHandle> &Output) const {
		Output.reserve((SIZE)CTypes::maxSize);
		for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
			if (_kfixedComp[i] != KHandle()) {
				Output.push_back(_kfixedComp[i]);
			}
		}
	}

	void KNode::getLogicComponents(std::vector<KHandle> &Output) const {
		Output.reserve(_klogicComp.size());
		for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {
			Output.push_back(it->second);
		}
	}

	bool KNode::hasComponent(CTypes Type, const std::string &Name = "") const {
		if (Type == CTypes::Logic) {
			return (_klogicComp.find(Name) != _klogicComp.end());
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return true;
			}
		}

		return false;
	}

	bool KNode::hasComponentType(CTypes Type) const {
		if (Type == CTypes::Logic && !_klogicComp.empty()) {
			return true;
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return true;
			}
		}

		return false;
	}

	void KNode::removeComponent(CTypes Type, const std::string &Name = "") {
		if (!hasComponent(Type, Name)) {
			return;
		}
		removeComponentByHandle(getComponent(Type, Name)->getHandle());
	}

	void KNode::removeComponentByHandle(const KHandle &CHandle) {
		// check ref counter
		auto comPtr = _kshared->cstorage[(U16)CHandle.getCType()]->get(CHandle);
		if (comPtr->_krefcounter > 0) {
			KD_FPRINT("this component is required by another component. you shuld remove them first. ctype: %s"
					  , getCTypesName(CHandle.getCType()).c_str());
			return;
		}

		// deattach component from entity and call deattach function
		comPtr->deattached(this);

		// call editor callback (we must call editor callback befor removing dependency to avoid poniter dangling)
#ifdef KITE_EDITOR
		if (_kremCallb) {
			(*_kremCallb)(comPtr, _kremOpaque);
		}
#endif

		// remove components id from entity
		if (CHandle.getCType() == CTypes::Logic) {
			_klogicComp.erase(_kshared->cstorage[(U16)CHandle.getCType()]->get(CHandle)->getName());

		} else {
			_kfixedComp[(U16)CHandle.getCType()] = KHandle();
		}

		// dec ref counter
		auto depList = comPtr->getDependency();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			auto dep = getComponent((*it), "");
			if (dep) {
				--dep->_krefcounter;

				// remove if on zero dep (only removable zero dep components)
				if (dep->_kremoveNoDep && dep->_krefcounter == 0) {
					removeComponentByHandle(dep->getHandle());
				}
			}
		}

		// and remove component itself from storage (add to trash list for avoiding pointer dangling)
		_kshared->trashList.push_back(CHandle);
	}

	void KNode::clearComponents(CClearMode Mode) {
		// logic components
		if (Mode == CClearMode::LOGIC || Mode == CClearMode::ALL) {
			for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {

				// call deattach on all components
				auto comPtr = _kshared->cstorage[(SIZE)CTypes::Logic]->get(it->second);
				comPtr->deattached(this);
				comPtr->unloadRes();

				_kshared->cstorage[(SIZE)CTypes::Logic]->remove(it->second);
			}
			_klogicComp.clear();
		}

		// fixed components
		if (Mode == CClearMode::FIXED || Mode == CClearMode::ALL) {
			for (SIZE i = 0; i < (SIZE)CTypes::maxSize; ++i) {
				if (_kfixedComp[i] == KHandle()) {
					continue;
				}

				auto storage = _kshared->cstorage[i];

				// call deattach on all components
				auto comPtr = storage->get(_kfixedComp[i]);
				comPtr->deattached(this);
				comPtr->unloadRes();

				// remove from storage
				storage->remove(_kfixedComp[i]);
				_kfixedComp[i] = KHandle();
			}
		}
	}
}