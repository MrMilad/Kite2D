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
#include "Kite/core/kistream.h"
#include "Kite/core/kostream.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/ecs/kcomponent.h"
#include "Kite/ecs/klistener.h"
#include "Kite/ecs/kmessenger.h"
#include "Kite/ecs/kcfstorage.h"
#include "Kite/ecs/kresource.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <vector>
#include <array>
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/namespace_alias.hpp>
#include "knode.khgen.h"
#include "ktypes.khgen.h"

#define KNODE_MEM_CHUNK 64 // pool size (number of nodes)

KMETA
namespace Kite {
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KNode : public KResource, public KMessenger, public KListener {
		KM_INFO(KI_NAME = "Node");
		KMETA_KNODE_BODY();
	public:
		/// this constructure will create a root node
		KM_CON(std::string)
		KNode(const std::string &Name);

		KNode(const KNode &CopyNode) = delete;
		KNode &operator=(const KNode &RightNode) = delete;

		/// will be called recursive if there are attached childs 
		~KNode();

		/// redirect recieved message to all sub-components
		KM_FUN()
		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		KM_FUN()
		bool saveStream(KOStream &Stream, const std::string &Address) override;

		KM_PRO_GET(KP_NAME = "parent", KP_TYPE = KHandle, KP_CM = "parent of the node")
			inline KNode *getParent() const { return _kparent; }

		KM_PRO_GET(KP_NAME = "zorder", KP_TYPE = U32, KP_CM = "render order of the node")
			inline U32 getZOrder() const { return _kzorder; }

		KM_PRO_SET(KP_NAME = "zorder")
			void setZOrder(U32 Order);

		KM_PRO_SET(KP_NAME = "name")
		void setName(const std::string &Name);

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "name of the node")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "listener", KP_TYPE = KListener, KP_CM = "node listener")
		inline KListener &getListener() { return *(KListener *)this; }

		KM_PRO_GET(KP_NAME = "messenger", KP_TYPE = KMessenger, KP_CM = "node messenger")
		inline KMessenger &getMessenger() { return *(KMessenger *)this; }

		KM_PRO_GET(KP_NAME = "isActive", KP_TYPE = bool, KP_CM = "active state of the node")
		inline bool isActive() const { return _kactive; }

		/// affect childs
		KM_PRO_SET(KP_NAME = "isActive")
		void setActive(bool Active);

		KM_PRO_GET(KP_NAME = "isRoot", KP_TYPE = bool, KP_CM = "this is root")
		inline bool isRoot() const { return (_kparent == nullptr); }

		// recursive
		KM_PRO_GET(KP_NAME = "root", KP_TYPE = KNode, KP_CM = "root node")
		inline KNode *getRoot() const { return _kshared->root; }

		KM_PRO_GET(KP_NAME = "envTableName", KP_TYPE = std::string, KP_CM = "name of the env table in lua")
		inline const std::string &getLuaTName() const { return _kluatable; }

		KM_PRO_GET(KP_NAME = "layer", KP_TYPE = U8, KP_CM = "layer id")
		inline U8 getLayer() const { return _klayerid; }

		KM_PRO_SET(KP_NAME = "layer")
		void setLayer(U8 LayerID);

		KM_PRO_GET(KP_NAME = "childCount", KP_TYPE = U32, KP_CM = "number of childs")
		inline U32 getChildCount() const { return _kchilds.size(); }

		KM_PRO_GET(KP_NAME = "static", KP_TYPE = bool, KP_CM = "is static")
		inline bool getStatic() const { return _kstatic; }

		KM_PRO_SET(KP_NAME = "static")
		void setStatic(bool Static);

		template<typename T>
		T *addComponent(const std::string &Name = "") {
			// check component slot
			if (hasComponent<T>(Name)) {
				KD_FPRINT("requested components slot is not empty. type name: %s", getCTypesName(Type).c_str());
				return getComponent<T>(Name);
			}

			// check interface
			const std::vector<ITypes> &ilist = T::getIntList();
			for (auto it = ilist.begin(); it != ilist.end(); ++it) {
				if (hasComponentByInterface((*it))) {
					KD_FPRINT("requested interface slot is not empty. type name: %s", getITypesName((*it)).c_str());
					return nullptr;
				}
			}

			// check dependency
			const std::vector<CTypes> &dlist = T::getDepList();
			for (auto it = dlist.begin(); it != dlist.end(); ++it) {
				if (!hasComponentByType((*it))) {
					KD_FPRINT("requested dependency components is not exist. type name: %s", getCTypesName((*it)).c_str());
					return nullptr;
				}
			}

			return static_cast<T *>(_addComponent(T::getType(), Name)); 
		}

		template<typename T>
		T *getComponent(const std::string &Name = "") const {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			U16 tindex = (U16)T::getType();
			// script component
			if (tindex == (U16)CTypes::Logic) {
				auto found = _klogicComp.find(Name);
				if (found != _klogicComp.end()) {
					return static_cast<T *>(_kshared->cstorage[tindex]->get(found->second));
				}

				// non-script component
			} else {
				if (_kfixedComp[tindex] != KHandle()) {
					return static_cast<T *>(_kshared->cstorage[tindex]->get(_kfixedComp[tindex]));
				}
			}

			KD_FPRINT("there is no component of this type in the given node. ctype: %s", getCTypesName(Type).c_str());
			return nullptr;
		}

		KM_FUN()
		KComponent *getComponentByType(CTypes Type, const std::string &Name = "") const;

		KM_FUN()
		KComponent *getComponentByInterface(ITypes Type) const;

		KM_FUN()
		KComponent *getComponentByHandle(const KHandle &Handle) const;

		/// pass empty string if you need checking logic existence
		template<typename T>
		bool hasComponent(const std::string &Name = "") const {
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			if (T::getType() == CTypes::Logic) {
				if (Name.empty()) {
					return (!_klogicComp.empty());
				} else {
					return (_klogicComp.find(Name) != _klogicComp.end());
				}
			} else {
				if (_kfixedComp[(U16)T::getType()] != KHandle()) {
					return true;
				}
			}

			return false;
		}

		/// pass empty string if you need checking logic existence
		KM_FUN()
		bool hasComponentByType(CTypes Type, const std::string &Name = "") const;

		KM_FUN()
		bool hasComponentByInterface(ITypes Type) const;

		template<typename T>
		void removeComponent(const std::string &Name = "") {
			auto com = getComponent<T>(Name);
			if (com) {
				removeComponentByHandle(com->getHandle());
			}
		}

		KM_FUN()
		void removeComponentByType(CTypes Type, const std::string &Name = "");

		KM_FUN()
		void removeComponentByInterface(ITypes Type);

		KM_FUN()
		void removeComponentByHandle(const KHandle &Handle);

		KM_FUN()
		void clearComponents(CClearMode Mode);

		void getFixedComponents(std::vector<KHandle> &Output) const;

		/// return vector of KHandle to avoiding pointer dangling during update
		void getLogicComponents(std::vector<KHandle> &Output) const;

		/// make a copy of child-nodes from this hierarchy or another hierarchy
		/// will not remove node from old parent.
		/// use addNewChild if you need a fresh new child.
		/// return moved node.
		KM_FUN()
		KNode *addChild(KNode *Nodes);

		KM_FUN()
		KNode *addNewChild();

		KM_FUN()
		void removeChild(KNode *Nodes);

		KM_FUN()
		void clearChilds();

		inline const auto childList() const { return &_kchilds; }

		template <typename T>
		bool registerComponent() {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			auto tindex = (U16)T::getType();

			// check type
			if (_kshared->cstorage[tindex] != nullptr) {
				KD_FPRINT("this type has already been registered. ctype: %s", getCTypesName(T::getType()).c_str());
				return false;
			}

			// register type
			_kshared->cstorage[tindex] = new Internal::CHolder<T, KComponent, T::getType()>;
			return true;
		}

	private:
		/// internal constructure for creating childs
		KNode(KNode *Parent, SIZE SelfIndex);

		/// resource manager
		KNode(const std::string &Name, const std::string &Address);

		void serial(KBaseSerial &Serializer);
		void deserial(KBaseSerial &Serializer, KResourceManager *RMan);

		struct SharedData {
			U32 zorder;		// total number of z order in the hierarchy
			KNode *root;
			std::array<Internal::BaseCHolder<KComponent> *, (U16)CTypes::maxSize> cstorage;	// component storages (array)
			std::vector<KHandle> trashList;
			memory::memory_pool<> nodePool; // data pool

			SharedData() :
				zorder(0),
				cstorage({}),
				nodePool(sizeof(KNode), KNODE_MEM_CHUNK * sizeof(KNode))
				{}

			~SharedData() {
				for (auto i = 0; i < (U16)CTypes::maxSize; ++i) {
					delete cstorage[i];
				}
			}
		};

		// raw add component (without calling system functions and no dep/int checking)
		// internal use only: serialization and copy node
		KComponent *_addComponent(CTypes Type, const std::string &Name) {
			// check component slot
			if (hasComponentByType(Type, Name)) {
				KD_FPRINT("requested components slot is not empty. type name: %s", getCTypesName(Type).c_str());
				return getComponentByType(Type, Name);
			}

			// create component
			U16 tindex = (U16)Type;
			auto handle = _kshared->cstorage[tindex]->add(this, Name);

			// set its handle
			auto com = _kshared->cstorage[tindex]->get(handle);
			com->_khandle = handle;

			// attach created component to the node
			if (Type == CTypes::Logic) {
				_klogicComp[Name] = handle;
			} else {
				_kfixedComp[tindex] = handle;
			}

			// set interface
			const std::vector<ITypes> &ilist = com->getDrivedIntList();
			for (auto it = ilist.begin(); it != ilist.end(); ++it) {
				_kinterface[(U16)(*it)] = handle;
			}

			// increase dependency ref counts
			const std::vector<CTypes> &dlist = com->getDrivedDepList();
			for (auto it = dlist.begin(); it != dlist.end(); ++it) {
				auto depCom = getComponentByType((*it), "");
				++depCom->_krefcounter;
			}

			// call attach functon
			com->attached();
			return com;
		}

		// initialize on deserialization stage (recursive)
		bool _loadStream(std::unique_ptr<KIStream> Stream, KResourceManager *RManager) override;

		// deserializable variable
		bool _kstatic;
		bool _kactive;
		U8 _klayerid;
		U32 _kzorder;
		std::string _kname;

		// runtime variables 
		std::string _kluatable;										// node env table name in lua
		KHandle _kinterface[(U16)ITypes::maxSize];					// interface slots (pointer to fixed slots)
		KHandle _kfixedComp[(U16)CTypes::maxSize];					// fixed components slots (built-in components)
		std::unordered_map<std::string, KHandle> _klogicComp;		// dynamic components (logic components)
		KNode *_kparent;											// node parent handle
		SIZE _kselfIndex;											// node self index (index of childs-list of the parent)
		std::vector<KNode *> _kchilds;								// child list (allocated from node pool)

		SharedData *_kshared;
	};
}

#endif // KNODE_H