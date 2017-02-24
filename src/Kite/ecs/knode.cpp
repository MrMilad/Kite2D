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
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdarray.h"
#include "Kite/serialization/types/kstdpair.h"
#include <luaintf\LuaIntf.h>
#include "kmeta.khgen.h"

namespace Kite {

	KNode::KNode(const std::string &Name) :
		KResource("", ""),
		_kparent(nullptr),
		_kshared(new KNode::SharedData()),
		_kname(Name),
		_klayerid(0),
		_kzorder(0),
		_kselfIndex(0),
		_kactive(true),
		_kstatic(false)
	{
		registerCTypes(this);
		_kshared->root = this;
		_kluatable.reserve(10);
		_kluatable.append("NODE");
		_kluatable.append(std::to_string(0));
	}

	KNode::KNode(const std::string &Name, const std::string &Address):
		KResource(Name, Address),
		_kparent(nullptr),
		_kshared(new KNode::SharedData()),
		_kname(Name)
	{
		registerCTypes(this);
		_kshared->root = this;
	}

	KNode::KNode(KNode *Parent, SIZE SelfIndex):
		KResource("", ""),
		_kparent(Parent),
		_kselfIndex(SelfIndex),
		_kshared(Parent->_kshared),
		_klayerid(Parent->_klayerid),
		_kactive(Parent->_kactive),
		_kstatic(Parent->_kstatic)
	{
		_kzorder = ++_kshared->zorder;
		_kluatable.reserve(10);
		_kluatable.append("NODE");
		_kluatable.append(std::to_string(_kzorder));
	}

	KNode::~KNode() {
		// clear all components
		clearComponents(CClearMode::ALL);

		// clear all childs
		clearChilds();

		// only root can create/destroy storages
		if (isRoot()) {
			delete _kshared;
			_kshared = nullptr;
		}
	}

	RecieveTypes KNode::onMessage(KMessage *Message, MessageScope Scope) {
		// redirect message
		// to self
		if (Scope == MessageScope::SELF || Scope == MessageScope::ALL) {
			for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {
				auto comp = _kshared->cstorage[(SIZE)CTypes::Logic]->get(it->second);
				if (comp != nullptr) {
					comp->onMessage(Message, Scope);
				}
			}

			// to childs
		} else if (Scope == MessageScope::CHILDREN || Scope == MessageScope::ALL) {
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				(*it)->onMessage(Message, MessageScope::CHILDREN);
			}
		}

