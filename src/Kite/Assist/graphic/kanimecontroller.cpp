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
#include "Kite/Assist/graphic/kanimecontroller.h"
#include "Kite/Core/math/ktween.h"

namespace Kite{
	KAnimeController::KAnimeController():
		_kclip(0),
		_katlas(0),
		_ktime(0),
		_klcounter(0),
		_kloop(false),
		_kstype(KAS_STOP),
		_kptype(KAP_FOREWARD),
		_kvalue()
	{}

	KAnimeController::KAnimeController(KAnimeObject *Object,
		const std::vector<KAnimeKey> *AnimationClip, const KAtlasObjects *SpriteSheet) :
		_kclip(AnimationClip),
		_katlas(SpriteSheet),
		_ktime(0),
		_klcounter(0),
		_kloop(false),
		_kstype(KAS_STOP),
		_kptype(KAP_FOREWARD),
		_kvalue()
	{}

	I32 KAnimeController::addObject(KAnimeObject *Object){
		if (Object){
			_klist.push_back(Object);
			return _klist.size() - 1;
		}

		return -1;
	}

	const KAnimeObject *KAnimeController::getObject(U32 ObjectID) const{
		if (ObjectID >= _klist.size() || _klist.empty())
			return 0;

		return _klist[ObjectID];
	}

	void KAnimeController::deleteObject(U32 ObjectID){
		if (ObjectID >= _klist.size() || _klist.empty())
			return;

		_klist.erase(_klist.begin() + ObjectID);
	}

	void KAnimeController::clear(){
		_klist.clear();
	}

	void KAnimeController::setClip(const std::vector<KAnimeKey> *AnimationClip){
		_kclip = AnimationClip;

		if (AnimationClip)
		if (!AnimationClip->empty()){
			// check clip channels
			_kvalue.transformChannel = AnimationClip->at(0).trchannel;
			_kvalue.colorChannel = AnimationClip->at(0).colchannel;
			_kvalue.uvChannel = AnimationClip->at(0).uvchannel;
		}
	}

	void KAnimeController::update(F32 Delta){
		if (!_kclip || _klist.empty())
			return;

		if (_kclip->empty())
			return;

		// map KTween functions into fucntion pointers
		static F32(*tweenFunction[])(F32, F32, F32, F32) = { KTween::linear, KTween::quadraticIn,
			KTween::quadraticOut, KTween::quadraticInOut, KTween::sinusoidalIn,
			KTween::sinusoidalOut, KTween::sinusoidalInOut, KTween::exponentialIn,
			KTween::exponentialOut, KTween::exponentialInOut };

		// check clip channels
		KInterpolationTypes type;
		const KAnimeKey *key1 = &_kclip->at(0);
		const KAnimeKey *key2 = &_kclip->at(1);
		type = key1->rinterp;

		_kvalue.rotate = tweenFunction[type](Delta, key1->rotate, key2->rotate, key2->time - key1->time);
		_kvalue.rchange = key1->rchange;

		_kvalue.scale.x = tweenFunction[type](Delta, key1->scale.x, key2->scale.x, key2->time - key1->time);
		_kvalue.scale.y = tweenFunction[type](Delta, key1->scale.y, key2->scale.y, key2->time - key1->time);
		_kvalue.schange = key1->schange;

		_kvalue.position.x = tweenFunction[type](Delta, key1->pos.x, key2->pos.x, key2->time - key1->time);
		_kvalue.position.y = tweenFunction[type](Delta, key1->pos.y, key2->pos.y, key2->time - key1->time);
		_kvalue.pchange = key1->pchange;

		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->animeUpdate(&_kvalue);
		}
	}

}