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
#include "Kite/graphic/korthomapcom.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/core/kresourcemanager.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KOrthoMapCom::KOrthoMapCom(const std::string &Name) :
		KComponent(Name),
		_kcullIsValid(false),
		_ktindex(0),
		_kisVisible(true),
		_kisize(0),
		_kmap(nullptr),
		_kshprog(nullptr),
		_katarray(nullptr),
		_kquery(true)
	{
		addDependency(CTypes::RenderInstance);
	}

	void KOrthoMapCom::attached(KEntity *Entity) {
		auto renderable = (KRenderCom *)Entity->getComponent(CTypes::RenderInstance);
		renderable->setRenderable(getType());
	}

	void KOrthoMapCom::deattached(KEntity *Entity) {}

	RecieveTypes KOrthoMapCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KOrthoMapCom::setMap(const KStringID &ResName) {
		if (ResName.hash != _kmapName.hash) {
			_kmapName = ResName;
			resNeedUpdate();
			_kquery = true;
		}
	}

	void KOrthoMapCom::setCullingArea(const KRectF32 &Area) {
		if (_kcullArea != Area) {
			_kcullArea = Area;
			_kquery = true;
			queryVerts();
		}
	}

	void KOrthoMapCom::getBoundingRect(KRectF32 &Output) const{
		Output.bottom = 0;
		Output.left = 0;
		Output.top = 0;
		Output.right = 0;
		if (_kmap != nullptr) {
			Output.top = _kmap->getMapHeightPixel();
			Output.right = _kmap->getMapWidthPixel();
		}
	}

	bool KOrthoMapCom::updateRes(){
		if (!getResNeedUpdate()) {
			return true;
		}

		// load resources
		if (getRMan()) {
			_katarray = (KAtlasTextureArray *)getRMan()->get(_ktextureArrayName.str);
			_kshprog = (KShaderProgram *)getRMan()->get(_kshprogName.str);
			_kmap = (KOrthogonalMap *)getRMan()->get(_kmapName.str);
			queryVerts();
			resUpdated();
			return true;
		}

		return false;
	}

	void KOrthoMapCom::queryVerts() {
		if (_kmap == nullptr) {
			_kverts.resize(0);
			return;
		}

		if (_kquery) {
			_kverts.resize(0);
			_kmap->queryTilesVertex(_kcullArea, _kverts);
			_kisize = (_kverts.size() / 4) * 6;
			_kquery = false;
		}
	}

	void KOrthoMapCom::setShader(const KStringID &Shader) {
		if (_kshprogName.hash != Shader.hash) {
			_kshprogName = Shader;
			resNeedUpdate();
			matNeedUpdate();
		}
	}

	void KOrthoMapCom::setAtlasTextureArraye(const KStringID &TextureArrayName) {
		if (_ktextureArrayName.hash != TextureArrayName.hash) {
			_ktextureArrayName = TextureArrayName;
			resNeedUpdate();
			matNeedUpdate();
		}
	}

	KMETA_KORTHOMAPCOM_SOURCE();
}