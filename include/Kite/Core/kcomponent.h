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
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/core/klistener.h"
#include "Kite/core/kmessenger.h"
#include "Kite/core/kany.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
#include <vector>
#include "kcomponent.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(COMPONENT, ABSTRACT)
	class KITE_FUNC_EXPORT KComponent : public KListener{
		friend class KEntityManager;
		friend class KEntity;
	public:
		KComponent(const std::string &Name = "");

		virtual ~KComponent();

		/// called when atached to a entity
		virtual void attached(KEntity *Owner) = 0;

		/// called when deattached from an entity
		virtual void deattached(KEntity *Owner) = 0;

		/// will be implemented by KHParser
		virtual bool setProperty(const std::string &Name, KAny &Value) = 0;

		/// will be implemented by KHParser
		virtual KAny getProperty(const std::string &Name) const = 0;

		/// will be implemented by KHParser
		/// usage: access base members in lua
		virtual KComponent *getBase() = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "type", KP_TYPE = std::string, KP_CM = "type of the component")
		virtual inline std::string getType() const = 0;

		/// will be implemented by KHParser
		KM_PRO_GET(KP_NAME = "hashType", KP_TYPE = std::string, KP_CM = "hash code of the component type's")
		virtual inline U32 getHashType() const = 0;

		KM_PRO_GET(KP_NAME = "name", KP_TYPE = std::string, KP_CM = "name of the component")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(KP_NAME = "needUpdate", KP_TYPE = bool, KP_CM = "update state of the component")
		inline bool getNeedUpdate() const { return _kneedup; }

		KM_PRO_GET(KP_NAME = "handle", KP_TYPE = KHandle, KP_CM = "component handle")
		inline const KHandle &getHandle() const { return _khandle; }

		KM_PRO_GET(KP_NAME = "ownerHandle", KP_TYPE = KHandle, KP_CM = "owner (entity) handle")
		inline const KHandle &getOwnerHandle() const { return _kohandle; }

		KM_PRO_GET(KP_NAME = "lsitener", KP_TYPE = KListener, KP_CM = "cmponent message listener")
		inline KListener &getListener() { return *(KListener *)this; }

		KM_FUN()
		inline const std::vector<std::string> &getDependency() const { return _kdeplist; }

		KMETA_KCOMPONENT_BODY();

	protected:
		/// add component dependency.
		/// use this function in constructure.
		/// circular-dependency is allowed but not recommended. (bad design)
		/// dont add Logic component as dependency
		inline void addDependency(const std::string &ComponentName) { _kdeplist.push_back(ComponentName); }
		inline void setNeedUpdate(bool NeedUpdate) { _kneedup = NeedUpdate; }
		inline void setOwnerHandle(const KHandle &Handle) { _kohandle = Handle; }

	private:
		bool _kneedup;
		std::vector<std::string> _kdeplist;
		KM_VAR() std::string _kname;
		KM_VAR() KHandle _khandle;
		KM_VAR() KHandle _kohandle;
	};
}

#endif // KCOMPONENT_H