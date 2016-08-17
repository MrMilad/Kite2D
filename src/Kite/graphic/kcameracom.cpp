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
#include "Kite/graphic/kcameracom.h"
#include "Kite/math/kmathdef.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>
#include <cmath>

namespace Kite{
	KCameraCom::KCameraCom(const std::string &Name) :
		KComponent(Name),
		_kflipx(false),
		_kflipy(false),
		_kcenter(0.0f, 0.0f),
		_kposition(0.0f, 0.0f),
        _krotation(0.0f),
        _kzoom(1.0f)
    {}

	void KCameraCom::attached(KEntity *Entity) {}

	void KCameraCom::deattached(KEntity *Entity) {}

	RecieveTypes KCameraCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KCameraCom::setSize(const KVector2U32 &Size) {
		if (Size != _ksize) {
			_ksize = Size;
			_kcenter.x = Size.x / 2.0f;
			_kcenter.y = Size.y / 2.0f;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::setPosition(const KVector2F32 &Position) {
		if (Position != _kposition) {
			_kposition = Position;
			_kcenter += Position;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::move(const KVector2F32 &Move) {
		if (Move != KVector2F32(0, 0)) {
			_kposition += Move;
			_kcenter += Move;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::setRotation(F32 Angle) {
		if (Angle != _krotation) {
			_krotation = Angle;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::rotate(F32 Angle) {
		if (Angle != 0) {
			_krotation += Angle;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::setZoom(F32 Factor) {
		if (Factor != _kzoom) {
			_kzoom = Factor;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::setFlipY(bool Flip) {
		if (Flip != _kflipy) {
			_kflipy = Flip;
			setNeedUpdate(true);
		}
	}

	void KCameraCom::setFlipX(bool Flip) {
		if (Flip != _kflipx) {
			_kflipx = Flip;
			setNeedUpdate(true);
		}
	}

	KMETA_KCAMERACOM_SOURCE();
	/*void KCameraCom::setViewport(const KRectI32 &Viewport){
		_kcenter.x = (Viewport.left + Viewport.right) / 2.0f;
		_kcenter.y = (Viewport.top + Viewport.bottom) / 2.0f;
		_ksize.x = Viewport.right - Viewport.left;
		_ksize.y = Viewport.top - Viewport.bottom;
	}*/

}
