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
#include "Kite/Core/graphic/kquad.h"
#include <vector>

namespace Kite{
	KMETA_DEF(KQuad, KMFLAG_CLASS) {
		KMETA_ADD_MEMBER(_kcolor);
		KMETA_ADD_MEMBER(_kuv);
	}

	KQuad::KQuad() :
		KBatchObject(4, 6, "quad")
	{
		setTileFlag(KTO_QUAD);
		_fillIndex();
		setDimension(KRect2F32());
		setUV(KRectF32(0.0f, 1.0f, 0.0f, 1.0f));
		setColor(KColor());
		setGeoType(KGP_TRIANGLES);
	}

	KQuad::KQuad(const KRectF32 &Dimension, const KRectF32 &UV, const KColor &Color):
		KBatchObject(4, 6, "quad")
	{
		setTileFlag(KTO_QUAD);
		_fillIndex();
		setDimension(KRect2F32(KVector2F32(Dimension.left, Dimension.bottom),
			KVector2F32(Dimension.left, Dimension.top),
			KVector2F32(Dimension.right, Dimension.bottom),
			KVector2F32(Dimension.right, Dimension.top)));
		setUV(UV);
		setColor(Color);
		setGeoType(KGP_TRIANGLES);
	}

	KQuad::KQuad(const KRect2F32 &Dimension, const KRectF32 &UV, const KColor &Color):
		KBatchObject(4, 6, "quad")
	{
		setTileFlag(KTO_QUAD);
		_fillIndex();
		setDimension(Dimension);
		setUV(UV);
		setColor(Color);
		setGeoType(KGP_TRIANGLES);
	}

	void KQuad::_fillIndex(){
		_kindex[0] = 0;
		_kindex[1] = 1;
		_kindex[2] = 2;
		_kindex[3] = 2;
		_kindex[4] = 1;
		_kindex[5] = 3;
	}

	void KQuad::setDimension(const KRect2F32 &Dimension){
		_kvertex[0].pos = Dimension.leftBottom;
		_kvertex[1].pos = Dimension.leftTop;
		_kvertex[2].pos = Dimension.rightBottom;
		_kvertex[3].pos = Dimension.rightTop;
		_kdim = Dimension;
	}

	void KQuad::setDimension(const KRectF32 &Dimension){
		setDimension(KRect2F32(KVector2F32(Dimension.left, Dimension.bottom),
			KVector2F32(Dimension.left, Dimension.top),
			KVector2F32(Dimension.right, Dimension.bottom),
			KVector2F32(Dimension.right, Dimension.top)));
	}

	void KQuad::setUV(const KRectF32 &UV){
		_kvertex[0].uv = KVector2F32(UV.left, UV.bottom);
		_kvertex[1].uv = KVector2F32(UV.left, UV.top);
		_kvertex[2].uv = KVector2F32(UV.right, UV.bottom);
		_kvertex[3].uv = KVector2F32(UV.right, UV.top);
		_kuv = UV;
	}

	void KQuad::setColor(const KColor &Color){
		_kvertex[0].color = _kvertex[1].color = _kvertex[2].color = _kvertex[3].color = Color;
		_kcolor = Color;
	}

	const KVector2F32 *KQuad::getTilePosition() const {
		return getPosition();
	}

	const KTransform & KQuad::getModelViewTransform() const{
		return *this->getTransform();
	}

	void KQuad::animeUpdate(const KAnimeValue *AnimatedValue){
		// center
		setCenter(AnimatedValue->center);

		// skew
		if (AnimatedValue->skewChannel)
		if (AnimatedValue->skchange == KAV_SET) setSkew(AnimatedValue->skew); else skew(AnimatedValue->skew);

		// scale
		if (AnimatedValue->scaleChannel)
		if (AnimatedValue->schange == KAV_SET) setScale(AnimatedValue->scale); else scale(AnimatedValue->scale);

		// rotate
		if (AnimatedValue->rotateChannel)
		if (AnimatedValue->rchange == KAV_SET) setRotation(AnimatedValue->rotate); else rotate(AnimatedValue->rotate);

		// translate
		if (AnimatedValue->trChannel){
			if (AnimatedValue->tchange == KAV_SET) setPosition(AnimatedValue->translate); else move(AnimatedValue->translate);

			// position (set position only when we use translate)
			move(AnimatedValue->position);
		}

		// animate color
		if (AnimatedValue->colorChannel){
			setColor(AnimatedValue->color);
		}

		// animate UV
		if (AnimatedValue->uvChannel){
			setUV(AnimatedValue->uv);
		}
	}

}