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
#include "src/Kite/window/sdlcall.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KMETA_KCONFIG_SOURCE();

	bool KEngine::deleted = true;
	KEngine *KEngine::instance = nullptr;

	KEngine *KEngine::createEngine() {
		if (deleted) {
			instance = new KEngine;
			deleted = false;
		}
		return instance;
	}

	KEngine::KEngine() :
		 _klstate(nullptr), _kwindow(nullptr),
		_kmman(nullptr), _ksman(nullptr),
		_krman(nullptr), _keman(nullptr),
		_kinite (false) 
	{
#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG)
		exitFlag = false;
		pauseFlag = false;
#endif
	}

	KEngine::~KEngine() {
		deleted = true;
	}

	bool KEngine::inite(const KConfig *Config, bool IniteMeta) {
		_kconfig = *Config;
		// inite lua vm
		_klstate = luaL_newstate();
		static const luaL_Reg lualibs[] =
		{
			{ "base", luaopen_base },
			{ "string", luaopen_string },
			{ NULL, NULL }
		};

		const luaL_Reg *lib = lualibs;
		for (; lib->func != NULL; lib++) {
			lib->func(_klstate);
			lua_settop(_klstate, 0);
		}

		// redirect lua print output into editor
#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG)
		LuaIntf::LuaRef lprint(_klstate, "_G");
		lprint["print"] = luaCustomPrint;
#endif

		// inite Kite2D ResourceManager, SceneManager, MetaManager
		if (IniteMeta) {
			_kmman = new KMetaManager();
		}

		_krman = new KResourceManager();
		_ksman = new KSceneManager(*_krman);

		registerKiteMeta(_kmman, _klstate); // _kmman can be passed as nullptr
		registerRTypes(_krman); // CTypes will registered in EntityManager constructure

		// create systems
		createSystems(_ksys);

		// sort systems by priority
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			if ((*it)->getClassName() == "KInputSys") {
				std::iter_swap(it, _ksys.begin());
			} else if ((*it)->getClassName() == "KRenderSys") {
				std::iter_swap(it, --_ksys.end());
			}
		}

		// inite gl window
		_kwindow = new KGLWindow(_kconfig.window);
		_kwindow->open(); // we have to open the window for initialize glew

		// inite systems (glew must initialize in render system)
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			(*it)->inite((void *)_klstate);
		}

		// inite input (Mouse, Keyboard)
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();

		// load dictionary
		if (!_kconfig.dictionary.empty()) {
			KFIStream stream;
			if (!_krman->loadDictionary(stream, _kconfig.dictionary)) {
				return false;
			}
		}

		// load and set startUp scene
		if (!_kconfig.startUpScene.empty()) {
			if (!_ksman->loadScene(_kconfig.startUpScene)) {
				KD_FPRINT("can't load startrUp scene. sname: %s", _kconfig.startUpScene.c_str());
				return false;
			}

			_ksman->setActiveScene(_ksman->getScene(_kconfig.startUpScene));
		}// else {use default K2D scene}

		_kinite = true;
		return true;
	}

	void KEngine::start() {
		if (!_kinite) {
			KD_PRINT("engine not initialized.");
			return;
		}

		while (_kwindow->update()) {

#if defined(KITE_EDITOR) && defined (KITE_DEV_DEBUG)
			if (exitFlag.load()) { break; }
			if (pauseFlag.load()) { continue; }
#endif
			for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
				_keman = _ksman->getActiveScene()->getEManager();
				
				if (!((*it)->update(0, _ksman->getActiveScene()->getEManager(), _krman))) {
					KD_FPRINT("updating systems failed. sname: %s", (*it)->getClassName().c_str());
					return;
				}
			}

			// clear trash list
			_ksman->getActiveScene()->getEManager()->postWork();

			// display render output
			_kwindow->display();
		}
	}

	void KEngine::shutdown() {
		lua_close(_klstate);

		// destroy systems
		// graphic sustem must destroy before closing window
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			it->get()->destroy();
		}
		_ksys.clear();

		delete _ksman;
		_ksman = nullptr;

		delete _kmman;
		_kmman = nullptr;

		delete _krman;
		_krman = nullptr;

		if (_kwindow->isOpen()) {
			_kwindow->close();
		}

		delete _kwindow;
		_kwindow = nullptr;

		Internal::destroySDL();
	}

#if defined(KITE_EDITOR) && defined(KITE_DEV_DEBUG)
	int KEngine::luaCustomPrint(lua_State* L) {
		int nargs = lua_gettop(L);

		std::string str;

		for (int i = 1; i <= nargs; i++) {
			const char *luaStr = lua_tostring(L, i);
			if (luaStr != nullptr) {
				str.append(luaStr);
				str.append(" ");
			} else {
				str.append("<nil> ");
			}
		}

		(pcallback)(str, msgType::MSG_LUA);

		return 0;
	}
#endif

	KMETA_KENGINE_SOURCE();
}