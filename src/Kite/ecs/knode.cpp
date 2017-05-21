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
#include "Kite/ecs/ksystem.h"
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

	Internal::SharedData::SharedData() :
		maxz(0),
		nodePool(sizeof(KNode), KNODE_MEM_CHUNK * sizeof(KNode)),
		comPool{ REGISTER_COMPONENTS },
		sysList(nullptr),
		lvm(nullptr)
	{
		nodes.reserve(KNODE_MEM_CHUNK);
		nodesFreeId.reserve(KNODE_MEM_CHUNK);
	}

	Internal::SharedData::~SharedData() {
		for (U16 i = 0; i < (U16)Component::maxSize; ++i) {
			delete comPool[i];
		}
	}

	KNode *KNode::_kactiveHierarchy = nullptr;
	const std::vector<Component> *KNode::_kdepList[(U16)Component::maxSize]{ DEPENDENCY_LIST };
	const std::vector<Interface> *KNode::_kintList[(U16)Component::maxSize]{ INTERFACE_LIST };
	KNode::KNode(const std::string &Name) :
		KResource("", ""),
		_kparent(nullptr),
		_kshared(new Internal::SharedData()),
		_kname(Name),
		_klayerid(0),
		_kselfIndex(0),
		_kactive(true),
		_kid(0),
		_kzorder(0)
	{
		initeSlots();
		_kshared->root = this;
		_kshared->nodes.push_back(this);
	}

	KNode::KNode(const std::string &Name, const std::string &Address):
		KResource(Name, Address),
		_kparent(nullptr),
		_kshared(new Internal::SharedData()),
		_klayerid(0),
		_kselfIndex(0),
		_kactive(true),
		_kid(0),
		_kzorder(0)
	{
		initeSlots();
		_kshared->root = this;
		_kshared->nodes.push_back(this);
	}

	KNode::KNode(KNode *Parent, SIZE SelfIndex):
		KResource("", ""),
		_kparent(Parent),
		_kselfIndex(SelfIndex),
		_kshared(Parent->_kshared),
		_klayerid(Parent->_klayerid),
		_kactive(Parent->_kactive)
	{
		initeSlots();

		// get id from free list
		if (!_kshared->nodesFreeId.empty()) {
			_kid = _kshared->nodesFreeId.back();
			_kshared->nodesFreeId.pop_back();
			_kshared->nodes[_kid] = this;

		} else {
			_kid = _kshared->nodes.size();
			_kshared->nodes.push_back(this);
		}
		
		_kzorder = _kshared->maxz++;;
	}

	KNode::~KNode() {
		// clear all components
		clearComponents(ClearMode::ALL);

		// clear all childs
		clearChilds();

		// remove from node id/list
		_kshared->nodes[_kid] = nullptr;
		_kshared->nodesFreeId.push_back(_kid);

		// only root can create/destroy storages
		if (isRoot()) {
			delete _kshared;
			_kshared = nullptr;
		}
	}

	int KNode::luaAddComponent(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// addComponent(self, KComponent *)
		// or addComponent(self, Component type) without name
		if (n == 2) {
#ifdef KITE_DEV_DEBUG
			try {
#endif
				if (lua_isnumber(L, 2)) {
					auto type = LuaIntf::Lua::get(L, 2).toValue<Component>();
					auto newCom = addComponent(type);
					LuaIntf::Lua::push(L, newCom);
					return 1;
				} else {
					auto com = LuaIntf::Lua::get(L, 2).toValue<KComponent *>();
					auto newCom = addComponent(com);
					LuaIntf::Lua::push(L, newCom);
					return 1;
				}
#ifdef KITE_DEV_DEBUG
			}catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, nullptr);
				return 1;
			}
#endif

		// addComponent(self, Component Type, string Name)
		} else if (n == 3) {
			Component type;
			std::string name;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
				name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, nullptr);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
#endif

			auto newCom = addComponent(type, name);
			LuaIntf::Lua::push(L, newCom);
			return 1;
		}

		KD_PRINT("incorrect argument.");
		LuaIntf::Lua::push(L, nullptr);
		return 1;
	}

	int KNode::luaGetComponent(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// getComponent(self, Component Type)
		if (n == 2) {
			Component type;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, nullptr);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
#endif

			auto newCom = getComponent(type);
			LuaIntf::Lua::push(L, newCom);
			return 1;

			// getComponent(self, Component Type, string Name)
		} else if (n == 3) {
			Component type;
			std::string name;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
				name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, nullptr);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
