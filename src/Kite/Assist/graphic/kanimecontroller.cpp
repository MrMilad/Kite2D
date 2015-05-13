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
#include <cmath>

namespace Kite{
	KAnimeController::KAnimeController():
		_kclip(0),
		_katlas(0),
		_ktime(0),
		_kloop(false),
		_kstype(KAS_STOP),
		_kvalue(),
		_kcurrentKey(0)
	{}

	KAnimeController::KAnimeController(KAnimeObject *Object,
		const std::vector<KAnimeKey> *AnimationClip, const std::vector<KAtlas> *SpriteSheet) :
		_katlas(SpriteSheet),
		_ktime(0),
		_kloop(false),
		_kstype(KAS_STOP),
		_kvalue(),
		_kcurrentKey(0)
	{
		addObject(Object);
		setClip(AnimationClip);
	}

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

	void KAnimeController::deleteAllObjects(){
		_klist.clear();
	}

	void KAnimeController::setClip(const std::vector<KAnimeKey> *AnimationClip){
		_kclip = AnimationClip;

		if (AnimationClip)
		if (!AnimationClip->empty()){
			_kcurrentKey = 0;
			_ktime = 0;
		}
	}

	void KAnimeController::setCurrentTime(F32 Time){
		if (!_kclip)
			return;

		if (Time > _kclip->back().time || Time < 0)
			return;

		_ktime = Time;
		keyLookup();
	}

	F32 KAnimeController::getTotalTime() const{
		if (_kclip)
		if (!_kclip->empty())
			return _kclip->back().time;

		return 0;
	}

	void KAnimeController::setCurrentKey(U32 KeyIndex){
		if (!_kclip)
			return;

		if (KeyIndex >= _kclip->size())
			return;

		_kcurrentKey = KeyIndex;
		_ktime = _kclip->at(KeyIndex).time;
	}

	void KAnimeController::addTimeTrigger(KCallAnimeTrigger Functor, void *Sender, F32 StartTime, F32 Duration){
		if (!Functor || StartTime < 0)
			return;

		_kttrig.push_back(Internal::KAnimeTimeTrigger(StartTime, StartTime + Duration, Functor, false, Sender));
	}

	void KAnimeController::deleteAllTriggers(){
		_kttrig.clear();
	}

