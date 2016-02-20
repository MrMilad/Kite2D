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
#ifndef KPARTICLEEMITTER_H
#define KPARTICLEEMITTER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathdef.h"
#include "Kite/Core/math/ktransformable.h"
#include "Kite/Core/utility/krandom.h"
#include "Kite/Core/utility/ktimeline.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kbatchobject.h"
#include "Kite/Core/graphic/katlas.h"
#include <forward_list>

namespace Kite{
	// use a separate batch to darw partciles
	class KITE_FUNC_EXPORT KParticleEmitter : public KBatchObject{
	public:
		KParticleEmitter(U32 ParticleSize);

		// rate particle/seconds
		void setEmissionRate(U32 Rate);
		inline U32 getEmissionRate() const { return _krate; }

		// life as seconds
		void setParticleLife(F32 Base, F32 Variation);

		// size
		void setParticleSize(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime);

		// speed
		void setParticleSpeed(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime);

		// angle
		void setParticleAngle(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime);

		// color
		void setParticleColor(const std::vector<KColor> &ColorSheet, bool RandomBase, const KTimeLine<U32, F32> *OverTime);

		// SpriteSheet and OverTime is optional.
		void setParticleUV(const KAtlas &SpriteSheet, bool RandomBase, const KTimeLine<U32, F32> *OverTime);

		void update(F32 Delta);

		// from KTileMapObject (output/send)
		const KVector2F32 *getTilePosition() const;

		const KTransform &getModelViewTransform() const;

	protected:

		// from KAnimeObject (input/recive)
		void animeUpdate(const KAnimeValue *AnimatedValue);

	private:
		KRandom _krandom;
		std::forward_list<KParticle> _kparticles;
		U32 _krate;
		U32 _kavail;
		KVector2F32 _klife;
		KVector2F32 _ksize;
		KVector2F32 _kspeed;
		KVector2F32 _kangle;
		const KTimeLine<F32, F32> *_ksizeovert;
		const KTimeLine<F32, F32> *_kspeedovert;
		const KTimeLine<F32, F32> *_kangleovert;
		const KTimeLine<U32, F32> *_kcolovert;
		const KTimeLine<U32, F32> *_kuvovert;
		const std::vector<KColor> *_kcolsheet;
		const KAtlas *_kuvsheet;
		bool _krndCol;
		bool _krndUV;
	};
}

#endif // KPARTICLEEMITTER_H