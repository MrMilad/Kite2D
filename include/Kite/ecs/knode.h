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
#include "Kite/serialization/kserialization.h"
#include <string>
#include <list>
#include <array>
#include "kentity.khgen.h"
#include "ktypes.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KNode : public KMessenger, public KListener {
		friend class KEngine;
		friend void registerCTypes(KNode *);
	public:
		/// this constructure will create a root node
		KM_CON()
		KNode();

		KNode(const KNode &CopyNode) = delete;

		/// will be called recursive if there are attached childs 
		~KNode();

		/// will not affect childs
		KNode &operator=(const KNode &RightNode) = delete;

		/// redirect recieved message to all sub-components
		KM_FUN()
		RecieveTypes onMessage(KMessage *Message, MessageScope Scope);

		KM_FUN()
		bool saveStream(KOStream &Stream, const std::string &Address);

		/// will unload current state of the node
		KM_FUN()
		bool loadStream(KIStream &Stream, const std::string &Address);

		KM_PRO_GET(KP_NAME = "parent", KP_TYPE = KHandle, KP_CM = "parent of the node")
		inline KNode *getParent() const { return _kparent; }

		KM_PRO_GET(KP_NAME = "zorder", KP_TYPE = U32, KP_CM = "render order of the node")
		inline U32 getZOrder() const { return _kzorder; }

		KM_PRO_SET(KP_NAME = "zorder")
		inline void setZOrder(U32 Order) { _kzorder = Order; }

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "unique name of the node")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "listener", KP_TYPE = KListener, KP_CM = "node listener")
		inline KListener &getListener() { return *(KListener *)this; }

		KM_PRO_GET(KP_NAME = "messenger", KP_TYPE = KMessenger, KP_CM = "node messenger")
		inline KMessenger &getMessenger() { return *(KMessenger *)this; }

		KM_PRO_GET(KP_NAME = "isActive", KP_TYPE = bool, KP_CM = "active state of the node")
		inline bool isActive() const { return _kactive; }

		KM_PRO_SET(KP_NAME = "isActive")
		void setActive(bool Active);

		KM_PRO_GET(KP_NAME = "isRoot", KP_TYPE = bool, KP_CM = "this is root")
		inline bool isRoot() const { return (_kparent == nullptr); }

		// recursive
		KM_PRO_GET(KP_NAME = "root", KP_TYPE = KNode, KP_CM = "root node")
		inline KNode *getRoot() const { return _kshared->root; }

		// getCopy() is not safe (pointer dangling) also we cant resolve copy pointer at deserialization stage
		// this function is used for marking purpose in the editor
		KM_PRO_GET(KP_NAME = "isCopy", KP_TYPE = bool, KP_CM = "this is a copy from another node")
		inline bool isCopy() const { return _kisCopy; }

		KM_PRO_GET(KP_NAME = "envTableName", KP_TYPE = std::string, KP_CM = "name of the env table in lua")
		inline const std::string &getLuaTName() const { return _kluatable; }

		KM_PRO_GET(KP_NAME = "layer", KP_TYPE = U8, KP_CM = "layer id")
		inline U8 getLayer() const { return _klayerid; }

		KM_PRO_SET(KP_NAME = "layer")
		void setLayer(U8 LayerID);

		KM_PRO_GET(KP_NAME = "childCount", KP_TYPE = U32, KP_CM = "number of childs")
		inline U8 getChildCount() const { return _kchilds.size(); }

		KM_PRO_GET(KP_NAME = "static", KP_TYPE = bool, KP_CM = "is static")
		inline bool getStatic() const { return _kstatic; }

		KM_PRO_SET(KP_NAME = "static")
		void setStatic(bool Static);

		// dont add multiple graphics(renderable) component on the same node. (undefined behavior)
		KM_FUN()
		KComponent *addComponent(CTypes Type, const std::string &Name = "");

		KM_FUN()
		KComponent *getComponent(CTypes Type, const std::string &Name = "") const;

		KM_FUN()
		KComponent *getComponentByHandle(const KHandle &Handle) const;

		KM_FUN()
		bool hasComponent(CTypes Type, const std::string &Name = "") const;

		KM_FUN()
		bool hasComponentType(CTypes Type) const;

		KM_FUN()
		void removeComponent(CTypes Type, const std::string &Name = "");

		KM_FUN()
		void removeComponentByHandle(const KHandle &Handle);

		KM_FUN()
		void clearComponents(CClearMode Mode);

		void getFixedComponents(std::vector<KHandle> &Output) const;

		/// return vector of KHandle to avoiding pointer dangling during update
		void getLogicComponents(std::vector<KHandle> &Output) const;

		/// make a copy of child-nodes from this hierarchy or another hierarchy
		/// will not remove node from old parent.
		/// use addNewChild if you need a new child.
		/// return moved node.
		KM_FUN()
		KNode *addChild(KNode *Nodes);

		KM_FUN()
		KNode *addNewChild();

		KM_FUN()
		bool removeChild(KNode *Nodes);

		inline const auto childList() const { return &_kchilds; }