	void KAnimeController::update(F32 Delta){
		if (!_kclip || _klist.empty())
			return;

		if (_kclip->empty())
			return;

		// check animation state
		if (_kstype == KAS_STOP)  {	_ktime = 0; _kcurrentKey = 0; return; }
		if (_kstype == KAS_PAUSE) { return; }

		bool needStop = false;
		bool needInterpolate = true;

		// map KTween functions into fucntion pointers
		static F32(*tweenFunction[])(F32, F32, F32, F32) = { KTween::linear, KTween::quadraticIn,
			KTween::quadraticOut, KTween::quadraticInOut, KTween::sinusoidalIn,
			KTween::sinusoidalOut, KTween::sinusoidalInOut, KTween::exponentialIn,
			KTween::exponentialOut, KTween::exponentialInOut };

		// time calibration
		if (_kloop){
			// out of range time (before 0 or after total duration)
			if ((_ktime + Delta) >= _kclip->back().time) { _ktime = fmod(_ktime + Delta, _kclip->back().time); }
			else if ((_ktime + Delta) <= 0) { _ktime = _kclip->back().time - fmod(abs(_ktime + Delta) + _kclip->back().time, _kclip->back().time); }
			// in range time
			else { _ktime += Delta; }
		}else{
			// out of range time (before 0 or after total duration)
			if ((_ktime + Delta) >= _kclip->back().time) { _ktime = _kclip->back().time; needStop = true; needInterpolate = false; }
			else if ((_ktime + Delta) <= 0) { _ktime = 0; needStop = true; needInterpolate = false; }
			// in range time
			else { _ktime += Delta; }
		}

		// find appropriate key
		keyLookup();

		const KAnimeKey *key1 = &_kclip->at(_kcurrentKey);
		if (needInterpolate){
			const KAnimeKey *key2 = 0;
			KInterpolationTypes type;
			key2 = &_kclip->at(_kcurrentKey + 1);

			// transform
			type = key1->tinterp;
			_kvalue.translate.x = tweenFunction[type](_ktime, key1->translate.x, key2->translate.x, key2->time - key1->time);
			_kvalue.translate.y = tweenFunction[type](_ktime, key1->translate.y, key2->translate.y, key2->time - key1->time);
			type = key1->sinterp;
			_kvalue.scale.x = tweenFunction[type](_ktime, key1->scale.x, key2->scale.x, key2->time - key1->time);
			_kvalue.scale.y = tweenFunction[type](_ktime, key1->scale.y, key2->scale.y, key2->time - key1->time);
			type = key1->rinterp;
			_kvalue.rotate = tweenFunction[type](_ktime, key1->rotate, key2->rotate, key2->time - key1->time);

			// color
			type = key1->cinterp;
			_kvalue.color.a = tweenFunction[type](_ktime, key1->color.a, key2->color.a, key2->time - key1->time);
			_kvalue.color.r = tweenFunction[type](_ktime, key1->color.r, key2->color.r, key2->time - key1->time);
			_kvalue.color.g = tweenFunction[type](_ktime, key1->color.g, key2->color.g, key2->time - key1->time);
			_kvalue.color.b = tweenFunction[type](_ktime, key1->color.b, key2->color.b, key2->time - key1->time);
		}else{
			key1 = &_kclip->at(_kcurrentKey);
			_kvalue.translate = key1->translate;
			_kvalue.scale = key1->scale;
			_kvalue.rotate = key1->rotate;
			_kvalue.color = key1->color;
		}

		// uv
		if (_katlas)
		if (_katlas->size() > key1->uv){
			const KAtlas *atlas = &_katlas->at(_kcurrentKey);
			_kvalue.uv = KRectF32(atlas->blu, atlas->tru, atlas->trv, atlas->blv);
		}

		// other values
		_kvalue.center = key1->center;
		_kvalue.tchange = key1->tchange;
		_kvalue.schange = key1->schange;
		_kvalue.rchange = key1->rchange;
		_kvalue.trChannel = key1->tchannel;
		_kvalue.scaleChannel = key1->schannel;
		_kvalue.rotateChannel = key1->rchannel;
		_kvalue.colorChannel = key1->cchannel;
		_kvalue.uvChannel = key1->uvchannel;

		for (U32 i = 0; i < _klist.size(); i++){
			_klist[i]->animeUpdate(&_kvalue);
		}

		// check for triggers
		triggerLookup();

		if (needStop)
			this->setState(KAS_STOP);
	}

	void KAnimeController::triggerLookup(){
		if (_kttrig.empty())
			return;

		for (U32 i = 0; i < _kttrig.size(); i++){
			if (_kttrig[i].start <= _ktime && _kttrig[i].end >= _ktime){
				if (!_kttrig[i].called){
					_kttrig[i].func(_kttrig[i].sender);
					_kttrig[i].called = true;
				}
			}
			else{
				_kttrig[i].called = false;
			}
		}
	}

	void KAnimeController::keyLookup(){
		if (!_kclip)
			return;

		if (_kclip->empty()){
			_kcurrentKey = 0;
			_ktime = 0;
			return;
		}

		// first step, we check last key in our clip
		if (_ktime == _kclip->back().time){
			_kcurrentKey = _kclip->size() - 1;
			return;
		}

		// then we check all keys from 0 to last - 1
		for (U32 i = 0; i < _kclip->size() - 1; i++){
			if (_kclip->at(i).time <= _ktime && _kclip->at(i + 1).time > _ktime){
				_kcurrentKey = i;
				return;
			}
		}
	}

}