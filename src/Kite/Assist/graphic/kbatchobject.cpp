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

#include "Kite/Assist/graphic/kbatchobject.h"

namespace Kite{

	KBatchObject::KBatchObject(U32 VertexSize, const std::string &Name, bool PointSprite) :
		_kvertex(new KVertex[VertexSize]),
		_kindex(0),
		_krealVSize(VertexSize),
		_krealISize(0),
		_kusedVSize(VertexSize),
		_kusedISize(0),
		_ktexture(0),
		_kshader(0),
		_kgtype(KGP_POINTS),
		_kvisible(true),
		_krelTrans(true),
		_kname(Name),
		_kindexed(false),
		_kpoint(PointSprite),
		_kreverse(false)
	{
		if (PointSprite) {
			_kpsprite = new KPointSprite[VertexSize];
		}
	}

	KBatchObject::KBatchObject(U32 VertexSize, U32 IndexSize, const std::string &Name) :
		_kvertex(new KVertex[VertexSize]),
		_kindex(new U16[IndexSize]),
		_krealVSize(VertexSize),
		_krealISize(IndexSize),
		_kusedVSize(VertexSize),
		_kusedISize(IndexSize),
		_ktexture(0),
		_kshader(0),
		_kgtype(KGP_POINTS),
		_kvisible(true),
		_krelTrans(true),
		_kname(Name),
		_kindexed(true),
		_kpoint(false),
		_kreverse(false)
	{}

	void KBatchObject::setUseVertexSize(U32 Size) {
		if (Size > _krealVSize) {
			Size = _krealVSize;
		}

		_kusedVSize = Size;
	}

	void KBatchObject::setUseIndexSize(U32 Size) {
		if (Size > _krealISize || !_kindexed) {
			Size = _krealISize;
		}

		_kusedISize = Size;
	}
}
