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
#ifndef KENGINE_H
#define KENGINE_H

#include "Kite/core/kcore.h"
#include "Kite/ecs/kecs.h"
#include "Kite/graphic/kgraphic.h"
#include "Kite/window/kwindow.h"
#include "Kite/input/kinput.h"
#include "Kite/logic/klogic.h"
#include "Kite/math/kmath.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "kengine.khgen.h"

#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG) // editor hooks	
	#include <condition_variable>
	#include <mutex>
	#include <atomic>
	#include <thread>
#endif

KMETA
namespace Kite {
	KM_CLASS(POD)
	struct KConfig {
		KMETA_KCONFIG_BODY();

		KM_VAR() KWindowState window;
		KM_VAR() KRenderState render;
		KM_VAR() std::string startUpScene;
		KM_VAR() std::string dictionary;
	};

	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KEngine : KNonCopyable {
		KMETA_KENGINE_BODY();
	public:
		~KEngine();

		KM_FUN()
		static KEngine *createEngine();

		// will show window but will not update it
		bool inite(const KConfig *Config, bool IniteMeta);

		// update window and systems
		void start();

		void shutdown();

		KM_FUN()
		inline auto getWindow() { return _kwindow; }

		KM_FUN()
		inline auto getSceneManager() { return _ksman; }

		KM_FUN()
		inline auto getMetaManager() { return _kmman; }

		KM_FUN()
		inline auto getResourceManager() { return _krman; }

		KM_FUN()
		inline const auto getConfig() const { return &_kconfig; }

		inline auto getLuaState() { return _klstate; }

#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG) // editor hooks	
		inline void setExitFlag(bool Value) { exitFlag.store(Value); }
		inline void setPauseFlag(bool Value) { pauseFlag.store(Value); }
#endif

	private:
		KEngine();
		KEngine(KEngine const&);
		void operator=(KEngine const&);

		lua_State *_klstate;
		KGLWindow *_kwindow;
		KMetaManager *_kmman;
		KResourceManager *_krman;
		std::vector<std::unique_ptr<KSystem>> _ksys;
		bool _kinite;
		KConfig _kconfig;
		static bool deleted;
		static KEngine *instance;
#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG) // editor hooks
		std::atomic<bool> exitFlag;
		std::atomic<bool> pauseFlag;
		static int luaCustomPrint(lua_State* L);
	public:
		std::mutex mx;
		std::condition_variable cv;
#endif
	};
}

#endif // KENGINE_H