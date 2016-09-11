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
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KOrthoMapCom::KOrthoMapCom(const std::string &Name) :
		KComponent(Name),
		_kcullIsValid(false)
	{
		addDependency(CTypes::RenderMaterial);
	}

	void KOrthoMapCom::attached(KEntity *Entity) {}

	void KOrthoMapCom::deattached(KEntity *Entity) {}

	RecieveTypes KOrthoMapCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KOrthoMapCom::setMap(const KStringID &ResName) {
		if (ResName.hash != _kmapName.hash) {
			_kmapName = ResName;
			setNeedUpdate(true);
		}
	}

	void KOrthoMapCom::setCullingArea(const KRectF32 &Area) {
		if (_kcullArea != Area) {
			_kcullArea = Area;
			setNeedUpdate(true);
		}
	}

	void KOrthoMapCom::getBoundingRect(KRectF32 &Output) const{
		Output.bottom = 0;
		Output.left = 0;
		Output.top = 0;
		Output.right = 0;
		if (_kmap != nullptr) {
			Output.bottom = _kpos.y;
			Output.left = _kpos.x;

			Output.top = _kpos.y + (_kmap->getMapHeight() * _kmap->getTileHeight());
			Output.right = _kpos.x+ (_kmap->getMapWidth() * _kmap->getTileWidth());
		}
	}

	void KOrthoMapCom::setPosition(const KVector2F32& Position) {
		if (_kpos != Position) {
			_kpos = Position;
			setNeedUpdate(true);
		}
	}

	void KOrthoMapCom::queryVerts() {
		_kverts.clear();
		if (_kmap != nullptr) {
			KRectF32 area = _kcullArea - _kpos;
			_kmap->queryTilesVertex(area, _kverts);
		}
	}

	KMETA_KORTHOMAPCOM_SOURCE();
}