#ifdef KITE_EDITOR
		inline void setAddComCallback(ComCallback Callback, void *OpaqPtr) { _kaddCallb = Callback; _kaddOpaque = OpaqPtr; }
		inline void setRemoveComCallback(ComCallback Callback, void *OpaqPtr) { _kremCallb = Callback; _kremOpaque = OpaqPtr; }
#endif

	private:
		// internal constructure for creating childs
		KNode(KNode *Parent, std::list<KNode>::iterator *SelfIt);

		struct SharedData {
			U32 zorder;		// total number of z order in the hierarchy
			KNode *root;
			std::array<Internal::BaseCHolder<KComponent> *, (U16)CTypes::maxSize> cstorage;	// component storages (array)
			std::vector<KHandle> trashList;

			SharedData() :
				zorder(0),
				cstorage({}) {}

			~SharedData() {
				for (auto i = 0; i < (U16)CTypes::maxSize; ++i) {
					delete cstorage[i];
				}
			}
		};

		// raw add component (without calling system functions)
		KComponent *_addComponent(CTypes Type, const std::string &CName = "");

		// lua side
		// usage: cast from void * to KNode
		KM_FUN()
		static KNode *toNode(void *Data) { return (KNode *)(Data); }

		template <typename T>
		bool registerComponent(CTypes Type) {
			// check base of T
			static_assert(std::is_base_of<KComponent, T>::value, "T must be derived from KComponent");
			auto tindex = (U16)Type;

			// check type
			if (_kshared->cstorage[tindex] != nullptr) {
				KD_FPRINT("this type has already been registered. ctype: %s", getCTypesName(Type).c_str());
				return false;
			}

			// register type
			_kshared->cstorage[tindex] = new Internal::CHolder<T, KComponent, Type>;
			_kshared->cstorage[tindex]->type = typeid(T).hash_code();
			return true;
		}

		// initialize on deserialization stage (recursive)
		void initeOnDeserial();

		bool _kstatic;
		bool _kactive;											// node actitvity state
		bool _kisCopy;											// this node is a copy from another node
		std::string _kname;										// node name (maybe not unique)
		std::string _kluatable;									// node env table name in lua
		U8 _klayerid;
		U32 _kzorder;
		KHandle _kfixedComp[(SIZE)CTypes::maxSize];				// fixed components slots (built-in components)
		std::unordered_map<std::string, KHandle> _klogicComp;	// dynamic components (logic components)

		// runtime variables 
		KNode *_kparent;								// node parent handle
		std::list<KNode>::iterator *_kselfIter;			// node self handle (iterator of the childs-list of the parent)
		std::list<KNode> _kchilds;						// child list

		SharedData *_kshared;


		// editor only
#ifdef KITE_EDITOR
		ComCallback _kaddCallb;
		ComCallback _kremCallb;
		void *_kaddOpaque;
		void *_kremOpaque;
#endif
	};
}

#endif // KNODE_H