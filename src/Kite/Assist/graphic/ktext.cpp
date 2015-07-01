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
#include "Kite/Assist/graphic/ktext.h"

namespace Kite{
	KText::KText(U32 MaxSize) :
		KIndexBatchObject(MaxSize * 4, MaxSize * 6),
		_kfont(0),
		_ktext(""),
		_kwidth(0),
		_ksize(MaxSize),
		_kcolor()
	{
		_fillIndex();
		setGeoType(KGP_TRIANGLES);
	}

	KText::KText(const std::string &Text, const std::vector<KAtlas> &Font, const KColor &Color) :
		KIndexBatchObject(Text.size() * 4, Text.size() * 6),
		_kfont(&Font),
		_ktext(Text),
		_kwidth(0),
		_ksize(Text.size()),
		_kcolor(Color)
	{
		_fillIndex();
		setFont(Font);
		setColor(Color);
		setGeoType(KGP_TRIANGLES);
	}

	void KText::_fillIndex(){
		U32 ind = 0;
		U32 val = 0;
		for (U32 i = 0; i < _ksize; i++){
			ind = i * 6;
			val = i * 4;
			_kindex[ind] = val;
			_kindex[ind + 1] = val + 1;
			_kindex[ind + 2] = val + 2;
			_kindex[ind + 3] = val + 2;
			_kindex[ind + 4] = val + 1;
			_kindex[ind + 5] = val + 3;
		}
	}

	void KText::_reshape(){
		if (_kfont && !_ktext.empty()){
			const KAtlas *atemp;
			U32 ind = 0;
			F32 width = 0;
			char ascii;

			for (U32 i = 0; i < _ksize; i++){
				// retrieve character from atlas
				ascii = _ktext[i];
				atemp = &KAtlas(0, 0, 0, 0, 0, 0, 0);
				if (ascii != '\0')
					atemp = &_kfont->at(ascii - 32);

				ind = i * 4;
				// size and position
				_kvertex[ind].pos = KVector2F32(width, 0.0f);
				_kvertex[ind + 1].pos = KVector2F32(width, atemp->h);
				_kvertex[ind + 2].pos = KVector2F32(width + atemp->w, 0.0f);
				_kvertex[ind + 3].pos = KVector2F32(width + atemp->w, atemp->h);

				// uv
				_kvertex[ind].uv = KVector2F32(atemp->blu, atemp->trv);
				_kvertex[ind + 1].uv = KVector2F32(atemp->blu, atemp->blv);
				_kvertex[ind + 2].uv = KVector2F32(atemp->tru, atemp->trv);
				_kvertex[ind + 3].uv = KVector2F32(atemp->tru, atemp->blv);

				width += atemp->w + _kmid;
			}
		}
	}

	void KText::setFont(const std::vector<KAtlas> &Font){
		_kfont = &Font;
		_reshape();
	}

	void KText::setText(const std::string &Text){
		_ktext = Text;
		_ktext.resize(_ksize);
		_reshape();
	}

	void KText::setMiddleSpace(U32 Size){
		_kmid = Size;
		setText(_ktext);
	}

	void KText::setColor(const KColor &Color){
		U32 ind = 0;
		_kcolor = Color;
		for (U32 i = 0; i < _ksize; i++){
			ind = i * 4;
			_kvertex[ind].color = _kvertex[ind + 1].color = _kvertex[ind + 2].color = _kvertex[ind + 3].color = Color;
		}
	}

	const KTransform &KText::getModelViewTransform() const{
		return *getTransform();
	}

	void KText::animeUpdate(const KAnimeValue *AnimatedValue){
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
	}
}