#endif

			auto newCom = getComponent(type, name);
			LuaIntf::Lua::push(L, newCom);
			return 1;
		}

		KD_PRINT("incorrect argument.");
		LuaIntf::Lua::push(L, nullptr);
		return 1;
	}

	int KNode::luaHasComponent(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// hasComponent(self, Component Type)
		if (n == 2) {
			Component type;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, false);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
#endif
			LuaIntf::Lua::push(L, hasComponent(type));
			return 1;

			// hasComponent(self, Component Type, string Name)
		} else if (n == 3) {
			Component type;
			std::string name;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
				name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, false);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
#endif

			LuaIntf::Lua::push(L, hasComponent(type, name));
			return 1;
		}

		KD_PRINT("incorrect argument.");
		LuaIntf::Lua::push(L, false);
		return 1;
	}

	int KNode::luaRemComponent(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// removeComponent(self, KComponent *)
		// or removeComponent(self, Component type) without name
		if (n == 2) {
#ifdef KITE_DEV_DEBUG
			try {
#endif
				if (lua_isnumber(L, 2)) {
					auto type = LuaIntf::Lua::get(L, 2).toValue<Component>();
					LuaIntf::Lua::push(L, removeComponent(type));
					return 1;
				} else {
					auto com = LuaIntf::Lua::get(L, 2).toValue<KComponent *>();
					LuaIntf::Lua::push(L, removeComponent(com));
					return 1;
				}
#ifdef KITE_DEV_DEBUG
			}catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, false);
				return 1;
			}
#endif

			// removeComponent(self, Component Type, string Name)
		} else if (n == 3) {
			Component type;
			std::string name;
#ifdef KITE_DEV_DEBUG
			try {
				type = LuaIntf::Lua::get(L, 2).toValue<Component>();
				name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, false);
				return 1;
			}
#else
			type = LuaIntf::Lua::get(L, 2).toValue<Component>();
			name = LuaIntf::Lua::get(L, 3).toValue<const std::string>();
