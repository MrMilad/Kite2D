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
#include "Kite/core/klistener.h"
#include "Kite/core/kmessenger.h"
#include "Kite/core/kany.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <string>
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
		virtual void attached() = 0;

		/// called when deattached from an entity
		virtual void deattached() = 0;

		/// will be implemented by KHParser
		virtual bool setProperty(const std::string &Name, KAny &Value) = 0;

		/// will be implemented by KHParser
		virtual KAny getProperty(const std::string &Name) const = 0;

		KM_PRO_GET(NAME = "name", TYPE = std::string, CM = "name of the component")
		inline const std::string &getName() const { return _kname; }

		KM_PRO_GET(NAME = "needUpdate", TYPE = bool, CM = "update state of the component")
		inline bool getNeedUpdate() const { return _kneedup; }

		KM_PRO_GET(NAME = "handle", TYPE = KHandle, CM = "component handle")
		inline const KHandle &getHandle() const { return _khandle; }

		KM_PRO_GET(NAME = "ownerHandle", TYPE = KHandle, CM = "owner (entity) handle")
			inline const KHandle &getOwnerHandle() const { return _kohandle; }

		KM_PRO_GET(NAME = "lsitener", TYPE = KListener, CM = "cmponent message listener")
			inline KListener &getListener() { return *(KListener *)this; }

		/// for simulate polymorphism in script
		KM_FUN()
		inline KComponent *getBase() { return this; }

		KMETA_KCOMPONENT_BODY();

	protected:
		inline void setNeedUpdate(bool NeedUpdate) { _kneedup = NeedUpdate; }
		inline void setOwnerHandle(const KHandle &Handle) { _kohandle = Handle; }

	private:
		KM_VAR() std::string _kname;
		KM_VAR() KHandle _khandle;
		KM_VAR() KHandle _kohandle;
		KM_VAR() bool _kneedup;
	};
}

#endif // KCOMPONENT_H