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
#include "Kite/engine/kenginedef.h"
#include "Kite/serialization/kserialization.h"
#include "Kite/stream/kfilestream.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/input/kinput.h"
#include "kmeta.khgen.h"
#include "src/Kite/window/sdlcall.h"
#include <luaintf\LuaIntf.h>
#include <algorithm>
#include <chrono>
#include <thread>

namespace Kite {
	KEngine::KEngine(const KConfig &Config) :
		 _klstate(nullptr), _kwindow(nullptr),
		_krman(nullptr), _kinite(false), _kneedSwap(false)
	{
		_kconfig = Config;
		_kinite = true;

		// resource manager
		_krman = new KResourceManager();

		// window
		Internal::initeSDL();
		_kwindow = new KGLWindow(_kconfig.window);
		_kwindow->open(); // we have to open the window for initialize glew

		// input (Mouse, Keyboard)
		// joysticks will initilized automatically
		KKeyboard::initeKeyboard();
		KMouse::initeMouse();

		// calcualte frame time
		if (_kconfig.window.fixFPS) { _kmaxMilli = 1000 / _kconfig.window.maxFPS; }

		// create systems
		CREATE_SYSTEMS(_ksys);

		// sort systems by priority
		for (U16 i = 0; i < (U16)System::maxSize; ++i) {
			_ksysOrder[i] = (System)i;
			// first
			if ((System)i == System::INPUTSYSTEM) {
				auto temp = _ksysOrder[0];
				_ksysOrder[0] = System::INPUTSYSTEM;
				if (i != 0){ _ksysOrder[i] = temp; }

			// second
			} else if ((System)i == System::LOGICSYSTEM) {
				auto temp = _ksysOrder[1];
				_ksysOrder[1] = System::LOGICSYSTEM;
				if (i != 1) { _ksysOrder[i] = temp; }
				
			// last
			}/* else if ((stypes)i == stypes::Render) {
				auto temp = _ksysOrder[(U16)stypes::maxSize];
				_ksysOrder[(U16)stypes::maxSize - 1] = stypes::Render;
				if (i != (U16)stypes::maxSize - 1) { _ksysOrder[i] = temp; }
			}*/
		}

		// load dictionary
		if (!_kconfig.start.dictionary.empty()) {
			KFileStream stream;
			if (!_krman->loadDictionary(stream, _kconfig.start.dictionary)) {
				_kinite = false;
				throw std::string("can't load dictinary. name: " + _kconfig.start.dictionary);
			}
		}

		// load and set startUp scene
		if (!_kconfig.start.startupNode.empty()) {
			setActiveHierarchy(_krman->load(_kconfig.start.startupNode));
		}
	}

	KEngine::~KEngine() {
		// free active hierarchy
		if (_kcurHierarchy) {
			auto node = static_cast<KNode *>(_kcurHierarchy.get());
			node->onActivate(nullptr);
			_kcurHierarchy = KSharedResource();
		}
		

		// destroy systems
		// graphic system should destroy before closing window
		for (auto it = _ksys.begin(); it != _ksys.end(); ++it) {
			if ((*it)) {
				delete (*it);
			}
		}

		// resource manager
		if (_krman) {
			_krman->clear();
			delete _krman;
		}

		// window
		if (_kwindow) {
			if (_kwindow->isOpen()) {
				_kwindow->close();
			}
			delete _kwindow;
		}

		// close lua state
		// lua state should close after destroing systems
		if (_klstate) {
			lua_close(_klstate);
		}

		Internal::destroySDL();
	}

	bool KEngine::setActiveHierarchy(const KSharedResource &Hierarchy) {
		if (!_kinite) {
			KD_PRINT("engine not initialized.");
			return false;
		}

		if (!Hierarchy || Hierarchy == _kcurHierarchy) {
			KD_PRINT("empty or equal hierarchy entered.");
			return false;
		}

		if (Hierarchy->getDrivedType() != Resource::NODE) {
			KD_PRINT("incorrect resource type.");
			return false;
		}

		_kneedSwap = true;
		_knewHierarchy = Hierarchy;
		return true;
	}

