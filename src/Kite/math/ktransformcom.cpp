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
#include "Kite/math/kmathdef.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/ecs/kresourcemanager.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf/LuaIntf.h>
#include <cmath>

namespace Kite{
	void KTransformCom::inite() {
		_kcompute = true;
		_krotation = 0.0f;
		_kscale.x = 1.0f;
		_kscale.y = 1.0f;
		_ksysIndex = 0;
	}

	void KTransformCom::attached() {}

	void KTransformCom::deattached() {}

    void KTransformCom::setPosition(const KVector2F32& Position){
		if (_kposition != Position) {
			_kposition = Position;
			_kcompute = true;
		}
    }

    void KTransformCom::setRotation(F32 Angle){
		if (Angle > 360.000f || Angle < -360.000f)
			Angle = (F32)fmod(Angle, 360);

		if (_krotation != Angle) {
			_krotation = Angle;
			_kcompute = true;
		}
    }

    void KTransformCom::setScale(const KVector2F32 &Scale){
		if (_kscale != Scale) {
			_kscale = Scale;
			_kcompute = true;
		}
    }

    void KTransformCom::setCenter(const KVector2F32 &Center){
		if (_kcenter != Center) {
			_kcenter = Center;
			_kcompute = true;
		}
    }

	void KTransformCom::computeMatrix() {
		if (_kcompute) {
			const F32 angle = -_krotation * KMATH_PIsub180; // 3.14 \ 180;
			const F32 cosine = static_cast<float>(std::cos(angle));
			const F32 sine = static_cast<float>(std::sin(angle));
			const F32 sxc = _kscale.x * cosine;
			const F32 syc = _kscale.y * cosine;
			const F32 sxs = _kscale.x * sine;
			const F32 sys = _kscale.y * sine;
			const F32 tx = -_kcenter.x * sxc - _kcenter.y * sys + _kposition.x;
			const F32 ty = _kcenter.x * sxs - _kcenter.y * syc + _kposition.y;

			_kmatrix = KMatrix3(sxc, sys, tx,
								-sxs, syc, ty,
								0.f, 0.f, 1.f);
			_kcompute = false;
		}
	}

	KTRANSFORMCOM_SOURCE();
}
