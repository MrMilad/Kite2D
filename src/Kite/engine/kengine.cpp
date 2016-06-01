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
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
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
#ifdef KITE_EDITOR
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
		registerRTypes(_krman); // CTypes will registered for every scene in KSceneManager

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

		// inite gl window
		_kwindow = new KGLWindow(_kconfig.window);

		// inite input (Mouse, Keyboard)
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();

		// load dictionary
		_kdict.clear();
		if (!_kconfig.dictionary.empty()) {
			KFIStream fstream;
			if (!fstream.open(_kconfig.dictionary, KIOTypes::KRT_BIN)) {
				KD_FPRINT("can't load dictionary. dname: %s", _kconfig.dictionary.c_str());
				return false;
			}

			KBinarySerial bserial;
			bserial.loadStream(&fstream);
			bserial >> _kdict;

			fstream.close();
		}

		// load and set startUp scene
		if (!_kconfig.startUpScene.empty()) {
			if (!_ksman->loadScene(_kconfig.startUpScene)) {
				KD_FPRINT("can't load startrUp scene. sname: %s", _kconfig.startUpScene.c_str());
				return false;
			}

			_ksman->setActiveScene(_ksman->getScene(_kconfig.startUpScene));
		}// else {use default K2D scene}

		// bind all managers into lua
		bindToLua();

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

#ifdef KITE_EDITOR
			if (exitFlag.load()) { break; }
			if (pauseFlag.load()) { continue; }
#endif
			for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
				_keman = _ksman->getActiveScene()->getEManager();
				(*it)->update(0, _ksman->getActiveScene()->getEManager(), _krman);


				// clear trash list
				_ksman->getActiveScene()->getEManager()->postWork();
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

		Internal::destroySDL();
	}

	void KEngine::bindToLua() {
		if (_klstate != nullptr){
			LuaIntf::LuaBinding(_klstate).beginModule("Kite").beginClass<KEngine>("Engine")
				.addStaticFunction("getInstance", &KEngine::createEngine)
				.endClass()
				.endModule();
		}
	}
}