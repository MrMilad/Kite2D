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
#include "Kite/Assist/graphic/kparticleemitter.h"
#include "Kite/Core/system/ksystemutil.h"
#include "Kite/Core/math/kmathdef.h"

namespace Kite {
	KParticleEmitter::KParticleEmitter(U32 ParticleSize) :
		KArrayBatchObject(ParticleSize, true),
		_krandom(getUniqueNumber()),
		_kparticles(),
		_krate(0),
		_kavail(ParticleSize),
		_ksizeovert(0),
		_kspeedovert(0),
		_kangleovert(0),
		_kcolovert(0),
		_kuvovert(0),
		_kcolsheet(0),
		_kuvsheet(0),
		_krndCol(false),
		_krndUV(false)
	{
		setTileFlag(KTO_EMITTER);
		setGeoType(KGP_POINTS);
		setRelativeTransform(false);
	}

	void KParticleEmitter::setEmissionRate(U32 Rate) {
		_krate = Rate;
	}

	void KParticleEmitter::setParticleLife(F32 Base, F32 Variation) {
		if (Base < 0.000f) {
			Base = 0.000f;
		}

		_klife.x = Base;
		_klife.y = Variation;
	}

	void KParticleEmitter::setParticleSize(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime) {
		_ksize.x = Base;
		_ksize.y = Variation;
		_ksizeovert = OverTime;
	}

	void KParticleEmitter::setParticleSpeed(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime) {
		_kspeed.x = Base;
		_kspeed.y = Variation;
		_kspeedovert = OverTime;
	}

	void KParticleEmitter::setParticleAngle(F32 Base, F32 Variation, const KTimeLine<F32, F32> *OverTime) {
		_kangle.x = Base;
		_kangle.y = Variation;
		_kangleovert = OverTime;
	}

	void KParticleEmitter::setParticleColor(const std::vector<KColor> &ColorSheet, bool RandomBase, const KTimeLine<U32, F32> *OverTime) {
		_kcolsheet = &ColorSheet;
		_kcolovert = OverTime;
		_krndCol = RandomBase;
	}

	void KParticleEmitter::setParticleUV(const std::vector<KAtlas> &SpriteSheet, bool RandomBase, const KTimeLine<U32, F32> *OverTime) {
		_kuvsheet = &SpriteSheet;
		_kuvovert = OverTime;
		_krndUV = RandomBase;
	}

