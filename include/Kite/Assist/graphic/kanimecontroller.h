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
#ifndef KANIMECONTROLLER_H
#define KANIMECONTROLLER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Assist/graphic/kanimeobject.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KAnimeController{
	public:
		KAnimeController();
		KAnimeController(KAnimeObject *Object, const std::vector<KAnimeKey> *AnimationClip, const std::vector<KAtlas> *SpriteSheet);

		// add object to list
		// all object in the list Will be animated in parallel
		// return -1 on error
		void addObject(KAnimeObject *Object);

		// clear object list
		void deleteAllObjects();

		// set animation clip
		void setClip(const std::vector<KAnimeKey> *AnimationClip);

		// get animation clip
		inline const std::vector<KAnimeKey> *getClip() const { return _kclip; }

		// set sprite sheet
		// pass 0 if there is no sprite sheet
		inline void setSpriteSheet(const std::vector<KAtlas> *SpriteSheet) { _katlas = SpriteSheet; }

		// get sprite sheet
		inline const std::vector<KAtlas> *getSpriteSheet() const { return _katlas; }

		// set animation loop
		// loop apply at any direction (foreward and backwrad)
		inline void setLoop(bool Loop) { _kloop = Loop; }

		// get animation loop
		inline bool getLoop() const { return _kloop; }

		inline void setPlayDirection(KAnimeDirectionTypes Direction) { _kdtype = Direction; }

		inline KAnimeDirectionTypes getPlayDirection() const { return _kdtype; }

		// set animation state (play, pause, stop)
		inline void setState(KAnimeStateTypes State) { _kstype = State; }

		// get animation state (play, pause, stop)
		inline KAnimeStateTypes getState() const { return _kstype; }

		// in seconds
		// Time must be in range (0 to last key time)
		void setCurrentTime(F32 Time);

		// in seconds
		inline F32 getCurrentTime() const { return _ktime; }

		F32 getTotalTime() const;

		// set current key
		// KeyIndex must be in range (0 to clip size - 1) 
		void setCurrentKey(U32 KeyIndex);

		// get current key
		inline U32 getCurrentKey() const { return _kcurrentKey; }

		// add a time trigger (if StartTime <= current time <= StartTime + Duration) then trigger
		// the functor will be executed only once in their duration
		// time count as second
		// avoid heavy work in the functor.
		void addTimeTrigger(KCallAnimeTrigger Functor, void *Sender, F32 StartTime, F32 Duration);

		// set a key-change callback.
		// the functor will be executed only once per key.
		// key-change happens before any trigger.
		// normally, the animation played from first key (clip[0]), so key-change for the first key in the first turn will not happen.
		// avoid heavy work in the functor.
		void setKeyChangeCallback(KCallKeyChange Functor, void *Sender);

		// delete all time triggers
		void deleteAllTimeTriggers();

		// delete all time triggers
		void deleteAllKeyTriggers();

		// update animation state
		// positive and negative values, are ineffective in play direction (use setPlayDirection())
		void update(F32 Delta);

	private:
		void triggerLookup(); // lookup triggers
		void keyLookup(); // lookup keys and find appropriate key (depend of time)
		const std::vector<KAnimeKey> *_kclip;
		const std::vector<KAtlas> *_katlas;
		std::vector<Internal::KAnimeTimeTrigger> _kttrig;
		F32 _ktime;
		bool _kloop;
		KAnimeStateTypes _kstype;
		std::vector<KAnimeObject *> _klist;
		KAnimeValue _kvalue;
		U32 _kcurrentKey;
		KAnimeDirectionTypes _kdtype;
		KCallKeyChange _kkcall;
		void *_kksender;
	};
}

#endif // KANIMECONTROLLER_H
