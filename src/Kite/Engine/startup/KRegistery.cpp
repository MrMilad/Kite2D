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
#include "Kite/Engine/startup/kregistery.h"
#include "Kite/Core/graphic/kgraphic.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	bool KRegistery::registerKite(bool Script){
		lua_State *lstate = nullptr;
		if (Script)
			lstate = getScriptState();

		//KColor::registerMetaData(nullptr, false, KST_SERIALIZE, nullptr, lstate);

		return true;
	}

	lua_State *KRegistery::getScriptState(){
		static lua_State* L = luaL_newstate();
		return L;
	}
}