	void KParticleEmitter::update(F32 Delta) {
		setUseSize(0);
		static F32 count;
		// add new particle if need
		if (_kavail > 0) {
			count += Delta * _krate;
			for (U32 j = 0; (_kavail > 0 && count >= 1.00f); j++) {
				KParticle temp;

				// timer (spawn timer)
				temp.timer = 0.0f;

				// life
				// convert to miliseconds
				_krandom.setRange(0, (U32)_klife.y * 1000);
				temp.life = _klife.x + ((float)_krandom.getRandomInt() / 1000.0f);

				// size
				_krandom.setRange(0, (U32)_ksize.y);
				temp.size = _ksize.x + (F32)_krandom.getRandomInt();

				// speed
				_krandom.setRange(0, (U32)_kspeed.y);
				temp.speed = _kspeed.x + _krandom.getRandomInt();

				// angle
				_krandom.setRange(0, (U32)_kangle.y);
				temp.angle = _kangle.x + _krandom.getRandomInt();

				// spawn position
				F32 inRadians = temp.angle * KMATH_PIsub180;
				temp.pos.x += (temp.speed * cos(inRadians)) * Delta;
				temp.pos.y += (temp.speed * sin(inRadians)) * Delta;
				if (!getRelativeTransform()) {
					temp.pos = getTransform()->transformPoint(temp.pos);
				}

				// color
				if (_kcolsheet) {
					if (!_kcolsheet->empty()) {

						// random base color
						if (_krndCol) {
							_krandom.setRange(0, _kcolsheet->size() - 1);
							temp.color = (*_kcolsheet)[_krandom.getRandomInt()];

						// use first color in color sheet
						} else {
							temp.color = (*_kcolsheet)[0];
						}
					}
				}

				// uv
				if (_kuvsheet) {
					if (!_kuvsheet->empty()) {

						// random base uv
						if (_krndUV) {
							_krandom.setRange(0, _kuvsheet->size() - 1);
							U32 rnd = _krandom.getRandomInt();
							temp.uv.left = (*_kuvsheet)[rnd].blu;
							temp.uv.bottom = (*_kuvsheet)[rnd].blv;
							temp.uv.right = (*_kuvsheet)[rnd].tru - (*_kuvsheet)[rnd].blu;;
							temp.uv.top = (*_kuvsheet)[rnd].trv - (*_kuvsheet)[rnd].blv;

							// use first color in color sheet
						} else {
							temp.uv.left = (*_kuvsheet)[0].blu;
							temp.uv.bottom = (*_kuvsheet)[0].blv;
							temp.uv.right = (*_kuvsheet)[0].tru - (*_kuvsheet)[0].blu;;
							temp.uv.top = (*_kuvsheet)[0].trv - (*_kuvsheet)[0].blv;
						}
					}
				}

				_kparticles.push_front(temp);
				--_kavail;
				--count;
			}
		}

		// update particles
		if (!_kparticles.empty()) {
			auto it = _kparticles.begin();
			auto prevIt = _kparticles.before_begin();
			U32 i = 0;

			// its a tricky loop (it and prevIt)
			for (it; it != _kparticles.end();) {

				// life
				if ((it->life - Delta) <= 0.000f) {
					_kparticles.erase_after(prevIt);
					it = prevIt;
					++it;
					++_kavail;
					continue;
				}

				it->life -= Delta;
				it->timer += Delta;

				// size
				_kpsprite[i].pointSize = it->size;
				if (_ksizeovert) {
					_kpsprite[i].pointSize += _ksizeovert->getValueByX(it->timer, true);
				}

				// speed
				F32 speed = it->speed;
				if (_kspeedovert) {
					speed += _kspeedovert->getValueByX(it->timer, true);
				}

				// pos
				F32 inRadians = it->angle;
				if (_kangleovert) {
					inRadians += _kangleovert->getValueByX(it->timer, true);
				}

				inRadians *= KMATH_PIsub180;
				it->pos.x += (speed * cos(inRadians)) * Delta;
				it->pos.y += (speed * sin(inRadians)) * Delta;
				_kvertex[i].pos = it->pos;

				// color
				_kvertex[i].color = it->color;
				if (_kcolovert) {
					std::pair<U32, U32> index;
					std::pair<F32, F32> time;
					_kcolovert->getSectionByX(it->timer, index, time);

					// color tween
					_kvertex[i].color.a = (F32)KTween<F64, F32>::linear(it->timer - time.first,
																   (*_kcolsheet)[index.first].a, (*_kcolsheet)[index.second].a,
																   time.second - time.first);

					_kvertex[i].color.r = (F32)KTween<F64, F32>::linear(it->timer - time.first,
																   (*_kcolsheet)[index.first].r, (*_kcolsheet)[index.second].r,
																   time.second - time.first);

					_kvertex[i].color.g = (F32)KTween<F64, F32>::linear(it->timer - time.first,
																   (*_kcolsheet)[index.first].g, (*_kcolsheet)[index.second].g,
																   time.second - time.first);

					_kvertex[i].color.b = (F32)KTween<F64, F32>::linear(it->timer - time.first,
																   (*_kcolsheet)[index.first].b, (*_kcolsheet)[index.second].b,
																   time.second - time.first);
				}

				// uv
				_kvertex[i].uv.x = it->uv.left;
				_kvertex[i].uv.y = it->uv.bottom;
				_kpsprite[i].textureSize.x = it->uv.right;
				_kpsprite[i].textureSize.y = it->uv.top;
				if (_kuvovert) {
					U32 index = _kuvovert->getValueByX(it->timer, false);
					_kvertex[i].uv.x = (*_kuvsheet)[index].blu;
					_kvertex[i].uv.y = (*_kuvsheet)[index].blv;
					_kpsprite[i].textureSize.x = (*_kuvsheet)[index].tru - (*_kuvsheet)[index].blu;
					_kpsprite[i].textureSize.y = (*_kuvsheet)[index].trv - (*_kuvsheet)[index].blv;
				}

				++i;
				++it;
				++prevIt;
			}

			// set buffer size
			this->setUseSize(i);
		}
		
	}

	const KVector2F32 *KParticleEmitter::getTilePosition() const {
		return this->getPosition();
	}

	const KTransform &KParticleEmitter::getModelViewTransform() const {
		return *this->getTransform();
	}

	void KParticleEmitter::animeUpdate(const KAnimeValue *AnimatedValue) {
		// center
		setCenter(AnimatedValue->center);

		// translate
		if (AnimatedValue->trChannel) {
			if (AnimatedValue->tchange == KAV_SET) setPosition(AnimatedValue->translate); else move(AnimatedValue->translate);

			// position (set position only when we use translate)
			move(AnimatedValue->position);
		}
	}

}