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
#ifndef KINPUTSYS_H
#define KINPUTSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/ksystem.h"
#include "Kite/meta/kmetadef.h"
#include "kinputsys.khgen.h"
#include <luaintf/LuaIntf.h>

KMETA
namespace Kite {
	KM_CLASS(SYSTEM)
	class KITE_FUNC_EXPORT KInputSys : public KSystem {
		KM_INFO(KI_NAME = "InputSystem");
		KINPUTSYS_BODY();
	public:
		void reset(KEngine *Engine) override;

		bool update(F64 Delta) override;

		void nodeAdded(KNode *Node) override;

		void nodeRemoved(KNode *Node) override;

		void componentAdded(KComponent *Com) override;

		void componentRemoved(KComponent *Com) override;

	private:
		lua_State *_klvm;
		LuaIntf::LuaRef _keventTrig;
	};
}


#endif // KINPUTSYS_H