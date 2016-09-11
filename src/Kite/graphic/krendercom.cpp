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

#include "Kite/graphic/krendercom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	KRenderCom::KRenderCom(const std::string &Name) :
		KComponent(Name),
		_kshprogptr(nullptr),
		_ktextureptr(nullptr)
	{
		addDependency(CTypes::GCulling);
		addDependency(CTypes::Transform);
		setRemoveOnDepZero(true);
	}

	void KRenderCom::attached(KEntity *Entity) {}

	void KRenderCom::deattached(KEntity *Entity) {}

	RecieveTypes KRenderCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KRenderCom::setShader(const KStringID &Shader) {
		if (_kshprog.hash != Shader.hash) {
			_kshprog = Shader;
			setNeedUpdate(true);
		}
	}

	void KRenderCom::setAtlasTexture(const KStringID &Texture) {
		if (_ktexture.hash != Texture.hash) {
			_ktexture = Texture;
			setNeedUpdate(true);
		}
	}

	KMETA_KRENDERCOM_SOURCE();
}
