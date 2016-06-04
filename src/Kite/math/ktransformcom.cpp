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
#include "Kite/math/ktransformcom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf/LuaIntf.h>
#include <cmath>

namespace Kite{
	KTransformCom::KTransformCom(const std::string &Name) :
		KComponent("Transform", Name)
	{
		_kposition = KVector2F32(0.0f, 0.0f);
		_krotation = 0.0f;
		_kscale = KVector2F32(1.0f, 1.0f);
		_kskew = KVector2F32(0.0f, 0.0f);
		_kcenter = KVector2F32(0.0f, 0.0f);
		setNeedUpdate(true);
	}

	void KTransformCom::attached() {}

	void KTransformCom::deattached() {}

    void KTransformCom::setPosition(const KVector2F32& Position){
        _kposition = Position;
		setNeedUpdate(true);
    }

	RecieveTypes KTransformCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

    void KTransformCom::setRotation(F32 Angle){
		if (Angle > 360.000f || Angle < -360.000f)
			Angle = (F32)fmod(Angle, 360);
        _krotation = Angle;
		setNeedUpdate(true);
    }

    void KTransformCom::setScale(const KVector2F32 &Scale){
        _kscale = Scale;
		setNeedUpdate(true);
    }

	void KTransformCom::setSkew(const KVector2F32 &Skew){
		_kskew.x = (fmod(Skew.x, 180) == 90) ? 0 : Skew.x;
		_kskew.y = (fmod(Skew.y, 180) == 90) ? 0 : Skew.y;
		setNeedUpdate(true);
	}

    void KTransformCom::setCenter(const KVector2F32 &Center){
        _kcenter = Center;
		setNeedUpdate(true);
    }

	KMETA_KTRANSFORMCOM_SOURCE();
}
