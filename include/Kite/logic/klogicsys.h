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
#ifndef KLOGICSYS_H
#define KLOGICSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/ksystem.h"
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/meta/kmetadef.h"
#include "klogicsys.khgen.h"
#include <luaintf/LuaIntf.h>

KMETA
namespace Kite {
#define	KLOGIC_NODE_ARRAY 128 // predefined size of node table in lua
#define	KLOGIC_COM_ARRAY 256 // predefined size of node table in lua
	class KLogicCom;
	KM_CLASS(SYSTEM)
	class KITE_FUNC_EXPORT KLogicSys : public KSystem {
		KM_INFO(KI_NAME = "LogicSystem");
		KLOGICSYS_BODY();
	public:
		KLogicSys();

		void reset(KEngine *Engine) override;

		bool update(F64 Delta) override;

		void nodeAdded(KNode *Node) override;

		void nodeRemoved(KNode *Node) override;

		void componentAdded(KComponent *Com) override;

		void componentRemoved(KComponent *Com) override;

		KM_PRO_SET(KP_NAME = "updatePerFrame")
		inline void setUpdatePerFrame(bool Enable) { _kupPerFrame = Enable; }

		KM_PRO_GET(KP_NAME = "updatePerFrame", KP_TYPE = bool, KP_CM = "enable updating per frame")
		inline bool getUpdatePerFrame() const { return _kupPerFrame; }

		KM_PRO_SET(KP_NAME = "collectGarbagePerFrame")
		inline void setCollectGarbagePerFrame(bool Enable) { _kcgarbage = Enable; }

		KM_PRO_GET(KP_NAME = "collectGarbagePerFrame", KP_TYPE = bool, KP_CM = "enable collecting garbage per frame")
		inline bool getCollectGarbagePerFrame() const { return _kcgarbage; }

	private:
		void reloadChunk(KLogicCom *Com);
		bool _kcgarbage;
		bool _kupPerFrame;
		lua_State *_klvm;
		LuaIntf::LuaRef _kevents;
		LuaIntf::LuaRef _ktrigger;
		LuaIntf::LuaRef _kremove;
		LuaIntf::LuaRef _kcoms;
		LuaIntf::LuaRef _kglobalVars;
	};
}
#endif // KLOGICSYS_H