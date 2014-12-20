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
#include "Kite/Assist/graphic/kanime.h"

namespace Kite{
	KAnime::KAnime() :
		_kanime(0),
		_kloop(true),
		_kreset(false),
		_kstate(KAS_STOP),
		_kpstate(KAP_FOREWARD),
		_kframe(0)
	{}

	KAnime::KAnime(const KAnimeObject *Animation) :
		_kanime(Animation),
		_kloop(true),
		_kreset(false),
		_kstate(KAS_STOP),
		_kpstate(KAP_FOREWARD),
		_kframe(0)
	{}

	void KAnime::setAnimationFrame(U16 Frame){
		if (_kanime)
		if (Frame < _kanime->frames)
			_kframe = Frame;
	}

	void KAnime::updateAnimation(){
		if (_kanime){
			// stopped
			if (_kstate == KAS_STOP){
				// foreward
				if (_kpstate == KAP_FOREWARD){
					_kframe = 0;
				}
				// backward
				else if (_kpstate == KAP_BACKWARD){
					_kframe = _kanime->frames - 1;
				}
			}

			// process current frame
			_getJointsState(&_kanime->states[_kframe * _kanime->joints], _kanime->joints, _kframe);

			// calculate next frame
			// playing
			if (_kstate == KAS_PLAY){
				// foreward
				if (_kpstate == KAP_FOREWARD){
					++_kframe;
					if (_kframe >= _kanime->frames)
					if (_kloop){
						_kframe = 0;
					}
					else{
						_kframe = _kanime->frames - 1;
					}
				}// backward
				else if (_kpstate == KAP_BACKWARD){
					if (_kframe == 0){
						if (_kloop){
							_kframe = _kanime->frames - 1;;
						}
					}
					else{
						--_kframe;
					}
				}

			}
			// paused
			// do nothing
		}
	}
}