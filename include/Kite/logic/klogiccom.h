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
#ifndef KLOGICCOM_H
#define KLOGICCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kcomponent.h"
#include "Kite/ecs/knode.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/ecs/kresourcemanager.h"
#include <string>
#include "klogiccom.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KLogicCom : public KComponent {
		friend class KLogicSys;
		KM_INFO(KI_NAME = "Logic");
		KLOGICCOM_BODY();

	public:
		KM_PRO_SET(KP_NAME = "script")
		void setScript(const KSharedResource &Script);
		
		KM_PRO_GET(KP_NAME = "script", KP_CM = "lua script", KP_RES = Resource::SCRIPT)
		inline const KSharedResource &getScript() const { return _kscript; }

		KM_PRO_GET(KP_NAME = "tableIndex", KP_TYPE = U32, KP_CM = "index of lua table", KP_SHOW = false)
		inline U32 getTableIndex() const { return _klid; }

	private:
		void inite() override;

		void attached() override;

		/// remove this script from entity
		void deattached() override;

		KM_FUN()
		int getTable(lua_State *L);

		KM_VAR() KSharedResource _kscript;

		U32 _klid;
		KLogicSys *_ksys;
		typedef void(KLogicSys::*reloadScript)(KLogicCom *Self);
		reloadScript _kreloadScriptCallb;
	};
}


#endif // KLOGICCOM_H