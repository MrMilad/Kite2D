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
#include "Kite/Core/math/kmath.h"

namespace Kite{
	class KITE_FUNC_EXPORT KAnimeController{
	public:
		KAnimeController();
		KAnimeController(const KAnimeObjects *Animation, const KAtlasObjects *SpriteSheet);

		inline void setAnimation(const KAnimeObjects *Animation) { _kanime = Animation; _kreset = true; }
		inline const KAnimeObjects *getAnimation() const { return _kanime; }

		inline void setSpriteSheet(const KAtlasObjects *SpriteSheet) { _katlas = SpriteSheet; }
		inline const KAtlasObjects *getSpriteSheet() const { return _katlas; }

		inline void setLoop(bool Loop) { _kloop = Loop; }
		inline bool getLoop() const { return _kloop; }

		inline void Reset() { _kreset = true; _ktime = 0; }

		inline void setState(KAnimationStateTypes State) { _kstype = State; }
		inline KAnimationStateTypes getState() const { return _kstype; }

		inline void setPlayState(KAnimationPlayTypes PlayState) { _kptype = PlayState; }
		inline KAnimationPlayTypes getPlayState() const { return _kptype; }

		// in seconds
		inline F32 getCurrentTime() const { return _ktime; }

		// in seconds
		void setFrame(F32 Time);

		void update(F32 Delta);

		// get current value of all animated properties
		inline const KAnimeValue *getAnimeValue() const { return &_kvalue; }

	private:
		const KAnimeObjects *_kanime;
		const KAtlasObjects *_katlas;
		bool _kloop;
		bool _kreset;
		KAnimationStateTypes _kstype;
		KAnimationPlayTypes _kptype;
		U32 _ktime;
		KAnimeValue _kvalue;
	};
}

#endif // KANIMECONTROLLER_H
