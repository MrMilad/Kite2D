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
#include "Kite/window/kwindow.h"
#include "Kite/input/kinput.h"
#include "Kite/logic/klogic.h"
#include "Kite/math/kmath.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/serialization/kserialization.h"
#include <vector>
#include <memory>
#include "kengine.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(POD)
	struct KConfig {
		KMETA_KCONFIG_BODY();

		KM_VAR() KWindowState window;
	};

	class KITE_FUNC_EXPORT KEngine : KNonCopyable {
	public:
		KEngine();

		bool inite(const KScript &Loader, bool IniteMeta);
		void start();
		void shutdown();

		inline auto getWindow() { return _kwindow; }
		inline auto getSceneManager() { return _ksman; }
		inline auto getMetaManager() { return _kmman; }
		inline auto getEntityManager() { return _keman; }
		inline auto getResourceManager() { return _krman; }
		inline auto getLuaState() { return _klstate; }

	private:
		lua_State *_klstate;
		KGLWindow *_kwindow;
		KMetaManager *_kmman;
		KSceneManager *_ksman;
		KEntityManager *_keman;
		KResourceManager *_krman;
		std::vector<std::unique_ptr<KSystem>> _ksys;
		bool _kinite;
		KConfig _kconfig;
	};
}

#endif // KENGINE_H