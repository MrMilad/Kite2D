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
#include <chrono>

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
		if (!Config) return false;
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
		KComponent::_krman = _krman;
		registerKiteMeta(_kmman, _klstate); // _kmman can be passed as nullptr

		// create systems
		createSystems(_ksys);

		// sort systems by priority
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			// first
			if ((*it)->getClassName() == "KInputSys") {
				std::iter_swap(it, _ksys.begin());

			// second
			}else if ((*it)->getClassName() == "KLogicSys") {
					std::iter_swap(it, _ksys.begin() + 1);

			// last
			} else if ((*it)->getClassName() == "KRenderSys") {
				std::iter_swap(it, --_ksys.end());
			}
		}

		// inite gl window
		Internal::initeSDL();
		_kwindow = new KGLWindow(_kconfig.window);
		_kwindow->open(); // we have to open the window for initialize glew

		// inite systems (glew must initialize in render system)
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			(*it)->inite((void *)this);
		}

		// inite input (Mouse, Keyboard)
		// joystick will initilized automatically
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

		_keman = _ksman->getActiveScene()->getEManager();
		_kinite = true;
		return true;
	}

	void KEngine::start() {
		if (!_kinite) {
			KD_PRINT("engine not initialized.");
			return;
		}

		// start off assuming an ideal frame time (60 FPS). 
		F64 delta = 1.0 / 60.0;

		// Prime the pump by reading the current time.
		auto begin_ticks = std::chrono::high_resolution_clock::now();

		while (_kwindow->update()) {
#if defined(KITE_EDITOR) && defined(KITE_DEV_DEBUG)
			std::unique_lock<std::mutex> lk(mx);
			if (pauseFlag.load()) { cv.wait(lk); }
			if (exitFlag.load()) { break; }
			std::this_thread::sleep_for(std::chrono::milliseconds(5)); // just for sync with editor
#endif
			// inja baiad beine har tavize scene 1 bar nullptr be tamame systema beferestim
			for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
				if (!((*it)->update(delta, _ksman->getActiveScene()->getEManager(), _krman))) {
					KD_FPRINT("updating systems failed. sname: %s", (*it)->getClassName().c_str());
					return;
				}
			}

			// display render output
			_kwindow->display();

			// clear trash list and post works will executed in logic system

			// end ticks
			auto end_ticks = std::chrono::high_resolution_clock::now();

			// convert to seconds
			delta = (F64)std::chrono::duration_cast<std::chrono::milliseconds>(end_ticks - begin_ticks).count() / 1000.0;

			// Use end_ticks as the new begin_ticks for next frame
			begin_ticks = end_ticks;
		}
	}

	void KEngine::shutdown() {
		lua_close(_klstate);
		_klstate = nullptr;

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

		_krman->clear();
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