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
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/math/kmathdef.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/serialization/types/kstdbitset.h"
#include <luaintf/LuaIntf.h>
#include <cmath>

namespace Kite{
	void KCameraCom::inite() {
		_kdepth = 0;
		_kclearView = true;
		_kparaReset = true;
		_kflipx = false;
		_kflipy = false;
		_krtextureIndex = 0;
		_krotation = 0.0f;
		_krotateDelta = 0;
		_kzoomFactor = 1.0f;
		_kzoomDelta = 0;
		_klayers = std::bitset<32>("01");
		_kcompute = true;
		_krtexture = KSharedResource();
		_krindex = 0;
	}

	void KCameraCom::attached() {}

	void KCameraCom::deattached() {}

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
			_kparaReset = true;
		}
	}

	void KCameraCom::setCenter(const KVector2F32 &Center) {
		if (Center != _kcenter) {
			_kcenter = Center;
			_kmoveDelta = KVector2F32(0, 0);
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::move(const KVector2F32 &Move) {
		if (Move != KVector2F32(0, 0)) {
			_kmoveDelta += Move;
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::parallaxMove(const KVector2F32 &Move) {
		if (Move != KVector2F32(0, 0)) {
			_kmoveDelta += Move;
			_kcompute = true;
			_kparaReset = false;
		}
	}

	void KCameraCom::setRotation(F32 Angle) {
		if (Angle != _krotation) {
			_krotation = Angle;
			_krotateDelta = 0;
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::rotate(F32 Angle) {
		if (Angle != 0) {
			_krotateDelta += Angle;
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::parallaxRotate(F32 Angle) {
		if (Angle != 0) {
			_krotateDelta += Angle;
			_kcompute = true;
			_kparaReset = false;
		}
	}

	void KCameraCom::setZoom(F32 Factor) {
		if (Factor != _kzoomFactor) {
			_kzoomFactor = Factor;
			_kzoomDelta = 0;
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::zoom(F32 Value) {
		if (Value != 0) {
			_kzoomDelta += Value;
			_kcompute = true;
			_kparaReset = true;
		}
	}

	void KCameraCom::parallaxZoom(F32 Value) {
		if (Value != 0) {
			_kzoomDelta += Value;
			_kcompute = true;
			_kparaReset = false;
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

	KRectF32 KCameraCom::getViewRect(I32 ParallaxIndex) const {
		auto halfWidth = _ksize.x / 2;
		auto halfHeight = _ksize.y / 2;
		F32 cx = _kcenter.x + _kmoveDelta.x;
		F32 cy = _kcenter.y + _kmoveDelta.y;

		if (ParallaxIndex < (I32)_kmatList.size()) {
			if (ParallaxIndex >= 0 && !_kparaReset) {
				auto ratio = _kmatList[ParallaxIndex].second;
				cx = _kcenter.x + (_kmoveDelta.x * ratio.center.x);
				cy = _kcenter.y + (_kmoveDelta.y * ratio.center.y);
			}
		}

		return KRectF32(cx - halfWidth, cx + halfWidth, cy - halfHeight, cy + halfHeight);
	}

	const KMatrix3 &KCameraCom::getRatioMatrix(I32 Index) const {
		// check range
		if (Index < (I32)_kmatList.size()) {
			if (Index < 0 || _kparaReset) {
				return _kdefMat;
			}

			return _kmatList[Index].first;
		}

		// out of range (return identity matrix)
		KD_PRINT("index of transform ratio is out of range.");
		return _kdefMat;
	}

	I32 KCameraCom::addParallax(const KParallaxRatio &Ratio) {
		// compare with default ratio
		if (KParallaxRatio() == Ratio) {
			KD_PRINT("try to add duplicate ratio (default)");
			return -1;
		}

		// serch list
		for (SIZE i = 0; i < _kmatList.size(); ++i) {
			if (_kmatList.at(i).second == Ratio) {
				KD_PRINT("try to add duplicate ratio");
				return i;
			}
		}

		// add new ratio to list
		_kmatList.push_back({ KMatrix3(), Ratio });

		// compute new tarnsform
		computeMatrix(_kmatList.back().first, Ratio);

		// return index
		return _kmatList.size() - 1;
	}

	bool KCameraCom::setParallax(I32 Index, const KParallaxRatio &Ratio) {
		// check range
		if (Index >= (I32)_kmatList.size() || Index < 0) {
			KD_PRINT("index of transform ratio is out of range.");
			return false;
		}

		// compare with default ratio
		if (KParallaxRatio() == Ratio) {
			KD_PRINT("try to set duplicate ratio (default)");
			return false;
		}

		// serch list
		for (SIZE i = 0; i < _kmatList.size(); ++i) {
			KD_PRINT("try to set duplicate ratio");
			if (_kmatList.at(i).second == Ratio) {
				KD_PRINT("try to set duplicate ratio");
				return false;
			}
		}

		// set new ratio
		_kmatList[Index].second = Ratio;

		// recompute matrix
		computeMatrix(_kmatList[Index].first, _kmatList[Index].second);
		return true;
	}

	KParallaxRatio KCameraCom::getParallax(I32 Index) const {
		if (Index < 0) {
			return KParallaxRatio();
		}

		// check range
		if (Index < (I32)_kmatList.size()) {
			return _kmatList[Index].second;
		}

		KD_PRINT("index of transform ratio is out of range.");
		return KParallaxRatio();
	}

	void KCameraCom::clearParallax() {
		_kmatList.clear();
	}

	void KCameraCom::computeMatrixes() {
		if (_kcompute) {
			// default matrix
			computeMatrix(_kdefMat, KParallaxRatio());

			// ratios mats
			if (!_kparaReset) {
				for (auto it = _kmatList.begin(); it != _kmatList.end(); ++it) {
					computeMatrix(it->first, it->second);
				}
			}

			_kcompute = false;
		}
	}

	void KCameraCom::computeMatrix(KMatrix3 &Mat, const KParallaxRatio &Rat) {
		// set ratio
		auto rotateRat = _krotation + (_krotateDelta * Rat.rotation);
		auto centerRat = _kcenter + (_kmoveDelta * Rat.center);
		auto zoomRat = _kzoomFactor + (_kzoomDelta * Rat.zoom);

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

	KCAMERACOM_SOURCE();
	/*void KCameraCom::setViewport(const KRectI32 &Viewport){
		_kcenter.x = (Viewport.left + Viewport.right) / 2.0f;
		_kcenter.y = (Viewport.top + Viewport.bottom) / 2.0f;
		_ksize.x = Viewport.right - Viewport.left;
		_ksize.y = Viewport.top - Viewport.bottom;
	}*/

}
