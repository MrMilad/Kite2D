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
#ifndef KANIME_H
#define KANIME_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Assist/graphic/kindexBatchObject.h"
#include <cstring>
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KAnime{
	public:
		KAnime();
		KAnime(const KAnimeObject *Animation);

		inline void setAnimation(const KAnimeObject *Animation) { _kanime = Animation; _kreset = true; }
		inline const KAnimeObject *getAnimation() const { return _kanime; }

		inline void setAnimationLoop(bool Loop) { _kloop = Loop; }
		inline bool getAnimationLoop() const { return _kloop; }

		inline void AnimationReset() { _kreset = true; _kframe = 0; }

		inline void setAnimationState(KAnimationStateTypes State) { _kstate = State; }
		inline KAnimationStateTypes getAnimationState() const { return _kstate; }

		inline void setAnimationPlayState(KAnimationPlayTypes PlayState) { _kpstate = PlayState; }
		inline KAnimationPlayTypes getAnimationPlayState() const { return _kpstate; }

		void setAnimationFrame(U16 Frame);
		inline U16 getAnimationFrame() const { return _kframe; }

		void updateAnimation();

	protected:
		virtual void _getJointsState(const KAnimeState *States, U32 Count, U16 Frame) = 0;

	private:
		const KAnimeObject *_kanime;
		bool _kloop;
		bool _kreset;
		KAnimationStateTypes _kstate;
		KAnimationPlayTypes _kpstate;
		U16 _kframe;
	};
}

#endif // KANIME_H
