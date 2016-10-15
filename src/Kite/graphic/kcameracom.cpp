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
		_kdepth(0),
		_kclearView(true),
		_kflipx(false),
		_kflipy(false),
		_kcenter(0.0f, 0.0f),
		_kposition(0, 0),
        _krotation(0.0f),
        _kzoom(1.0f),
		_kcompute(true)
    {}

	void KCameraCom::attached(KEntity *Entity) {}

	void KCameraCom::deattached(KEntity *Entity) {}

	RecieveTypes KCameraCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	void KCameraCom::setSize(const KVector2U32 &Size) {
		if (Size != _ksize) {
			_ksize = Size;
			_kcompute = true;
		}
	}

	void KCameraCom::setPosition(const KVector2I32 &Position) {
		if (Position != _kposition) {
			_kposition = Position;
			_kcompute = true;
		}
	}

	void KCameraCom::setCenter(const KVector2F32 &Center) {
		if (Center != _kcenter) {
			_kcenter = Center;
			_kcompute = true;
		}
	}

	void KCameraCom::move(const KVector2F32 &Move) {
		if (Move != KVector2F32(0, 0)) {
			_kcenter += Move;
			_kcompute = true;
		}
	}

	void KCameraCom::setRotation(F32 Angle) {
		if (Angle != _krotation) {
			_krotation = Angle;
			_kcompute = true;
		}
	}

	void KCameraCom::rotate(F32 Angle) {
		if (Angle != 0) {
			_krotation += Angle;
			_kcompute = true;
		}
	}

	void KCameraCom::setZoom(F32 Factor) {
		if (Factor != _kzoom) {
			_kzoom = Factor;
			_kcompute = true;
		}
	}

	void KCameraCom::setFlipY(bool Flip) {
		if (Flip != _kflipy) {
			_kflipy = Flip;
			_kcompute = true;
		}
	}

	void KCameraCom::setFlipX(bool Flip) {
		if (Flip != _kflipx) {
			_kflipx = Flip;
			_kcompute = true;
		}
	}

	KRectF32 KCameraCom::getViewRect() const {
		auto halfWidth = _ksize.x / 2;
		auto halfHeight = _ksize.y / 2;
		return KRectF32(_kcenter.x - halfWidth, _kcenter.x + halfWidth, _kcenter.y - halfHeight, _kcenter.y + halfHeight);
	}

	const KMatrix3 &KCameraCom::getRatioMatrix(I8 Index) const {
		if (Index < 0) {
			return _kdefMat;
		}

		// check range
		if (Index < _kmatList.size()) {
			return _kmatList[Index];
		}

		// out of range (return identity matrix)
		KD_PRINT("index of transform ratio is out of range.");
		return _kdefMat;
	}

	I8 KCameraCom::addTransformRatio(const KCameraTransformRatio &Ratio) {
		// compare with default ratio
		if (KCameraTransformRatio() == Ratio) {
			KD_PRINT("try to add duplicate ratio (default)");
			return -1;
		}

		// serch list
		for (SIZE i = 0; i < _kratList.size(); ++i) {
			if (_kratList.at(i) == Ratio) {
				KD_PRINT("try to add duplicate ratio");
				return i;
			}
		}

		// add new ratio to list
		_kratList.push_back(Ratio);
		_kmatList.push_back(KMatrix3());

		// compute new tarnsform
		computeMatrix(_kmatList.back(), _kratList.back());

		// return index
		return _kratList.size() - 1;
	}

	bool KCameraCom::setTransformRatio(I8 Index, const KCameraTransformRatio &Ratio) {
		// check range
		if (Index >= _kmatList.size() || Index < 0) {
			KD_PRINT("index of transform ratio is out of range.");
			return false;
		}

		// compare with default ratio
		if (KCameraTransformRatio() == Ratio) {
			KD_PRINT("try to set duplicate ratio (default)");
			return false;
		}

		// serch list
		for (SIZE i = 0; i < _kratList.size(); ++i) {
			KD_PRINT("try to set duplicate ratio");
			if (_kratList.at(i) == Ratio) {
				KD_PRINT("try to set duplicate ratio");
				return false;
			}
		}

		// set new ratio
		_kratList[Index] = Ratio;

		// recompute matrix
		computeMatrix(_kmatList[Index], _kratList[Index]);
	}

	const KCameraTransformRatio &KCameraCom::getTransformRatio(I8 Index) const {
		if (Index < 0) {
			return KCameraTransformRatio();
		}

		// check range
		if (Index < _kmatList.size()) {
			return _kratList[Index];
		}

		KD_PRINT("index of transform ratio is out of range.");
		return KCameraTransformRatio();
	}

	void KCameraCom::clearRatio() {
		_kmatList.clear();
		_kratList.clear();
	}

	void KCameraCom::computeMatrixes() {
		if (_kcompute) {
			// default matrix
			computeMatrix(_kdefMat, KCameraTransformRatio());

			// ratios mats
			for (SIZE i = 0; i < _kratList.size(); ++i) {
				computeMatrix(_kmatList[i], _kratList[i]);
			}

			_kcompute = false;
		}
	}

	void KCameraCom::computeMatrix(KMatrix3 &Mat, const KCameraTransformRatio &Rat) {
		// set ratio
		auto rotateRat = _krotation * Rat.rotation;
		auto centerRat = _kcenter * Rat.center;
		auto zoomRat = _kzoom * Rat.zoom;

		// compute matrix
		F32 flipy = 1.0f;
		if (_kflipy) flipy = -1.0f;
		F32 flipx = 1.0f;
		if (_kflipx) flipx = -1.0f;
		// rotation components
		F32 angle = rotateRat * KMATH_PIsub180;
		F32 cosx = (F32)(std::cos(angle));
		F32 sinx = (F32)(std::sin(angle));
		//F32 tx = -(_kviewport.right + _kviewport.left) / (_kviewport.right - _kviewport.left);
		//F32 ty = -(_kviewport.top + _kviewport.bottom) / (_kviewport.top - _kviewport.bottom);
		F32 tx = -centerRat.x * cosx - centerRat.y * sinx + centerRat.x;
		F32 ty = centerRat.x * sinx - centerRat.y * cosx + centerRat.y;

		// projection components
		F32 a = (2.0f * flipx) / (_ksize.x / zoomRat);
		F32 b = (2.0f * flipy) / (_ksize.y / zoomRat); // -2.f for microsoft windows coordinate system
		F32 c = -a * centerRat.x;
		F32 d = -b * centerRat.y;

		// rebuild the projection matrix
		Mat = KMatrix3(a * cosx, a * sinx, a * tx + c,
							-b * sinx, b * cosx, b * ty + d,
							0.0f, 0.0f, 1.0f);

		/*_kmatrix = KMatrix3(	a,		0.0f,	tx,
		0.0f,	b,		ty,
		0.0f,	0.0f,	1.0f);*/

	}

	KMETA_KCAMERACOM_SOURCE();
	/*void KCameraCom::setViewport(const KRectI32 &Viewport){
		_kcenter.x = (Viewport.left + Viewport.right) / 2.0f;
		_kcenter.y = (Viewport.top + Viewport.bottom) / 2.0f;
		_ksize.x = Viewport.right - Viewport.left;
		_ksize.y = Viewport.top - Viewport.bottom;
	}*/

}
