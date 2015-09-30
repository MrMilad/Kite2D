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
#include "Kite/Core/graphic/kanimecontroller.h"
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
		_kcurrentKey(0),
		_kdtype(KAD_FOREWARD),
		_kkcall(0)
	{}

	KAnimeController::KAnimeController(KAnimeObject *Object,
									   const std::vector<KAnimeKey> *AnimationClip, const KAtlas *SpriteSheet = 0) :
		_katlas(SpriteSheet),
		_ktime(0),
		_kloop(false),
		_kstype(KAS_STOP),
		_kvalue(),
		_kcurrentKey(0),
		_kdtype(KAD_FOREWARD),
		_kkcall(0)
	{
		addObject(Object);
		setClip(AnimationClip);
	}

	void KAnimeController::addObject(KAnimeObject *Object){
		if (Object){
			_klist.push_back(Object);
		}
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
		_ktime = (*_kclip)[KeyIndex].time;
	}

	void KAnimeController::addTimeTrigger(KCallAnimeTrigger Functor, void *Sender, F32 StartTime, F32 Duration){
		if (!Functor || StartTime < 0)
			return;

		_kttrig.push_back(Internal::KAnimeTimeTrigger(StartTime, StartTime + Duration, Functor, false, Sender));
	}

	void KAnimeController::setKeyChangeCallback(KCallKeyChange Functor, void *Sender){
		_kkcall = Functor;
		_kksender = Sender;
	}

	void KAnimeController::deleteAllTimeTriggers(){
		_kttrig.clear();
	}

	void KAnimeController::update(F32 Delta){
		if (!_kclip || _klist.empty())
			return;

		if (_kclip->empty())
			return;

		// check animation state
		if (_kstype == KAS_STOP)  {	_ktime = 0; _kcurrentKey = 0; return; }
		if (_kstype == KAS_PAUSE || Delta == 0) { return; }

		bool needStop = false;
		bool needInterpolate = true;

		// map KTween functions into fucntion pointers
		static KCallEasingFloat tweenFunction[] = { KTween<F64, F32>::linear, KTween<F64, F32>::quadraticIn,
			KTween<F64, F32>::quadraticOut, KTween<F64, F32>::quadraticInOut, KTween<F64, F32>::sinusoidalIn,
			KTween<F64, F32>::sinusoidalOut, KTween<F64, F32>::sinusoidalInOut, KTween<F64, F32>::exponentialIn,
			KTween<F64, F32>::exponentialOut, KTween<F64, F32>::exponentialInOut };

		// direction
		if (_kdtype == KAD_FOREWARD){
			Delta = fabs(Delta);
		}else{
			Delta = -fabs(Delta);
		}

		// time calibration
		if (_kloop){
			//// this method causes both sides of animation stick together (like a circle) 
			//// and loss of a few frames(with associated event) but its not a problem, its a method!
			// out of range time (before 0 or after total duration) 
			//if ((_ktime + Delta) > _kclip->back().time) { _ktime = fmodf(_ktime + Delta, _kclip->back().time); }
			//else if ((_ktime + Delta) <= 0) { _ktime = _kclip->back().time - fmodf(abs(_ktime + Delta) + _kclip->back().time, _kclip->back().time); }
			// in range time
			//else { _ktime += Delta; }
			
			if (_kdtype == KAD_FOREWARD && _ktime == _kclip->back().time){ _ktime = 0; needInterpolate = true; }
			if (_kdtype == KAD_BACKWARD && _ktime == 0) _ktime = _kclip->back().time;

			// out of range time (before 0 or after total duration)
			if ((_ktime + Delta) > _kclip->back().time) { _ktime = _kclip->back().time; needInterpolate = false; }
			else if ((_ktime + Delta) < 0) { _ktime = 0; }
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

		const KAnimeKey *key1 = &(*_kclip)[_kcurrentKey];
		U8 type;
		if (needInterpolate){
			const KAnimeKey *key2 = 0;
			key2 = &(*_kclip)[_kcurrentKey + 1];

			// transform
			type = key1->tinterp;
			_kvalue.translate.x = (F32)tweenFunction[type](_ktime - key1->time, key1->translate.x, key2->translate.x, key2->time - key1->time);
			_kvalue.translate.y = (F32)tweenFunction[type](_ktime - key1->time, key1->translate.y, key2->translate.y, key2->time - key1->time);
			type = key1->sinterp;
			_kvalue.scale.x = (F32)tweenFunction[type](_ktime - key1->time, key1->scale.x, key2->scale.x, key2->time - key1->time);
			_kvalue.scale.y = (F32)tweenFunction[type](_ktime - key1->time, key1->scale.y, key2->scale.y, key2->time - key1->time);
			type = key1->skinterp;
			_kvalue.skew.x = (F32)tweenFunction[type](_ktime - key1->time, key1->skew.x, key2->skew.x, key2->time - key1->time);
			_kvalue.skew.y = (F32)tweenFunction[type](_ktime - key1->time, key1->skew.y, key2->skew.y, key2->time - key1->time);
			type = key1->rinterp;
			_kvalue.rotate = (F32)tweenFunction[type](_ktime - key1->time, key1->rotate, key2->rotate, key2->time - key1->time);

			// color
			// bekhater taghis animation be property animation in ghesmato ke baiad color haro be getGL...() taghir name bedim ro dorost nemikonam
			type = key1->cinterp;
			/*_kvalue.color.a = (F32)tweenFunction[type](_ktime - key1->time, key1->color.a, key2->color.a, key2->time - key1->time);
			_kvalue.color.r = (F32)tweenFunction[type](_ktime - key1->time, key1->color.r, key2->color.r, key2->time - key1->time);
			_kvalue.color.g = (F32)tweenFunction[type](_ktime - key1->time, key1->color.g, key2->color.g, key2->time - key1->time);
			_kvalue.color.b = (F32)tweenFunction[type](_ktime - key1->time, key1->color.b, key2->color.b, key2->time - key1->time);*/

			_kvalue.trChannel = key2->trChannel;
			_kvalue.scaleChannel = key2->scaleChannel;
			_kvalue.skewChannel = key2->skewChannel;
			_kvalue.rotateChannel = key2->rotateChannel;
			_kvalue.colorChannel = key2->colorChannel;
			_kvalue.uvChannel = key2->uvChannel;

		}else{
			key1 = &(*_kclip)[_kcurrentKey];
			_kvalue.translate = key1->translate;
			_kvalue.scale = key1->scale;
			_kvalue.skew = key1->skew;
			_kvalue.rotate = key1->rotate;
			_kvalue.color = key1->color;
		}

		// uv
		if (_katlas)
		if (_katlas->getItems()->size() > key1->uv){
			const KAtlasItem *atlas = &(*_katlas->getItems())[key1->uv];
			_kvalue.uv = KRectF32(atlas->blu, atlas->tru, atlas->trv, atlas->blv);
		}

		// other values
		_kvalue.center = key1->center;
		_kvalue.position = key1->position;
		_kvalue.tchange = key1->tchange;
		_kvalue.schange = key1->schange;
		_kvalue.skchange = key1->skchange;
		_kvalue.rchange = key1->rchange;

		// send animated values to objects
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
			}else{
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

		U32 temp = _kcurrentKey;

		// first step, we check last key in our clip
		if (_ktime == _kclip->back().time){

			// set current key
			_kcurrentKey = _kclip->size() - 1;

			// key-change callback
			if (_kkcall && temp != _kclip->size() - 1){
				_kkcall(_kksender, _kclip->size() - 1);
			}
			return;
		}

		// then we check all keys from 0 to last - 1
		for (U32 i = 0; i < _kclip->size() - 1; i++){

			if ((*_kclip)[i].time <= _ktime && (*_kclip)[i + 1].time > _ktime){

				// set current key
				_kcurrentKey = i;

				// key-change callback
				if (_kkcall && temp != i){
					if (_kdtype == KAD_FOREWARD){
						_kkcall(_kksender, i);
					}
					else{
						_kkcall(_kksender, i + 1);
					}
				}
		
				return;
			}
		}
	}

}