#endif
			LuaIntf::Lua::push(L, removeComponent(type, name));
			return 1;
		}

		KD_PRINT("incorrect argument.");
		LuaIntf::Lua::push(L, false);
		return 1;
	}

	int KNode::luaAddChild(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// addChild(self)
		if (n == 1) {
			LuaIntf::Lua::push(L, addChild());
			return 1;

			// addChild(self, Node *)
		} else if (n == 2) {
			KNode *node;
#ifdef KITE_DEV_DEBUG
			try {
				node = LuaIntf::Lua::get(L, 2).toValue<KNode *>();
			}
			catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				LuaIntf::Lua::push(L, nullptr);
				return 1;
			}
#else
			node = LuaIntf::Lua::get(L, 2).toValue<KNode *>();
#endif

			LuaIntf::Lua::push(L, addChild(node));
			return 1;
		}

		KD_PRINT("incorrect argument.");
		LuaIntf::Lua::push(L, nullptr);
		return 1;
	}

	int KNode::luaAddListener(lua_State *L) {
		// check hierarchy state
		if (!isOnActiveHierarchy()) {
			KD_FPRINT("node:addListener Error: node.id: %u : inactive node", getID());
			return 0;
		}

		// get number of arguments
		int n = lua_gettop(L);

		// addListener(self, Node, Type, function)
		if (n == 4) {
			KNode *node;
			const char *type;
			LuaIntf::LuaRef fun;
#ifdef KITE_DEV_DEBUG
			try {
#endif
				node = LuaIntf::Lua::get(L, 2).toValue<KNode *>();
				if (!node->isOnActiveHierarchy()) {
					KD_FPRINT("node:addListener Error: node.id: %u : inactive node", node->getID());
					return 0;
				}
				type = LuaIntf::Lua::get(L, 3).toValue<const char *>();
				fun = LuaIntf::Lua::get(L, 4);

				// create node table if its not exist
				auto nodestbl = LuaIntf::LuaRef(L, "_G.nodes");
				LuaIntf::LuaRef nodetbl = nodestbl.get(getID());
				if (!nodetbl.isTable()) {
					nodetbl = LuaIntf::LuaRef::createTable(L);
					nodestbl.set(getID(), nodetbl);
				}

				// cretae type table (1 table per type)
				LuaIntf::LuaRef typetbl = nodetbl.get(type);
				if (!typetbl.isTable()) {
					typetbl = LuaIntf::LuaRef::createTable(L);
					nodetbl.set(type, typetbl);
				}

#ifdef KITE_DEV_DEBUG
				// add function to its type table
				if (typetbl.has(node->getID())) {
					KD_FPRINT("node:addListener() Warning: node.id: %u function handle replaced", node->getID());
				}
#endif

				typetbl.set(node->getID(), fun);
#ifdef KITE_DEV_DEBUG
			}catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				return 0;
			}
#endif
			return 0;
		}

		KD_PRINT("incorrect argument.");
		return 0;
	}

	int KNode::luaRemoveListener(lua_State *L) {
		// check hierarchy state
		if (!isOnActiveHierarchy()) {
			KD_FPRINT("node:removeListener Error: node.id: %u : inactive node", getID());
			return 0;
		}

		// get number of arguments
		int n = lua_gettop(L);

		// removeListener(self, Node, Type)
		if (n == 3) {
			KNode *node;
			const char *type;
#ifdef KITE_DEV_DEBUG
			try {
#endif
				node = LuaIntf::Lua::get(L, 2).toValue<KNode *>();
				if (!node->isOnActiveHierarchy()) {
					KD_FPRINT("node:removeListener Error: node.id: %u : inactive node", node->getID());
					return 0;
				}
				type = LuaIntf::Lua::get(L, 3).toValue<const char *>();

				// get node table if its not exist
				auto nodestbl = LuaIntf::LuaRef(L, "_G.nodes");
				LuaIntf::LuaRef nodetbl = nodestbl.get(getID());
				if (nodetbl.isTable()) {
					// get type table (1 table per type)
					LuaIntf::LuaRef typetbl = nodetbl.get(type);
					if (typetbl.isTable()) {
						typetbl.remove(node->getID());
					}
				}
#ifdef KITE_DEV_DEBUG
			}catch (std::exception& e) {
				KD_FPRINT("incorrect argument type: %s", e.what());
				return 0;
			}
#endif
			return 0;
		}

		KD_PRINT("incorrect argument.");
		return 0;
	}

	int KNode::luaGetChild(lua_State *L) {
		if (_kchilds.empty()) {
			LuaIntf::Lua::push(L, nullptr);
			return 1;
		}

		// lua arrays start from 1
		auto arrayTable = LuaIntf::LuaRef::createTable(L, _kchilds.size());
		for (auto i = 1; i <= _kchilds.size(); ++i) {
			arrayTable.set(i, _kchilds[i - 1]);
		}

		LuaIntf::Lua::push(L, arrayTable);
		return 1;
	}

	int KNode::luaConstruct(lua_State *L) {
		// get number of arguments
		int n = lua_gettop(L);

		// node(self)
		if (n == 1) {
			LuaIntf::Lua::push(L, KSharedResource(new KNode("")));
			return 1;

		// // node(self, string Name)
		} else if(n = 2){
			const char *name;
			name = LuaIntf::Lua::get(L, 2).toValue<const char *>();
			LuaIntf::Lua::push(L, KSharedResource(new KNode(std::string(name))));
			return 1;
		}

		KD_PRINT("incorrect argument.");
		return 0;
	}

	void KNode::serial(KBaseSerial &Serializer) {
		// node information
		Serializer << _kname;
		Serializer << _kactive;
		Serializer << _klayerid;

		// serialize fixed components
		std::vector<KComponent *> hlist;
		getFixedComponents(hlist);

		// write size
		Serializer << hlist.size();

		// write components data
		for (auto it = hlist.begin(); it != hlist.end(); ++it) {
			Serializer << (*it)->getDrivedType();
			(*it)->serial(Serializer, true);
		}

		// logic components
		getLogicComponents(hlist);

		// write size
		Serializer << hlist.size();

		// write components data
		for (auto it = hlist.begin(); it != hlist.end(); ++it) {
			Serializer << (*it)->getName();
			(*it)->serial(Serializer, true);
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
		Serializer >> _kname;
		Serializer >> _kactive;
		Serializer >> _klayerid;

		// deserialize fixed components
		SIZE fsize = 0;
		Serializer >> fsize;
		for (SIZE i = 0; i < fsize; ++i) {
			Component type;
			Serializer >> type;
			
			auto com = _addComponent(type, "", false);
			com->deserial(Serializer, true);	// deserial base because we need ref counter value
			com->loadRes(RMan);
		}

		// deserialize logic components
		Serializer >> fsize;
		for (SIZE i = 0; i < fsize; ++i) {
			std::string name;
			Serializer >> name;
			auto com = _addComponent(Component::LOGIC, name, false);
			com->deserial(Serializer, true);	// deserial base because we need ref counter value
			com->loadRes(RMan);
		}

		// read childs size
		Serializer >> fsize;
		for (SIZE i = 0; i < fsize; ++i) {
			auto child = addChild();
			child->deserial(Serializer, RMan);
		}
	}

	void KNode::initeSlots() {
		for (U16 i = 0; i < (U16)Component::maxSize; ++i) {
			_kfixedComp[i] = nullptr;
		}

		for (U16 i = 0; i < (U16)Interface::maxSize; ++i) {
			_kinterface[i] = nullptr;
		}
	}

	bool KNode::saveStream(KIOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << std::string("KNode");

		// recursive
		serial(bserial);

		return bserial.saveStream(Stream, Address, 0);
	}

	bool KNode::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager){
		KBinarySerial bserial;
		if (!bserial.loadStream(*Stream, getAddress())) {
			KD_PRINT("can't load stream");
			return false;
		}

		std::string format;
		bserial >> format;

		if (format != "KNode") {
			KD_PRINT("incrrect file format");
			return false;
		}

		deserial(bserial, RManager);

		// notfying systems about new created components
		// we notyfi system here because components will deserialized in random order dependencies ignored

		return true;
	}

	void KNode::setActive(bool Active) {
		if (_kactive != Active) {
			_kactive = Active;

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

		_klayerid = LayerID;
	}

	void KNode::onActivate(const std::vector<KSystem *> *List) {
		_kshared->sysList = List;
		if (List) _kactiveHierarchy = _kshared->root;
	}

	KComponent *KNode::_addComponent(Component Type, const std::string &Name, bool NotifySystems) {
		// allocate new component
		U16 tindex = (U16)Type;
		auto newCom = _kshared->comPool[tindex]->create(this, Name);

		// attach created component to the node
		if (Type == Component::LOGIC) {
			_klogicComp[Name] = newCom;
		} else {
			_kfixedComp[tindex] = newCom;
		}

		// set interface (we set interface here because this function used in deserialization stage)
		for (auto it = _kintList[tindex]->begin(); it != _kintList[tindex]->end(); ++it) {
			_kinterface[(U16)(*it)] = newCom;
		}

		// call attach functon
		newCom->attached();

		// notify systems
		if (NotifySystems && _kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->componentAdded(newCom);
			}
		}

		return newCom;
	}

	void KNode::_remComponent(KComponent *Com) {
		U16 tindex = (U16)Com->getDrivedType();

		// deattach component from node and call deattach function
		Com->deattached();

		// remove components id from node
		if (Com->getDrivedType() == Component::LOGIC) {
			_klogicComp.erase(Com->getName());

		} else {
			_kfixedComp[tindex] = nullptr;
		}

		// notify systems
		if (_kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->componentRemoved(Com);
			}
		}

		_kshared->comPool[tindex]->remove(Com);
	}

	KComponent *KNode::addComponent(KComponent *Component) {
		auto Type = Component->getDrivedType();
		auto Name = Component->getName();

#ifndef KITE_CHECKLESS
		// check component slot
		if (hasComponent(Type, Name)) {
			KD_FPRINT("requested components slot is not empty. type name: %s", getComponentName(Type).c_str());
			return nullptr;
		}

		// check interface
		for (auto it = _kintList[(U16)Type]->begin(); it != _kintList[(U16)Type]->end(); ++it) {
			if (hasInterface((*it))) {
				KD_FPRINT("requested interface slot is not empty. type name: %s", getInterfaceName((*it)).c_str());
				return nullptr;
			}
		}

		// check dependency
		for (auto it = _kdepList[(U16)Type]->begin(); it != _kdepList[(U16)Type]->end(); ++it) {
			if (!hasComponent((*it))) {
				KD_FPRINT("requested dependency components is not exist. type name: %s", getComponentName((*it)).c_str());
				return nullptr;
			}
		}
#endif

		// add new component
		auto newCom = _addComponent(Type, Name, false);

		// copy from source
		newCom->copyFrom(Component);

		// notify systems
		if (_kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->componentAdded(newCom);
			}
		}

		// increase dependency ref counts
		for (auto it = _kdepList[(U16)Type]->begin(); it != _kdepList[(U16)Type]->begin(); ++it) {
			auto depCom = getComponent((*it), "");
			++depCom->_krefcounter;
		}

		return newCom;
	}

	KComponent *KNode::addComponent(Component Type, const std::string &Name) {
#ifndef KITE_CHECKLESS
		// check component slot
		if (hasComponent(Type, Name)) {
			KD_FPRINT("requested components slot is not empty. type name: %s", getComponentName(Type).c_str());
			return nullptr;
		}

		// check interface
		for (auto it = _kintList[(U16)Type]->begin(); it != _kintList[(U16)Type]->end(); ++it) {
			if (hasInterface((*it))) {
				KD_FPRINT("requested interface slot is not empty. type name: %s", getInterfaceName((*it)).c_str());
				return nullptr;
			}
		}

		// check dependency
		for (auto it = _kdepList[(U16)Type]->begin(); it != _kdepList[(U16)Type]->end(); ++it) {
			if (!hasComponent((*it))) {
				KD_FPRINT("requested dependency components is not exist. type name: %s", getComponentName((*it)).c_str());
				return nullptr;
			}
		}
#endif

		// add new component
		auto newCom = _addComponent(Type, Name, false);

		// notify systems
		if (_kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->componentAdded(newCom);
			}
		}

		// increase dependency ref counts
		for (auto it = _kdepList[(U16)Type]->begin(); it != _kdepList[(U16)Type]->begin(); ++it) {
			auto depCom = getComponent((*it), "");
			++depCom->_krefcounter;
		}

		return newCom;
	}

	KComponent *KNode::getComponent(Component Type, const std::string &Name) const {
#ifndef KITE_CHECKLESS
		if (Type >= Component::maxSize) {
			KD_PRINT("component type is incorrect.");
			return nullptr;
		}
#endif

		// script component
		if (Type == Component::LOGIC) {
			auto found = _klogicComp.find(Name);
			if (found != _klogicComp.end()) {
				return found->second;
			}

			// non-script component
		} else {
			if (_kfixedComp[(U16)Type] != nullptr) {
				return _kfixedComp[(U16)Type];
			}
		}

		KD_FPRINT("there is no component of this type in the given node. ctype: %s", getComponentName(Type).c_str());
		return nullptr;
	}

	KComponent *KNode::getInterface(Interface Type) const {
#ifndef KITE_CHECKLESS
		if (Type >= Interface::maxSize) {
			KD_PRINT("interface type is incorrect");
			return nullptr;
		}
#endif
		
		return _kinterface[(U16)Type];
	}

	KNode *KNode::getNode(U32 ID) {
		if (ID < _kshared->nodes.size()) {
			return _kshared->nodes[ID];
		}
		KD_FPRINT("this id: %u is not exist in this hierarchy", ID);
		return nullptr;
	}

	KNode *KNode::getActiveRoot() {
		if (_kactiveHierarchy) {
			return _kactiveHierarchy->getRoot();
		}

		KD_PRINT("there is no active hierarchy now");
		return nullptr;
	}

	void KNode::getFixedComponents(std::vector<KComponent *> &Output) const {
		Output.clear();
		Output.reserve((SIZE)Component::maxSize);
		for (SIZE i = 0; i < (SIZE)Component::maxSize; ++i) {
			if (_kfixedComp[i] != nullptr) {
				Output.push_back(_kfixedComp[i]);
			}
		}
	}

	void KNode::getLogicComponents(std::vector<KComponent *> &Output) const {
		Output.clear();
		Output.reserve(_klogicComp.size());
		for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {
			Output.push_back(it->second);
		}
	}

	bool KNode::hasComponent(Component Type, const std::string &Name) const {
#ifndef KITE_CHECKLESS
		if (Type >= Component::maxSize) {
			KD_PRINT("componet type is incorrect.");
			return false;
		}
#endif
		if (Type == Component::LOGIC) {
			if (Name.empty()) {
				return (!_klogicComp.empty());
			} else {
				return (_klogicComp.find(Name) != _klogicComp.end());
			}
		} else {
			if (_kfixedComp[(SIZE)Type] != nullptr) {
				return true;
			}
		}

		return false;
	}

	bool KNode::hasInterface(Interface Type) const {
#ifndef KITE_CHECKLESS
		if (Type >= Interface::maxSize) {
			KD_PRINT("interface type is incorrect.");
			return false;
		}
#endif
		return (_kinterface[(U16)Type] != nullptr);
	}

	bool KNode::removeComponent(KComponent *Component) {
		if (Component->getOwnerNode() == this) {
			return removeComponent(Component->getDrivedType(), Component->getName());
		}
		KD_PRINT("this node is not owner of the given component");
		return false;
	}

	bool KNode::removeComponent(Component Type, const std::string &Name) {
#ifndef KITE_CHECKLESS
		if (Type >= Component::maxSize) {
			KD_PRINT("component type is incorrect.");
			return false;
		}
#endif
		// check ref counter
		auto comPtr = getComponent(Type, Name);
		if (!comPtr) {
			KD_FPRINT("there is no component of this type. ctype: %s", getComponentName(Type).c_str());
			return false;
		}

		if (comPtr->_krefcounter > 0) {
			KD_FPRINT("this component is required by another component. you should remove them first. ctype: %s"
					  , getComponentName(Type).c_str());
			return false;
		}

		// dec ref counter
		for (auto it = _kdepList[(U16)Type]->begin(); it != _kdepList[(U16)Type]->end(); ++it) {
			auto depCom = getComponent((*it));
			--depCom->_krefcounter;
		}

		// clear its interface
		for (auto it = _kintList[(U16)Type]->begin(); it != _kintList[(U16)Type]->end(); ++it) {
			_kinterface[(U16)(*it)] = nullptr;
		}

		// remove component from pool
		_remComponent(comPtr);
		return true;
	}

	bool KNode::removeInterface(Interface Type) {
#ifndef KITE_CHECKLESS
		if (Type >= Interface::maxSize) {
			KD_PRINT("interface type is incorrect.");
			return false;
		}
#endif
		if (_kinterface[(U16)Type] != nullptr) {
			return removeComponent(_kinterface[(U16)Type]->getDrivedType());
		}

		KD_FPRINT("there is no component of this type. itype: %s", getInterfaceName(Type).c_str());
		return false;
	}

	KNode *KNode::addChild(KNode *Nodes) {
		// allocate new node from pool
		auto newNode = new(_kshared->nodePool.allocate_node()) KNode(this, _kchilds.size());

		if (Nodes == nullptr) {
			// save new node to child list
			_kchilds.push_back(newNode);

			return newNode;
		}

		// copy node state
		newNode->_kname = Nodes->_kname;
		newNode->_kactive = Nodes->_kactive;
		newNode->_klayerid = Nodes->_klayerid;

		// copy fixed components
		for (U16 i = 0; i < (U16)Component::maxSize; ++i) {
			if (Nodes->_kfixedComp[i] != nullptr) {
				// add component to new node
				auto newCom = newNode->_addComponent((Component)i, "", false);

				// copy
				newCom->copyFrom(Nodes->_kfixedComp[i]);
			}
		}

		// copy logic components
		for (auto it = Nodes->_klogicComp.begin(); it != Nodes->_klogicComp.end(); ++it) {
			// add component to new node
			auto newCom = newNode->_addComponent(Component::LOGIC, it->first, false);

			// copy
			newCom->copyFrom(it->second);
		}

		// notify systems
		if (_kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->nodeAdded(newNode);
			}
		}

		// add childs (recursive)
		for (auto it = Nodes->_kchilds.begin(); it != Nodes->_kchilds.end(); ++it) {
			newNode->addChild((*it));
		}

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
	
	void KNode::clearComponents(ClearMode Mode) {
		// notifying systems
		if (_kshared->sysList) {
			for (auto it = _kshared->sysList->begin(); it != _kshared->sysList->end(); ++it) {
				(*it)->nodeRemoved(this);
			}
		}

		// logic components
		if (Mode == ClearMode::LOGIC || Mode == ClearMode::ALL) {
			for (auto it = _klogicComp.begin(); it != _klogicComp.end(); ++it) {

				// deattach component and move it to trash list
				it->second->deattached();
				_kshared->comPool[(U16)Component::LOGIC]->remove(it->second);
			}
			_klogicComp.clear();
		}

		// fixed components
		if (Mode == ClearMode::FIXED || Mode == ClearMode::ALL) {
			for (U16 i = 0; i < (U16)Component::maxSize; ++i) {
				if (_kfixedComp[i] != nullptr) {
					_kfixedComp[i]->deattached();
					_kshared->comPool[(U16)i]->remove(_kfixedComp[i]);
					_kfixedComp[i] = nullptr;
				}
			}

			// interfaces
			for (U16 i = 0; i < (U16)Interface::maxSize; ++i) {
				_kinterface[i] = nullptr;
			}
		}
	}

	KNODE_SOURCE();
}