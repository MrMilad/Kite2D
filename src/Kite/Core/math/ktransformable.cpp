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
#include "Kite/Core/math/ktransformable.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include <cmath>

namespace Kite{
    KTransformable::KTransformable():
        _kposition(0,0),
        _krotation(0.0f),
        _kscale(1,1),
		_kskew(0.0f, 0.0f),
        _kcenter(0,0),
		_ktransform(KMatrix3()), // unit matrix
        _kneedUpdate(true)
    {}

    KTransformable::~KTransformable(){}

    void KTransformable::setPosition(const KVector2F32& Position){
        _kposition = Position;
        _kneedUpdate = true;
    }

	void KTransformable::setPosition(F32 X, F32 Y) {
		_kposition.x = X;
		_kposition.y = Y;
		_kneedUpdate = true;
	}

    void KTransformable::move(const KVector2F32 &Steps){
		_kposition += Steps;
		_kneedUpdate = true;
    }

	void KTransformable::move(F32 X, F32 Y) {
		_kposition.x += X;
		_kposition.y += Y;
		_kneedUpdate = true;
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

	void KTransformable::setScale(F32 X, F32 Y) {
		_kscale.x = X;
		_kscale.y = Y;
		_kneedUpdate = true;
	}

    void KTransformable::scale(const KVector2F32 &Scale){
        _kscale *= Scale;
        _kneedUpdate = true;
    }

	void KTransformable::scale(F32 X, F32 Y) {
		_kscale.x *= X;
		_kscale.y *= Y;
		_kneedUpdate = true;
	}

	void KTransformable::setSkew(const KVector2F32 &Skew){
		_kskew.x = (fmod(Skew.x, 180) == 90) ? 0 : Skew.x;
		_kskew.y = (fmod(Skew.y, 180) == 90) ? 0 : Skew.y;
	}

	void KTransformable::setSkew(F32 X, F32 Y) {
		_kskew.x = (fmod(X, 180) == 90) ? 0 : X;
		_kskew.y = (fmod(Y, 180) == 90) ? 0 : Y;
	}

	void KTransformable::skew(const KVector2F32 &Skew){
		_kskew.x += (fmod(Skew.x, 180) == 90) ? 0 : Skew.x;
		_kskew.y += (fmod(Skew.y, 180) == 90) ? 0 : Skew.y;
	}

	void KTransformable::skew(F32 X, F32 Y) {
		_kskew.x += (fmod(X, 180) == 90) ? 0 : X;
		_kskew.y += (fmod(Y, 180) == 90) ? 0 : Y;
	}

    void KTransformable::setCenter(const KVector2F32 &Center){
        _kcenter = Center;
        _kneedUpdate = true;
    }

	void KTransformable::setCenter(F32 X, F32 Y) {
		_kcenter.x = X;
		_kcenter.y = Y;
		_kneedUpdate = true;
	}

	const KTransform *KTransformable::getTransform() const{
		// calculate
		if (_kneedUpdate){
			F32 angle = -_krotation * KMATH_PIsub180; // 3.14 \ 180;
			F32 skewX = _kskew.x * KMATH_PIsub180;
			F32 skewY = _kskew.y * KMATH_PIsub180;
			F32 cosine = (float)(std::cos(angle));
			F32 sine = (float)(std::sin(angle));
			F32 tanX = (float)(std::tan(skewX));
			F32 tanY = (float)(std::tan(skewY));
			F32 sxc = _kscale.x * cosine;
			F32 syc = _kscale.y * cosine;
			F32 sxs = _kscale.x * sine + tanX;
			F32 sys = _kscale.y * sine + tanY;
			F32 tx = -_kcenter.x * sxc - _kcenter.y * sys + _kposition.x;
			F32 ty = _kcenter.x * sxs - _kcenter.y * syc + _kposition.y;

			_ktransform = KTransform(KMatrix3(	sxc, sys, tx,
												-sxs, syc, ty,
												0.f, 0.f, 1.f));
			_kneedUpdate = false;
		}

		return &_ktransform;
	}
}
