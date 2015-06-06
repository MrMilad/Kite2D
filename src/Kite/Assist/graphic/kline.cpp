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
#include "Kite/Assist/graphic/kline.h"

namespace Kite{
	KLine::KLine() :
		KBatchObject(2),
		_kstart(0.0f, 0.0f),
		_kend(0.0f, 0.0f),
		_kcolor()
	{
		setGeoType(KGP_LINES);
	}

	KLine::KLine(const KVector2F32 &Start, const KVector2F32 &End, const KColor &Color) :
		KBatchObject(2),
		_kstart(Start),
		_kend(End),
		_kcolor(Color)
	{
		setStart(Start);
		setEnd(End);
		setColor(Color);
		setGeoType(KGP_LINES);
	}

	KLine::~KLine(){}

	void KLine::setStart(const KVector2F32 &Start){
		_kvertex[0].pos = Start;
		_kstart = Start;
	}

	void KLine::setEnd(const KVector2F32 &End){
		_kvertex[1].pos = End;
		_kend = End;
	}

	void KLine::setColor(const KColor &Color){
		_kvertex[0].color = _kvertex[1].color = Color;
		_kcolor = Color;
	}

	const KTransform &KLine::getModelViewTransform() const{
		return *getTransform();
	}

	void KLine::animeUpdate(const KAnimeValue *AnimatedValue){
		// center
		setCenter(AnimatedValue->center);

		// scale
		if (AnimatedValue->scaleChannel)
		if (AnimatedValue->schange == KAV_SET) setScale(AnimatedValue->scale); else scale(AnimatedValue->scale);

		// rotate
		if (AnimatedValue->rotateChannel)
		if (AnimatedValue->rchange == KAV_SET) setRotation(AnimatedValue->rotate); else rotate(AnimatedValue->rotate);

		// translate
		if (AnimatedValue->trChannel)
		if (AnimatedValue->tchange == KAV_SET) setPosition(AnimatedValue->translate); else move(AnimatedValue->translate);

		// animate color
		if (AnimatedValue->colorChannel){
			setColor(AnimatedValue->color);
		}
	}
}