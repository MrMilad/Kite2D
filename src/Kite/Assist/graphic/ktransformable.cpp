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
#include "Kite/Assist/graphic/ktransformable.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include <cmath>

namespace Kite{
    KTransformable::KTransformable():
        _kposition(0,0),
        _krotation(0.0f),
        _kscale(1,1),
        _kcenter(0,0),
		_ktransform(KMatrix3()), // unit matrix
        _kneedUpdate(true)
    {}

    KTransformable::~KTransformable(){}

    void KTransformable::setPosition(const KVector2F32& Position){
        _kposition = Position;
        _kneedUpdate = true;
    }

    void KTransformable::move(const KVector2F32 &Steps){
        setPosition(_kposition + Steps);
    }

    void KTransformable::setRotation(F32 Angle){
		if (Angle > 360.000f || Angle < -360.000f)
			Angle = fmod(Angle, 360);
        _krotation = Angle;
        _kneedUpdate = true;
    }

    void KTransformable::rotate(F32 Angle){
        setRotation(_krotation + Angle);
    }

    void KTransformable::setScale(const KVector2F32 &Scale){
        _kscale = Scale;
        _kneedUpdate = true;
    }

    void KTransformable::scale(const KVector2F32 &Scale){
        _kscale *= Scale;
        _kneedUpdate = true;
    }

    void KTransformable::setCenter(const KVector2F32 &Center){
        _kcenter = Center;
        _kneedUpdate = true;
    }

	const KTransform *KTransformable::getTransform() const{
		// calculate
		if (_kneedUpdate){
			F32 angle = -_krotation * KMATH_PIsub180; // 3.14 \ 180;
			F32 cosine = (float)(std::cos(angle));
			F32 sine = (float)(std::sin(angle));
			F32 sxc = _kscale.x * cosine;
			F32 syc = _kscale.y * cosine;
			F32 sxs = _kscale.x * sine;
			F32 sys = _kscale.y * sine;
			F32 tx = -_kcenter.x * sxc - _kcenter.y * sys + _kposition.x;
			F32 ty = _kcenter.x * sxs - _kcenter.y * syc + _kposition.y;

			_ktransform = KTransform(KMatrix3(sxc, sys, tx,
				-sxs, syc, ty,
				0.f, 0.f, 1.f));
			_kneedUpdate = false;
		}

		return &_ktransform;
	}
}
