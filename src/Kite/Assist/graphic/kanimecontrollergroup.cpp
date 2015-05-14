/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Kite/Assist/graphic/kanimecontrollergroup.h"

namespace Kite{
	KAnimeControllerGroup::KAnimeControllerGroup():
		_kloop(false),
		_kstype(KAS_STOP)
	{}

	void KAnimeControllerGroup::addController(KAnimeController *Controller){
		if (Controller){
			_klist.push_back(Controller);
			Controller->setState(_kstype);
			Controller->setLoop(_kloop);
		}
	}

	void KAnimeControllerGroup::deleteAllController(){
		_klist.clear();
	}

	void KAnimeControllerGroup::setLoop(bool Loop){
		_kloop = Loop;
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setLoop(Loop);
		}
	}

	void KAnimeControllerGroup::setState(KAnimeStateTypes State){
		_kstype = State;
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setState(State);
		}
	}

	void KAnimeControllerGroup::update(F32 Delta){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->update(Delta);
		}
	}
}