	void KEngine::recursiveHierarchyScan(KNode *Hierarchy) {
		// notify parrent
		for (U16 i = 0; i < (U16)System::maxSize; ++i) {
			_ksys[(U16)_ksysOrder[i]]->nodeAdded(Hierarchy);
		}

		// check childs
		for (auto it = Hierarchy->childList()->begin(); it != Hierarchy->childList()->end(); ++it) {
			recursiveHierarchyScan((*it));
		}
	}

	void KEngine::swapHierarchy() {

		// deactive current hierarchy
		if (_kcurHierarchy) {
			auto node = static_cast<KNode *>(&(*_kcurHierarchy));
			node->onActivate(nullptr);
		}

		// we cant close lua current state here before reseting systems
		// so we save current state for close it later and create a new one
		lua_State *temp = nullptr;
		if (_klstate) {
			temp = _klstate;
		}

		// create a new state
		loadLuaState();

		// active new hierarchy
		_kcurHierarchy = _knewHierarchy;
		auto node = static_cast<KNode *>(&(*_kcurHierarchy));
		node->onActivate(&_ksys);

		// release new hierarchy
		_knewHierarchy = KSharedResource();
		_kneedSwap = false;

		// reset systems
		for (U16 i = 0; i < (U16)System::maxSize; ++i) {
			_ksys[(U16)_ksysOrder[i]]->reset(this);
		}

		// scan new hierarchy
		recursiveHierarchyScan(node->getRoot());

		// close old lua state
		if (temp) {
			lua_close(temp);
		}
	}

	void KEngine::loadLuaState() {
		_klstate = luaL_newstate();
		luaopen_base(_klstate);
		luaopen_math(_klstate);
		luaopen_string(_klstate);

		// bind kite
		LUA_BIND(_klstate);

		// bind engine
		LuaIntf::LuaBinding(_klstate).beginModule("k").beginClass<KEngine>("kengine")
			.addPropertyReadOnly("Window", &KEngine::getWindow)
			.addPropertyReadOnly("Manager", &KEngine::getResourceManager)
			.addFunction("switchActiveHierarchy", &KEngine::setActiveHierarchy)
			.addFunction("getSystem", &KEngine::getSystem)
			.endClass().endModule();

		// implement events system and kite table
		const std::string code(KLUA_EVENTS);
		if (luaL_dostring(_klstate, code.c_str()) != 0) {
			_kinite = false;
			const char *out = lua_tostring(_klstate, -1);
			lua_pop(_klstate, 1);
			throw std::string("lua event system failed:" + std::string(out));
		}

		// add engine instance to lua
		LuaIntf::LuaRef::globals(_klstate).get("k").set("Engine", this);
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

#ifdef KITE_EDITOR
			// controllin loop with stdin commands from editor
#endif
			// swap hierarchy if crrent hierarchy changed
			if (_kneedSwap) { swapHierarchy(); }
			if (!_kcurHierarchy) {
				KD_PRINT("there is no active hierarchy");
				return;
			}

			// update systems
			for (U16 i = 0; i < (U16)System::maxSize; ++i) {
				if (!_ksys[(U16)_ksysOrder[i]]->update(delta)) {
					KD_FPRINT("updating systems failed. sname: %s", getSystemName(_ksysOrder[i]).c_str());
					return;
				}
			}

			// display render output
			_kwindow->display();

			// end ticks
			auto end_ticks = std::chrono::high_resolution_clock::now();

			// fix maximum fps
			if (_kconfig.window.fixFPS) {
				auto checkFrame = std::chrono::duration_cast<std::chrono::milliseconds>(end_ticks - begin_ticks).count();

				if (checkFrame < _kmaxMilli) {
					std::this_thread::sleep_for(std::chrono::milliseconds(_kmaxMilli - checkFrame));
					end_ticks = std::chrono::high_resolution_clock::now();
				}
			}

			// convert to seconds
			delta = (F64)std::chrono::duration_cast<std::chrono::milliseconds>(end_ticks - begin_ticks).count() / 1000.0;

			/*static double time = 0;
			static int fps = 0;
			time += delta;
			++fps;
			if (time >= 1) { printf("fps: %i\n", fps); fps = 0; time = 0; }*/

			// Use end_ticks as the new begin_ticks for next frame
			begin_ticks = end_ticks;
		}
	}

}