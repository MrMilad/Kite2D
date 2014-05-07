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
#include "Kite/Assist/graphic/kquad.h"

namespace Kite{
	KQuad::KQuad() :
		KIndexBatchObject(4, 6),
		_kdim(0.0f, 1.0f, 0.0f, 1.0f),
		_kuv(0.0f, 1.0f, 0.0f, 1.0f),
		_kcolor()
	{
		_kindex[0] = 0;
		_kindex[1] = 1;
		_kindex[2] = 2;
		_kindex[3] = 2;
		_kindex[4] = 1;
		_kindex[5] = 3;

		setDimension(_kdim);
		setUV(_kuv);
		setColor(_kcolor);
	}

	KQuad::KQuad(const KRectF32 &Dimension, const KRectF32 &UV, const KColor &Color):
		KIndexBatchObject(4, 6),
		_kdim(Dimension),
		_kuv(UV),
		_kcolor(Color)
	{
		_kindex[0] = 0;
		_kindex[1] = 1;
		_kindex[2] = 2;
		_kindex[3] = 2;
		_kindex[4] = 1;
		_kindex[5] = 3;

		setDimension(_kdim);
		setUV(_kuv);
		setColor(_kcolor);
	}

	void KQuad::setDimension(const KRectF32 &Dimension){
		_kvertex[0].pos = KVector2F32(Dimension.left, Dimension.top);
		_kvertex[1].pos = KVector2F32(Dimension.left, Dimension.bottom);
		_kvertex[2].pos = KVector2F32(Dimension.right, Dimension.top);
		_kvertex[3].pos = KVector2F32(Dimension.right, Dimension.bottom);
		_kdim = Dimension;
	}

	void KQuad::setUV(const KRectF32 &UV){
		_kvertex[0].uv = KVector2F32(UV.left, UV.top);
		_kvertex[1].uv = KVector2F32(UV.left, UV.bottom);
		_kvertex[2].uv = KVector2F32(UV.right, UV.top);
		_kvertex[3].uv = KVector2F32(UV.right, UV.bottom);
		_kuv = UV;
	}

	void KQuad::setColor(const KColor &Color){
		_kvertex[0].color = _kvertex[1].color = _kvertex[2].color = _kvertex[3].color = Color;
		_kcolor = Color;
	}

	const KVertex *KQuad::getVertex() const{
		_kvertex[0].pos = KVector2F32(_kdim.left, _kdim.bottom) * getMatrix().getArray();
		_kvertex[1].pos = KVector2F32(_kdim.left, _kdim.top) * getMatrix().getArray();
		_kvertex[2].pos = KVector2F32(_kdim.right, _kdim.bottom) * getMatrix().getArray();
		_kvertex[3].pos = KVector2F32(_kdim.top, _kdim.right) * getMatrix().getArray();
		return _kvertex;
	}
}