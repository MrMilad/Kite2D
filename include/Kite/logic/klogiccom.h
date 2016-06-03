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
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/logic/kscript.h"
#include <string>
#include "klogiccom.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KLogicCom : public KComponent {
		friend class KLogicSys;
		KM_INFO(KI_CTYPE = "Logic");
		KMETA_KLOGICCOM_BODY();

	public:
		KLogicCom(const std::string &Name = "");

		void attached() override;

		/// remove this script from entity
		void deattached() override;

		KM_FUN()
		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		KM_PRO_SET(KP_NAME = "script")
		void setScript(const std::string &ResName);
		
		KM_PRO_GET(KP_NAME = "script", KP_TYPE = std::string, KP_CM = "Lua script", KP_RES = "KScript")
		inline const std::string &getScript() const { return _kresName; }

		KM_PRO_GET(KP_NAME = "ownerTable", KP_TYPE = std::string, KP_CM = "Owner table in lua")
		inline const std::string &getTName() const { return _ktname; }

	private:
		void removeLuaEnv();
		void setLuaState(lua_State *L);

		KM_VAR() std::string _kresName;
		KM_VAR() std::string _ktname;

		bool _kinite;;
		bool _kstart;

		KScript *_kscript;
		lua_State *_klstate;
	};
}


#endif // KLOGICCOM_H