		return RecieveTypes::RECEIVED;
	}

	void KNode::serial(KBaseSerial &Serializer) {
		// node information
		Serializer << _kstatic;
		Serializer << _kactive;
		Serializer << _klayerid;
		Serializer << _kzorder;

		// serialize fixed components
		std::vector<KHandle> hlist;
		getFixedComponents(hlist);

		// write size
		Serializer << hlist.size();

		// write components data
		for (auto it = hlist.begin(); it != hlist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			Serializer << com->getDrivedType();
			com->serial(Serializer, false);
		}

		// logic components
		getLogicComponents(hlist);

		// write size
		Serializer << hlist.size();

		// write components data
		for (auto it = hlist.begin(); it != hlist.end(); ++it) {
			auto com = getComponentByHandle((*it));
			Serializer << com->getName();
			com->serial(Serializer, false);
		}

		//write childs size
		Serializer << _kchilds.size();

		// write childs data
		for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
			(*it)->serial(Serializer);
		}
	}

	void KNode::deserial(KBaseSerial &Serializer, KResourceManager *RMan) {
		// node information
		Serializer >> _kstatic;
		Serializer >> _kactive;
		Serializer >> _klayerid;
		Serializer >> _kzorder;

		// deserialize fixed components
		SIZE fsize = 0;
		Serializer >> fsize;

		// read components data
		for (SIZE i = 0; i < fsize; ++i) {
			CTypes type;
			Serializer >> type;
			
			auto com = _addComponent(type, "");
			com->deserial(Serializer, false);
			com->loadRes(RMan);
		}

		// deserialize logic components
		Serializer >> fsize;

		// read components data
		for (SIZE i = 0; i < fsize; ++i) {
			std::string name;
			Serializer >> name;
			auto com = _addComponent(CTypes::Logic, name);
			com->deserial(Serializer, false);
			com->loadRes(RMan);
		}

		// read childs size
		Serializer >> fsize;

		// read childs data
		for (SIZE i = 0; i < fsize; ++i) {
			auto child = addNewChild();
			child->deserial(Serializer, RMan);
		}
	}

	bool KNode::saveStream(KOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << std::string("KNode");

		// recursive
		serial(bserial);

		// save to stream
		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}
		return true;
	}

	bool KNode::_loadStream(std::unique_ptr<KIStream> Stream, KResourceManager *RManager){
		KBinarySerial bserial;
		bserial.loadStream(*Stream, getAddress());

		std::string format;
		bserial >> format;

		if (format != "KNode") {
			KD_PRINT("incrrect file format");
			return false;
		}

		deserial(bserial, RManager);
		return false;
	}

	void KNode::setZOrder(U32 Order) {
		if (_kzorder != Order) {
			_kzorder = Order;

			// send message about this change
			KMessage msg;
			msg.setType("onZOrderChange");
			msg.setData(this, sizeof(this));
			postMessage(&msg, MessageScope::ALL);
		}
	}

	void KNode::setName(const std::string &Name) {
		if (_kname != Name) {
			_kname = Name;

			// send message about this change
			KMessage msg;
			msg.setType("onNameChange");
			msg.setData(this, sizeof(this));
			postMessage(&msg, MessageScope::ALL);
		}
	}

	void KNode::setActive(bool Active) {
		if (_kactive != Active) {
			_kactive = Active;

			// send message about this change
			KMessage msg;
			msg.setType("onActiveChange");
			msg.setData(this, sizeof(this));
			postMessage(&msg, MessageScope::ALL);

			// set activation of childs
			for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
				(*it)->setActive(Active);
			}
		}
	}

	void KNode::setLayer(U8 LayerID) {
		if (LayerID >= KENTITY_LAYER_SIZE) {
			KD_PRINT("layer id is out of range");
			return;
		}

		if (_klayerid != LayerID) {
			_klayerid = LayerID;

			// send message about this change
			KMessage msg;
			msg.setType("onLayerChange");
			msg.setData(this, sizeof(this));
			postMessage(&msg, MessageScope::ALL);
		}
	}

	void KNode::setStatic(bool Static) {
		if (_kstatic != Static) {
			_kstatic = Static;

			// send message about this change
			KMessage msg;
			msg.setType("onStaticChange");
			msg.setData(this, sizeof(this));
			postMessage(&msg, MessageScope::ALL);
		}
	}

	KComponent *KNode::getComponentByType(CTypes Type, const std::string &Name) const {
		// script component
		if (Type == CTypes::Logic) {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return _kshared->cstorage[(U16)Type]->get(found->second);
			}

			// non-script component
		} else {
			if (_kfixedComp[(U16)Type] != KHandle()) {
				return _kshared->cstorage[(U16)Type]->get(_kfixedComp[(U16)Type]);
			}
		}

		KD_FPRINT("there is no component of this type in the given node. ctype: %s", getCTypesName(Type).c_str());
		return nullptr;
	}

	KComponent *KNode::getComponentByInterface(ITypes Type) const {
		return getComponentByHandle(_kinterface[(U16)Type]);
	}

	KComponent *KNode::getComponentByHandle(const KHandle &CHandle) const {
		if (!hasComponentByType(CHandle.getCType())) {
			KD_FPRINT("there is no component of this type in this node. ctype: %s", getCTypesName(CHandle.getCType()).c_str());
			return nullptr;
		}

		auto com = _kshared->cstorage[(U16)CHandle.getCType()]->get(CHandle);
		if (com) {
			if (com->getOwnerNode() != this) {
				KD_PRINT("this node is not owner of the handle.");
				return nullptr;
			}
		}

		return com;
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

	bool KNode::hasComponentByType(CTypes Type, const std::string &Name) const {
		if (Type == CTypes::Logic) {
			if (Name.empty()) {
				return (!_klogicComp.empty());
			} else {
				return (_klogicComp.find(Name) != _klogicComp.end());
			}
		} else {
			if (_kfixedComp[(SIZE)Type] != KHandle()) {
				return true;
			}
		}

		return false;
	}

	bool KNode::hasComponentByInterface(ITypes Type) const {
		if (_kinterface[(U16)Type] != KHandle()) {
			return true;
		}
		return false;
	}

	void KNode::removeComponentByType(CTypes Type, const std::string &Name) {
		if (hasComponentByType(Type, Name)) {
			removeComponentByHandle(getComponentByType(Type, Name)->getHandle());
		}
	}

	void KNode::removeComponentByInterface(ITypes Type) {
		if (_kinterface[(U16)Type] != KHandle()) {
			removeComponentByHandle(_kinterface[(U16)Type]);
		}
	}

	void KNode::removeComponentByHandle(const KHandle &CHandle) {
		// check ref counter
		auto comPtr = getComponentByHandle(CHandle);
		if (comPtr->_krefcounter > 0) {
			KD_FPRINT("this component is required by another component. you should remove them first. ctype: %s"
					  , getCTypesName(CHandle.getCType()).c_str());
			return;
		}

		// deattach component from node and call deattach function
		comPtr->deattached();

		// remove components id from node
		if (CHandle.getCType() == CTypes::Logic) {
			_klogicComp.erase(_kshared->cstorage[(U16)CHandle.getCType()]->get(CHandle)->getName());

		} else {
			_kfixedComp[(U16)CHandle.getCType()] = KHandle();

			// clear its interface
			auto ilist = comPtr->getDrivedIntList();
			for (auto it = ilist.begin(); it != ilist.end(); ++it) {
				_kinterface[(U16)(*it)] = KHandle();
			}
		}

		// dec ref counter
		auto depList = comPtr->getDrivedDepList();
		for (auto it = depList.begin(); it != depList.end(); ++it) {
			auto depCom = getComponentByType((*it));
			--depCom->_krefcounter;
		}

		// and remove component itself from storage (add to trash list for avoiding pointer dangling)
		_kshared->trashList.push_back(CHandle);
	}

	KNode *KNode::addChild(KNode *Nodes) {
		// allocate new node from pool
		auto newNode = new(_kshared->nodePool.allocate_node()) KNode(this, _kchilds.size());

		// save new node to child list
		_kchilds.push_back(newNode);

		// copy node state
		newNode->_kstatic = Nodes->_kstatic;
		newNode->_kactive = Nodes->_kactive;
		newNode->_klayerid = Nodes->_klayerid;

		// copy fixed components
		for (U16 i = 0; i < (U16)CTypes::maxSize; ++i) {
			if (Nodes->_kfixedComp[i] != KHandle()) {
				// add component to new node
				auto newCom = newNode->_addComponent((CTypes)i, "");

				// copy
				auto com = Nodes->getComponentByHandle(Nodes->_kfixedComp[i]);
				newCom->copyFrom(com);
			}
		}

		// copy logic components
		for (auto it = Nodes->_klogicComp.begin(); it != Nodes->_klogicComp.end(); ++it) {
			// add component to new node
			auto newCom = newNode->_addComponent(CTypes::Logic, it->first);

			// copy
			auto com = Nodes->getComponentByHandle(it->second);
			newCom->copyFrom(com);
		}

		return newNode;
	}

	KNode *KNode::addNewChild() {
		// allocate new node from pool
		auto newNode = new(_kshared->nodePool.allocate_node()) KNode(this, _kchilds.size());

		// save new node to child list
		_kchilds.push_back(newNode);

		return newNode;
	}

	void KNode::removeChild(KNode *Nodes) {
		if (Nodes->getParent() == this) {
			// remove from child list.
			// swap/pop
			_kchilds.back()->_kselfIndex = Nodes->_kselfIndex;
			_kchilds[Nodes->_kselfIndex] = _kchilds.back();
			_kchilds.pop_back();

			// deallocate node
			Nodes->~KNode();
			_kshared->nodePool.deallocate_node(Nodes);

		} else {
			KD_PRINT("this node is not owner of the given node");
		}
	}

	void KNode::clearChilds() {
		for (auto it = _kchilds.begin(); it != _kchilds.end(); ++it) {
			(*it)->~KNode();
			_kshared->nodePool.deallocate_node((*it));
		}

		_kchilds.clear();
	}

	void KNode::clearComponents(CClearMode Mode) {
		// logic components
		if (Mode == CClearMode::LOGIC || Mode == CClearMode::ALL) {
			for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {

				// call deattach on all components
				auto comPtr = _kshared->cstorage[(SIZE)CTypes::Logic]->get(it->second);
				comPtr->deattached();

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
				comPtr->deattached();

				// remove from storage
				storage->remove(_kfixedComp[i]);
				_kfixedComp[i] = KHandle();
			}
		}
	}

	KMETA_KNODE_SOURCE();
}