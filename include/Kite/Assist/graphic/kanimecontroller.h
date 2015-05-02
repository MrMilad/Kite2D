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
#ifndef KANIMECONTROLLER_H
#define KANIMECONTROLLER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Assist/graphic/kanimeobject.h"
#include <list>

namespace Kite{
	class KITE_FUNC_EXPORT KAnimeController{
	public:
		KAnimeController();
		KAnimeController(KAnimeObject *Object, const KAnimeClip *AnimationClip, const KAtlasObjects *SpriteSheet);

		// add object to list
		// all object in the list Will be animated in parallel
		U32 addObject(KAnimeObject *Object);

		// get object
		inline const KAnimeObject *getObject(U32 ObjectID) const;

		// delete object from list
		void deleteObject(U32 ObjectID);

		// clear list
		void clear();

		// set animation clip
		void setClip(const KAnimeClip *AnimationClip);

		// get animation clip
		inline const KAnimeClip *getClip() const { return _kclip; }

		// set sprite sheet
		// pass 0 if there is no sprite sheet
		inline void setSpriteSheet(const KAtlasObjects *SpriteSheet) { _katlas = SpriteSheet; }

		// get sprite sheet
		inline const KAtlasObjects *getSpriteSheet() const { return _katlas; }

		// set animation loop
		inline void setLoop(bool Loop) { _kloop = Loop; }

		// get animation loop
		inline bool getLoop() const { return _kloop; }

		// set loop counter
		inline void setLoopCounter(U16 Counter) { _klcounter = Counter; }

		// get loop counter
		inline U16 getLoopCounter() const { return _klcounter; }

		// set animation state (play, pause, stop)
		inline void setState(KAnimationStateTypes State) { _kstype = State; }

		// get animation state (play, pause, stop)
		inline KAnimationStateTypes getState() const { return _kstype; }

		// set animation play state (FOREWARD, BACKWARD)
		inline void setPlayState(KAnimationPlayTypes PlayState) { _kptype = PlayState; }

		// get animation play state (FOREWARD, BACKWARD)
		inline KAnimationPlayTypes getPlayState() const { return _kptype; }

		// in seconds
		inline F32 setCurrentTime(F32 Time) { _ktime = Time; }

		// in seconds
		inline F32 getCurrentTime() const { return _ktime; }

		// update animation state
		void update(F32 Delta);

	private:
		const KAnimeClip *_kclip;
		const KAtlasObjects *_katlas;
		F32 _ktime;
		U16 _klcounter;
		bool _kloop;
		KAnimeStateTypes _kstype;
		KAnimePlayTypes _kptype;
		std::list<KAnimeObject *> _klist;
	};
}

#endif // KANIMECONTROLLER_H
