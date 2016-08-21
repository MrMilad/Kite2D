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
#include "Kite/graphic/kquadcom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	KQuadCom::KQuadCom(const std::string &Name) :
		KComponent(Name),
		_kreverse(false),
		_kborder(0),
		_kwidth(100),
		_kheight(100),
		_kvertex(4),
		_kindexsize(6)
	{
		addDependency("Render");
		_setDim();
		//setUV(KRectF32(0.0f, 1.0f, 0.0f, 1.0f));
		setColor(KColor());
	}

	void KQuadCom::attached(KEntity *Owner) {}

	void KQuadCom::deattached(KEntity *Owner) {}

	RecieveTypes KQuadCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KQuadCom::setBorder(F32 Border) {
		_kborder = Border;
		_setDim();
	}

	void KQuadCom::setWidth(F32 Width) {
		_kwidth = Width;
		_setDim();
	}

	void KQuadCom::setHeight(F32 Height) {
		_kheight = Height;
		_setDim();
	}

	void KQuadCom::_setDim() {
		_kvertex[0].pos = KVector2F32(-(_kwidth / 2), -(_kheight / 2));
		_kvertex[1].pos = KVector2F32(-(_kwidth / 2), (_kheight / 2));
		_kvertex[2].pos = KVector2F32((_kwidth / 2), -(_kheight / 2));
		_kvertex[3].pos = KVector2F32((_kwidth / 2), (_kheight / 2));

		if (_kborder > 0) {
			_kvertex.resize(8);
			_kindexsize = 12;
			_kreverse = true;
			_kvertex[4].pos = KVector2F32(-((_kwidth / 2) + _kborder), -((_kheight / 2) + _kborder));
			_kvertex[5].pos = KVector2F32(-((_kwidth / 2) + _kborder), (_kheight / 2) + _kborder);
			_kvertex[6].pos = KVector2F32((_kwidth / 2) + _kborder, -((_kheight / 2) + _kborder));
			_kvertex[7].pos = KVector2F32((_kwidth / 2) + _kborder, (_kheight / 2) + _kborder);

		} else {
			_kreverse = false;
			_kvertex.resize(4);
			_kindexsize = 6;
		}
	}

	/*void KQuadCom::setUV(const KRectF32 &UV){
		_kvertex[0].uv = KVector2F32(UV.left, UV.bottom);
		_kvertex[1].uv = KVector2F32(UV.left, UV.top);
		_kvertex[2].uv = KVector2F32(UV.right, UV.bottom);
		_kvertex[3].uv = KVector2F32(UV.right, UV.top);
		_kuv = UV;
	}*/

	void KQuadCom::setBorderColor(const KColor &Color) {
		if (_kborder > 0) {
			_kvertex[4].color = _kvertex[5].color = _kvertex[6].color = _kvertex[7].color = Color;
		}
		_kbcolor = Color;
	}

	void KQuadCom::setColor(const KColor &Color){
		_kvertex[0].color = _kvertex[1].color = _kvertex[2].color = _kvertex[3].color = Color;
		_kcolor = Color;
	}

	KMETA_KQUADCOM_SOURCE();
}