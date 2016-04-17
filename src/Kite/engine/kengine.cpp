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
#include "Kite/engine/kengine.h"
#include "kmeta.khgen.h"
#include <algorithm>
#include <luaintf\LuaIntf.h>

namespace Kite {
	KMETA_KCONFIG_SOURCE();

	KEngine::KEngine() :
		 _klstate(nullptr), _kwindow(nullptr),
		_krman(nullptr), _ksman(nullptr),
		_kmman(nullptr), _kinite (false) {}

	bool KEngine::inite(const KScript &Loader, bool IniteMeta) {
		// inite lua vm
		_klstate = luaL_newstate();
		static const luaL_Reg lualibs[] =
		{
			{ "base", luaopen_base },
			{ NULL, NULL }
		};

		const luaL_Reg *lib = lualibs;
		for (; lib->func != NULL; lib++) {
			lib->func(_klstate);
			lua_settop(_klstate, 0);
		}

		// inite Kite2D ResourceManager, SceneManager, MetaManager
		if (IniteMeta) {
			_kmman = new KMetaManager();
		}

		_krman = new KResourceManager();
		_ksman = new KSceneManager(*_krman);

		registerKiteMeta(_kmman, _klstate);
		registerRTypes(*_krman); // CTypes will registered for every scene in KSceneManager

		// create systems
		createSystems(_ksys);

		// sort systems by priority
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			if ((*it)->getClassName() == "KInputSys") {
				std::iter_swap(it, _ksys.begin());
			}
		}

		// inite systems
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			(*it)->inite((void *)_klstate);
		}

		// run loader script
		luaL_dostring(_klstate, Loader.getCode().c_str());
		LuaIntf::LuaRef loader(_klstate, "_G.IniteEngine");
		if (loader.isFunction()) {
			loader(&_kconfig);
		}

		// inite gl window
		_kwindow = new KGLWindow(_kconfig.window);

		// inite input (Mouse, Keyboard)
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();

		_kinite = true;

		return true;
	}

	void KEngine::start() {
		if (!_kinite) {
			KD_PRINT("engine not initialized.");
			return;
		}

		// open window
		_kwindow->open();

		while (_kwindow->update()) {
			for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
				(*it)->update(0, _ksman->getActiveScene()->getEManager(), *_krman);


				// clear trash list
				_ksman->getActiveScene()->getEManager().postWork();
			}
		}
	}

	void KEngine::shutdown() {
		lua_close(_klstate);

		if (_kwindow->isOpen()) {
			_kwindow->close();
		}
		delete _kwindow;
		_kwindow = nullptr;

		delete _ksman;
		_ksman = nullptr;

		delete _kmman;
		_kmman = nullptr;

		delete _krman;
		_krman = nullptr;

		_ksys.clear();
	}
}