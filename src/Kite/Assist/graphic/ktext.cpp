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
	KText::KText(const std::vector<KAtlas> &Font) :
		KIndexBatchObject(100 * 4, 100 * 6),
		_kfont(&Font),
		_kwidth(0),
		_kheight(0),
		_kmaxSize(100),
		_kmid(0),
		_kpgraph(15)
	{}

	KText::KText(U32 MaximumSize, const std::vector<KAtlas> &Font) :
		KIndexBatchObject(MaximumSize * 4, MaximumSize * 6),
		_kfont(&Font),
		_kwidth(0),
		_kheight(0),
		_kmaxSize(100),
		_kmid(0),
		_kpgraph(15)
	{}

	KText::KText(const std::string &Text, const std::vector<KAtlas> &Font, const KColor &Color) :
		KIndexBatchObject(Text.size() * 4, Text.size() * 6),
		_kfont(&Font),
		_kcolor(Color),
		_kwidth(0),
		_kheight(0),
		_kmaxSize(Text.size()),
		_kmid(0),
		_kpgraph(15)
	{
		setText(Text);
		setGeoType(KGP_TRIANGLES);
	}

	void KText::_fillIndex(){
		U32 ind = 0;
		U32 val = 0;

		// set index buffer size
		this->setUseIndexSize(_ktext.size() * 6);

		for (U32 i = 0; i < _ktext.size(); i++) {
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
		if (_kfont && !_ktext.empty()) {
			const KAtlas *atemp;
			U32 ind = 0;
			F32 width = 0, height = 0;
			char ascii;

			for (U32 i = 0; i < _ktext.size(); i++) {
				// retrieve character from atlas
				ascii = _ktext[i];
				atemp = &KAtlas(0, 0, 0, 0, 0, 0, 0);

				// skip '\0' 
				if (ascii == '\0') { 
					continue; 

				// space
				} else if (ascii == ' ') {
					width += _kfont->at(0).w;
					continue;

				// next line '\n'
				} else if (ascii == '\n') {
					width = 0; height += _kpgraph;
					continue;

				// normal characters
				} else if (_kfont->size() > (ascii - 32) && (ascii - 32) >= 0) {
					// we have key
					atemp = &_kfont->at(ascii - 32);
				} else {
					// we dont have key
					// fill with " " space instead.
					width += _kfont->at(0).w;
					continue;
				}


				KVertex temp;

				// vertex 0
				temp.pos = KVector2F32(width, height);
				temp.uv = KVector2F32(atemp->blu, atemp->trv);
				temp.color = _kcolor;
				_kvertex[ind + 0] = temp;

				// vertex 1
				temp.pos = KVector2F32(width, atemp->h + height);
				temp.uv = KVector2F32(atemp->blu, atemp->blv);
				temp.color = _kcolor;
				_kvertex[ind + 1] = temp;

				// vertex 2
				temp.pos = KVector2F32(width + atemp->w, height);
				temp.uv = KVector2F32(atemp->tru, atemp->trv);
				temp.color = _kcolor;
				_kvertex[ind + 2] = temp;

				// vertex 3
				temp.pos = KVector2F32(width + atemp->w, atemp->h + height);
				temp.uv = KVector2F32(atemp->tru, atemp->blv);
				temp.color = _kcolor;
				_kvertex[ind + 3] = temp;

				width += atemp->w + _kmid;
				ind += 4;
			}

			if (_kwidth < width) { _kwidth = width; }
			_kheight = height;

			// set vertex buffer size
			this->setUseVertexSize(ind);
		}
	}

	void KText::setFont(const std::vector<KAtlas> &Font){
		_kfont = &Font;
		_reshape();
	}

	void KText::setText(const std::string &Text){
		_ktext = Text;
		if (_ktext.size() > _kmaxSize) {
			_ktext.resize(_kmaxSize);
		}
		_reshape();
		_fillIndex();
	}

	void KText::setMiddleSpace(U32 Size){
		_kmid = Size;
		_reshape();
	}

	void KText::setColor(const KColor &Color){
		U32 ind = 0;
		_kcolor = Color;
		_reshape();
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
