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
#ifndef KPARTICLEEMITTER_H
#define KPARTICLEEMITTER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"

namespace Kite{
	class KITE_FUNC_EXPORT KParticleEmitter{
	public:
		KParticleEmitter(U32 ParticleSize) :
			_kpsize(ParticleSize),
			_kparticles(new KParticle[ParticleSize])
		{}

		~KParticleEmitter(){
			delete[] _kparticles;
		}

		inline U32 getParticleSize() const { return _kpsize; }

		inline const KParticle *getParticles() const { return _kparticles; }

		inline void setUV(const KRectF32 &UV) { _kuv = UV; }
		inline const KRectF32 &getUV() const { return _kuv; }

	private:
		KParticle *_kparticles;
		const U32 _kpsize;
		KRectF32 _kuv;
	};
}

#endif // KPARTICLEEMITTER_H