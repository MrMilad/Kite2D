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
#include "Kite/window/kwindow.h"
#include <vector>

KMETA
namespace Kite {
	struct KStartState {
		std::string startupNode;
		std::string dictionary;
	};

	struct KConfig {
		KWindowState window;
		KStartState start;
	};

	class KITE_FUNC_EXPORT KEngine {
	public:
		// will show window but will not update it
		KEngine(const KConfig &Config);
		KEngine() = delete;
		KEngine(const KEngine &Copy) = delete;
		KEngine &operator=(const KEngine &Right) = delete;
		~KEngine();

		// update window and systems
		void start();

		inline auto getWindow() const { return _kwindow; }

		inline auto getResourceManager() const { return _krman; }

		inline const auto getConfig() const { return &_kconfig; }

		inline auto getSystem(System Type) const { return _ksys[(U16)Type]; }

		inline auto getLuaState() const { return _klstate; }

		inline bool isInite() const { return _kinite; }

		bool setActiveHierarchy(const KSharedResource &Hierarchy);

	private:
		void recursiveHierarchyScan(KNode *Hierarchy);
		void swapHierarchy();
		void loadLuaState();
		lua_State *_klstate;
		KGLWindow *_kwindow;
		KResourceManager *_krman;
		KSharedResource _kcurHierarchy;
		KSharedResource _knewHierarchy;
		std::vector<KSystem *> _ksys;
		System _ksysOrder[(U16)System::maxSize];
		KConfig _kconfig;
		bool _kinite;
		bool _kneedSwap;
		U32 _kmaxMilli;
	};
}

#endif // KENGINE_H