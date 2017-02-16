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
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/ecs/knode.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/graphic/kgcullingcom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	KQuadCom::KQuadCom(const std::string &Name) :
		KComponent(Name, false, { CTypes::RenderInstance }),
		_ktindex(0),
		_kisVisible(true),
		_kculling(false),
		_kwidth(100),
		_kheight(100),
		_kvertex(4),
		_kindexsize(6),
		_kshprog(nullptr),
		_katarray(nullptr)
	{
		_setDim();
		setAtlasItem(KAtlasItem());
		setBlendColor(KColor());
	}

	void KQuadCom::attached(KNode *Owner) {
		auto renderable = (KRenderCom *)Owner->getComponent(CTypes::RenderInstance);
		renderable->registerInterface(this);

		auto cullable = (KGCullingCom *)Owner->getComponent(CTypes::GCullingInstance);
		cullable->registerInterface(this);
	}

	void KQuadCom::deattached(KNode *Owner) {
		auto renderable = (KRenderCom *)Owner->getComponent(CTypes::RenderInstance);
		renderable->unregisterInterface(this);

		auto cullable = (KGCullingCom *)Owner->getComponent(CTypes::GCullingInstance);
		cullable->unregisterInterface(this);
	}

	RecieveTypes KQuadCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
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
		_kvertex[0].pos = KVector2F32(0, 0);
		_kvertex[1].pos = KVector2F32(0, _kheight);
		_kvertex[2].pos = KVector2F32(_kwidth , 0);
		_kvertex[3].pos = KVector2F32(_kwidth, _kheight);
	}

	void KQuadCom::setAtlasItem(const KAtlasItem &AtlasItem) {
		_katlasItem = AtlasItem;
		_kvertex[1].uv = KVector2F32(AtlasItem.blu, AtlasItem.blv);
		_kvertex[0].uv = KVector2F32(AtlasItem.blu, AtlasItem.trv);
		_kvertex[3].uv = KVector2F32(AtlasItem.tru, AtlasItem.blv);
		_kvertex[2].uv = KVector2F32(AtlasItem.tru, AtlasItem.trv);
	}

	void KQuadCom::getBoundingRect(KRectF32 &Output) const {
		Output.bottom = 0;
		Output.left = 0;
		Output.top = _kheight;
		Output.right = _kwidth;
	}

	/*void KQuadCom::setUV(const KRectF32 &UV){
		_kvertex[0].uv = KVector2F32(UV.left, UV.bottom);
		_kvertex[1].uv = KVector2F32(UV.left, UV.top);
		_kvertex[2].uv = KVector2F32(UV.right, UV.bottom);
		_kvertex[3].uv = KVector2F32(UV.right, UV.top);
		_kuv = UV;
	}*/

	void KQuadCom::setBlendColor(const KColor &Color){
		_kvertex[0].r = _kvertex[1].r = _kvertex[2].r = _kvertex[3].r = Color.getR();
		_kvertex[0].g = _kvertex[1].g = _kvertex[2].g = _kvertex[3].g = Color.getG();
		_kvertex[0].b = _kvertex[1].b = _kvertex[2].b = _kvertex[3].b = Color.getB();
		_kvertex[0].a = _kvertex[1].a = _kvertex[2].a = _kvertex[3].a = Color.getA();
		_kcolor = Color;
	}

	void KQuadCom::setShader(KShaderProgram *ShaderProgram) {
		if (_kshprog != ShaderProgram) {
			_kshprog = ShaderProgram;
			matNeedUpdate();
		}
	}

	void KQuadCom::setAtlasTextureArraye(KAtlasTextureArray *TextureArray) {
		if (_katarray != TextureArray) {
			_katarray = TextureArray;
			matNeedUpdate();
		}
	}

	void KQuadCom::setTextureArrayIndex(U16 Index) {
		_kvertex[0].tindex = _kvertex[1].tindex = _kvertex[2].tindex = _kvertex[3].tindex = Index;
		_ktindex = Index;
	}

	KMETA_KQUADCOM_SOURCE();
}