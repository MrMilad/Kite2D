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
#ifndef KENTITY_H
#define KENTITY_H

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
#include <unordered_map>
#include <string>
#include <vector>
#include <bitset>
#include "kentity.khgen.h"
#include "ktypes.khgen.h"

KMETA
namespace Kite {
#ifdef KITE_EDITOR
	typedef void(*ComCallback)(KComponent *, void *);
#endif
	KM_CLASS(ENTITY, SCRIPTABLE)
	class KITE_FUNC_EXPORT KEntity: public KMessenger, public KListener{
		friend class KScene;
		KMETA_KENTITY_BODY();

	public:
		KEntity(const std::string &Name = "");
		virtual ~KEntity();

		/// redirect recieved message to all sub-components
		KM_FUN()
		RecieveTypes onMessage(KMessage *Message, MessageScope Scope);

		KM_PRO_GET(KP_NAME = "phandle", KP_TYPE = KHandle, KP_CM = "parent handle")
		inline KHandle getParentHandle() const { return _kphandle; }

		KM_PRO_GET(KP_NAME = "handle", KP_TYPE = KHandle, KP_CM = "entity handle")
		inline KHandle getHandle() const { return _khandle; }

		KM_PRO_GET(KP_NAME = "zorder", KP_TYPE = U32, KP_CM = "render order")
		inline U32 getZOrder() const { return _kzorder; }

		KM_PRO_SET(KP_NAME = "zorder")
		inline void setZOrder(U32 Order) { _kzorder = Order; }

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "entity unique name")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "listener", KP_TYPE = KListener, KP_CM = "entity message listener")
		inline KListener &getListener() { return *(KListener *)this; }

		KM_PRO_GET(KP_NAME = "messenger", KP_TYPE = KMessenger, KP_CM = "entity messenger")
		inline KMessenger &getMessenger() { return *(KMessenger *)this; }

		KM_PRO_GET(KP_NAME = "isActive", KP_TYPE = bool, KP_CM = "is active")
		inline bool isActive() const { return _kactive; }

		KM_PRO_SET(KP_NAME = "isActive")
		void setActive(bool Active);

		KM_PRO_GET(KP_NAME = "isRemoved", KP_TYPE = bool, KP_CM = "is removed")
		inline bool isRemoved() const { return _kdeleted; }

		KM_PRO_GET(KP_NAME = "isPrefab", KP_TYPE = bool, KP_CM = "is prefab")
		inline bool isPrefab() const {return _kisPrefab; }

		KM_PRO_GET(KP_NAME = "envTableName", KP_TYPE = std::string, KP_CM = "entity env table name in lua")
		inline const std::string &getLuaTName() const { return _kluatable; }

		KM_PRO_GET(KP_NAME = "prefab", KP_TYPE = std::string, KP_CM = "entity prefab name")
		inline const std::string &getPrefab() const { return _kprefabName; }

		KM_PRO_GET(KP_NAME = "layer", KP_TYPE = U8, KP_CM = "layer id")
		inline U8 getLayer() const { return _klayerid; }

		KM_PRO_SET(KP_NAME = "layer")
		void setLayer(U8 LayerID);

		KM_PRO_GET(KP_NAME = "static", KP_TYPE = bool, KP_CM = "is static")
		inline bool getStatic() const { return _kstatic; }

		KM_PRO_SET(KP_NAME = "static")
		void setStatic(bool Static);

		// dont add multiple graphics component on the same entity. (undefined behavior)
		// use an new entity
		KM_FUN()
		KComponent *addComponent(CTypes Type, const std::string &CName = "");

		KM_FUN()
		KComponent *getComponent(CTypes Type, const std::string &CName = "");

		KM_FUN()
		KComponent *getComponentByHandle(const KHandle &CHandle);

		KM_FUN()
		void getFixedComponents(std::vector<KHandle> &Output);

		/// return vector of KHandle to avoiding pointer dangling during update
		KM_FUN()
		void getScriptComponents(std::vector<KHandle> &Output);

		KM_FUN()
		bool hasComponent(CTypes Type, const std::string &Name = "");

		KM_FUN()
		bool hasComponentType(CTypes Type);

		KM_FUN()
		KHandle getComponentHandle(CTypes Type, const std::string &Name = "");

		KM_FUN()
		void removeComponent(CTypes Type, const std::string &ComponentName = "");

		KM_FUN()
		void removeComponentByHandle(const KHandle &CHandle);

		KM_FUN()
		void clearComponents();

		KM_FUN()
		void addChild(const KHandle &EHandle);

		KM_FUN()
		bool hasChild() const;

		KM_FUN()
		static KEntity *toEntity(void *Data);

		inline const auto childList() const { return &_kchilds; }

#ifdef KITE_EDITOR
		inline void setAddComCallback(ComCallback Callback, void *OpaqPtr) { _kaddCallb = Callback; _kaddOpaque = OpaqPtr; }
		inline void setRemoveComCallback(ComCallback Callback, void *OpaqPtr) { _kremCallb = Callback; _kremOpaque = OpaqPtr; }
#endif

	private:

		// internal use 
		void setComponent(KComponent *Comp);
		void remChildIndex(U32 ID);
		void setHandle(const KHandle Handle);
		void forceRemoveCom(CTypes Type); // force remove components and ignore dependency. (only fixed types)
		void initeComponents(); // initialize (call attach()) on deserialization stage

		void postClearComponents(); // clear all components immediately
		static void postWork(Internal::BaseCHolder<KComponent> **Storage);

		// internal script function (copy,cut,paste)
		KM_FUN()
		inline void setPrefabName(const std::string &Name) { _kprefabName = Name; _kisPrefab = true; }
		
		KM_VAR() bool _kstatic;
		KM_VAR() bool _kdeleted;
		KM_VAR() bool _kactive;											// entity actitvity state
		KM_VAR() bool _kisPrefab;										// is prefab instance
		KM_VAR() KHandle _khandle;										// entity handle in the entity manager
		KM_VAR() KHandle _kphandle;										// entity parent handle
		KM_VAR() U32 _kplistid;											// entity self id in the parent list
		KM_VAR() std::string _kprefabName;								// entity prefab name
		KM_VAR() std::string _kname;									// entity unique name
		KM_VAR() std::string _kluatable;								// entity env table name in lua
		KM_VAR() U8 _klayerid;
		KM_VAR() U32 _kzorder;
		KM_VAR() KHandle _kfixedComp[(SIZE)CTypes::maxSize];			// fixed components slots (built-in components)
		KM_VAR() std::unordered_map<std::string, KHandle> _klogicComp;	// dynamic components (logic components)
		KM_VAR() std::vector<KHandle> _kchilds;							// children list
		static std::vector<KHandle> _ktrashList;						// components trash list

		// runtime variables (
		//Internal::BaseCHolder<KComponent> **_kcstorage;
		//KCFStorage<KEntity, CTypes::maxSize> *_kestorage;

		// editor only
#ifdef KITE_EDITOR
		ComCallback _kaddCallb;
		ComCallback _kremCallb;
		void *_kaddOpaque;
		void *_kremOpaque;
#endif
	};
}

#endif // KENTITY_H