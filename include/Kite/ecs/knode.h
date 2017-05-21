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
#ifndef KNODE_H
#define KNODE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/stream/kiostream.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/ecs/ksystem.h"
#include "Kite/ecs/kcomponent.h"
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/namespace_alias.hpp>
#include "knode.khgen.h"
#include "ktypes.khgen.h"

#define KNODE_MEM_CHUNK 128 // pool size (number of nodes)

KMETA
namespace Kite {
	namespace Internal {
		struct BaseCom {
			virtual KComponent *create(KNode *Owner, const std::string &Name) = 0;
			virtual void remove(KComponent *Comp) = 0;
		};

		template<typename T>
		struct comType : public BaseCom {
			memory::memory_pool<> comPool;

			comType() :
				comPool(sizeof(T), KNODE_MEM_CHUNK * sizeof(T)) {
				static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			}

			KComponent *create(KNode *Owner, const std::string &Name) {
				return new(comPool.allocate_node()) T(Owner, Name);
			}

			void remove(KComponent *Comp) {
				auto com = static_cast<T *>(Comp);
				com->~T();
				comPool.deallocate_node(Comp);
			}
		};

		struct SharedData {
			U32 maxz;									// maximum z order in the hierarchy
			KNode *root;								// root
			std::vector<KNode *> nodes;					// store nodes by its id
			std::vector<U32> nodesFreeId;				// free nodes id (swap and remove will change ids of nodes so we should use an free list) 
			BaseCom *comPool[(U16)Component::maxSize];	// components pools
			memory::memory_pool<> nodePool;				// node pool
			const std::vector<KSystem *> *sysList;		// pinter to system list
			lua_State *lvm;

			SharedData();
			~SharedData();
		};
	}

