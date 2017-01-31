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
#ifndef KCOMPONENT_H
#define KCOMPONENT_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kany.h"
#include "Kite/ecs/kecstypes.h"
#include "Kite/ecs/kecsstructs.h"
#include "Kite/ecs/klistener.h"
#include "Kite/ecs/kmessenger.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <vector>
#include <initializer_list>
#include "kcomponent.khgen.h"
#include "ktypes.khgen.h"

KMETA
namespace Kite {
	class KResourceManager;
	KM_CLASS(COMPONENT, ABSTRACT)
	class KITE_FUNC_EXPORT KComponent : public KListener{
		friend class KEntity;
		friend class KEngine;
	public:
		/// RemoveOnDepZero: automatic remove this component when there is no dependency on it.
		/// DepList: circular-dependency is allowed but not recommended. (bad design)
		///		dont add 'Logic' component as dependency.
		KComponent(const std::string &Name,bool RemoveOnDepZero, std::initializer_list<CTypes> DepList);

		virtual ~KComponent();

		/// will be implemented by KHParser
		virtual bool setProperty(const std::string &Name, KAny &Value) = 0;

		/// will be implemented by KHParser
		virtual KAny getProperty(const std::string &Name) const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "type", KP_TYPE = KCTypes, KP_CM = "type of the component")
		virtual inline CTypes getType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "typeName", KP_TYPE = std::string, KP_CM = "name of the cmponent's type")
		virtual inline std::string getTypeName() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "hashType", KP_TYPE = std::string, KP_CM = "hash code of the component type's")
		virtual inline U32 getHashType() const = 0;

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "name of the component")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "handle", KP_TYPE = KHandle, KP_CM = "component handle")
		inline const KHandle &getHandle() const { return _khandle; }

		KM_PRO_GET(KP_NAME = "ownerHandle", KP_TYPE = KHandle, KP_CM = "owner (entity) handle")
		inline const KHandle &getOwnerHandle() const { return _kohandle; }

		KM_PRO_GET(KP_NAME = "lsitener", KP_TYPE = KListener, KP_CM = "cmponent message listener")
		inline KListener &getListener() { return *(KListener *)this; }

		KM_PRO_GET(KP_NAME = "removeOnZeroDep", KP_TYPE = bool, KP_CM = "cmponent will removed whene there is no dependency")
		inline bool getRemoveOnZeroDep() { return _kremoveNoDep; }

		KM_FUN()
		inline const std::vector<CTypes> &getDependency() const { return _kdeplist; }

		KM_FUN()
		inline U16 getDepCounter() const { return _krefcounter; }

#ifdef KITE_EDITOR
		inline void setSceneItem(void *Ptr) { sceneItem = Ptr; }
		inline void *getSceneItem() const { return sceneItem; }
#endif
		KMETA_KCOMPONENT_BODY();

	protected:
		/// will be called when atached to a entity
		virtual void attached(KEntity *Owner) = 0;

		/// will be called when deattached from an entity
		virtual void deattached(KEntity *Owner) = 0;

		/// will be implemented by KHParser
		/// usage: access base members in lua
		virtual KComponent *getBase() const = 0;

		/// will be implemented by KHParser
		/// this function will be called only once (after deserialization)
		/// and will tracked all loaded resources
		/// so we can unloading them automatically after unloading parrent scene.
		virtual void loadRes() = 0;

		inline void setOwnerHandle(const KHandle &Handle) { _kohandle = Handle; }

		static KResourceManager * getRMan();

	private:

		bool _kremoveNoDep;				// component will removed if refcounter = 0
		std::vector<CTypes> _kdeplist;
		KM_VAR() U16 _krefcounter;		// dependency ref counter
		KM_VAR() std::string _kname;
		KM_VAR() KHandle _khandle;
		KM_VAR() KHandle _kohandle;

		static KResourceManager *_krman;

#ifdef KITE_EDITOR
		void *sceneItem;
#endif
	};
}

#endif // KCOMPONENT_H