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
#include "Kite/Assist/graphic/kanimecontrollergroup.h"

namespace Kite{
	void KAnimeControllerGroup::addObject(KAnimeController *Controller){
		if (Controller){
			_klist.push_back(Controller);
		}
	}

	void KAnimeControllerGroup::removeAllObjects(){
		_klist.clear();
	}

	void KAnimeControllerGroup::setLoop(bool Loop){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setLoop(Loop);
		}
	}

	void KAnimeControllerGroup::setPlayDirection(KAnimeDirectionTypes Direction){
		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->setPlayDirection(Direction);
		}
	}

	void KAnimeControllerGroup::setState(KAnimeStateTypes State){
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