	class KEngine;
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KNode : public KResource {
		friend KEngine;
		KM_INFO(KI_NAME = "Node");
		KNODE_BODY();
	public:
		/// this constructure will create a root node
		/// supported by lua
		KNode(const std::string &Name);

		KNode(const KNode &CopyNode) = delete;
		KNode &operator=(const KNode &RightNode) = delete;

		/// will be called recursive if there are attached childs 
		~KNode();

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

		KM_PRO_GET(KP_NAME = "parent", KP_TYPE = KHandle, KP_CM = "parent of the node")
		inline KNode *getParent() const { return _kparent; }

		KM_PRO_SET(KP_NAME = "name")
		inline void setName(const std::string &Name) { _kname = Name; }

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "name of the node")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "isActive", KP_TYPE = bool, KP_CM = "active state of the node")
		inline bool isActive() const { return _kactive; }

		/// affect childs
		KM_PRO_SET(KP_NAME = "isActive")
		void setActive(bool Active);

		KM_PRO_GET(KP_NAME = "isRoot", KP_TYPE = bool, KP_CM = "this is root")
		inline bool isRoot() const { return (_kparent == nullptr); }

		KM_PRO_GET(KP_NAME = "root", KP_TYPE = KNode, KP_CM = "root node")
		inline KNode *getRoot() const { return _kshared->root; }

		KM_PRO_GET(KP_NAME = "layer", KP_TYPE = U8, KP_CM = "layer id")
		inline U8 getLayer() const { return _klayerid; }

		KM_PRO_SET(KP_NAME = "layer")
		void setLayer(U8 LayerID);

		KM_PRO_GET(KP_NAME = "childsCount", KP_TYPE = U32, KP_CM = "number of childs")
		inline U32 getChildCount() const { return _kchilds.size(); }

		KM_PRO_GET(KP_NAME = "hierarchySize", KP_TYPE = U32, KP_CM = "number of nodes in this hierarchy")
		inline U32 getHierarchySize() const { return _kshared->nodes.size(); }

		KM_PRO_GET(KP_NAME = "id", KP_TYPE = U32, KP_CM = "node unique id in the hierarchy", KP_SHOW = false)
		inline U32 getID() const { return _kid; }

		KM_PRO_GET(KP_NAME = "zorder", KP_TYPE = U32, KP_CM = "node z (depth)", KP_SHOW = false)
		inline U32 getZOrder() const { return _kid; }

		KM_PRO_SET(KP_NAME = "zorder")
		inline void setZOrder(U32 ZOrder) { _kzorder = ZOrder; }

		KM_PRO_GET(KP_NAME = "isOnActiveHierarchy", KP_TYPE = bool, KP_CM = "node is on the current active hierarchy")
		inline bool isOnActiveHierarchy() const { return (_kshared->sysList != nullptr); }

		/// create a new component then copy Component to it
		/// return new created component
		KComponent *addComponent(KComponent *Component);

		/// create a new component and return it
		KComponent *addComponent(Component Type, const std::string &Name = "");

		/// Name will ignored with fixed component types
		KComponent *getComponent(Component Type, const std::string &Name = "") const;

		KM_FUN()
		KComponent *getInterface(Interface Type) const;

		/// pass empty string if you need checking logic existence
		bool hasComponent(Component Type, const std::string &Name = "") const;

		KM_FUN()
		bool hasInterface(Interface Type) const;

		bool removeComponent(KComponent *Component);

		/// Name will ignored with fixed component types
		bool removeComponent(Component Type, const std::string &Name = "");

		KM_FUN()
		bool removeInterface(Interface Type);

		/// make a copy of node(s) from this hierarchy or another hierarchy as a new child.
		/// will not remove copied node(s) from their parent.
		/// pass nullptr if you need a fresh new child.
		/// return new added child.
		KNode *addChild(KNode *Nodes = nullptr);

		KM_FUN()
		void removeChild(KNode *Nodes);

		KM_FUN()
		void clearChilds();

		/// return associated node with given ID from this hierarchy
		KM_FUN()
		KNode *getNode(U32 ID);

		/// return roots node of the currently active hierarchy
		KM_FUN()
		static KNode *getActiveRoot();

		inline const std::vector<KNode *> &getNodes() const { return _kshared->nodes; }

		void getFixedComponents(std::vector<KComponent *> &Output) const;

		/// return vector of KHandle to avoiding pointer dangling during update
		void getLogicComponents(std::vector<KComponent *> &Output) const;

		inline const auto childList() const { return &_kchilds; }

	private:
		/// internal constructure for creating childs
		KNode(KNode *Parent, SIZE SelfIndex);

		/// resource manager
		KNode(const std::string &Name, const std::string &Address);

		KM_FUN(KP_NAME = "addComponent")
		int luaAddComponent(lua_State *L);

		KM_FUN(KP_NAME = "getComponent")
		int luaGetComponent(lua_State *L);

		KM_FUN(KP_NAME = "hasComponent")
		int luaHasComponent(lua_State *L);

		KM_FUN(KP_NAME = "removeComponent")
		int luaRemComponent(lua_State *L);

		KM_FUN(KP_NAME = "addChild")
		int luaAddChild(lua_State *L);

		/// invoke another active node to this node for recieving message
		KM_FUN(KP_NAME = "addListener")
		int luaAddListener(lua_State *L);

		KM_FUN(KP_NAME = "removeListener")
		int luaRemoveListener(lua_State *L);

		/// return array (table) of childs
		KM_FUN(KP_NAME = "getChilds")
		int luaGetChild(lua_State *L);

		KM_FUN(KP_NAME = "__call")
		static int luaConstruct(lua_State *L);

		void serial(KBaseSerial &Serializer);
		void deserial(KBaseSerial &Serializer, KResourceManager *RMan);
		void initeSlots();
		void clearComponents(ClearMode Mode);

		/// internal usage by engine
		void onActivate(const std::vector<KSystem *> *List);

		// raw components operations (without calling system functions and no dep/int checking and by type)
		// internal use only: serialization and copy node
		KComponent *_addComponent(Component Type, const std::string &Name, bool NotifySystems);
		void _remComponent(KComponent *Com);

		// initialize on deserialization stage (recursive)
		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		// deserializable variable
		bool _kactive;
		U8 _klayerid;
		std::string _kname;

		// runtime variables 
		U32 _kid;													// node unique id in the hierarchy
		U32 _kzorder;												// node zorder
		KComponent *_kinterface[(U16)Interface::maxSize];			// interface slots (pointer to fixed slots)
		KComponent *_kfixedComp[(U16)Component::maxSize];			// fixed components slots (built-in components)
		std::unordered_map<std::string, KComponent *> _klogicComp;	// dynamic components (logic components)
		KNode *_kparent;											// node parent handle
		SIZE _kselfIndex;											// node self index (index of childs-list of the parent)
		std::vector<KNode *> _kchilds;								// child list (allocated from node pool)

		Internal::SharedData *_kshared;												// shared hierarchy data
		static KNode *_kactiveHierarchy;											// current active hierarchy
		static const std::vector<Component> *_kdepList[(U16)Component::maxSize];	// comonents dependency list
		static const std::vector<Interface> *_kintList[(U16)Component::maxSize];	// components interface list
	};
}

#endif // KNODE_H