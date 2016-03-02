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
#include "Kite/math/ktransformsys.h"
#include <cmath>

namespace Kite {

	void KTransformSys::update(F32 Delta, KEntityManager &EManager) {
		if (EManager.isRegistered<KTransformCom>()) {
			for (auto it = EManager.beginEntity(); it != EManager.endEntity(); ++it) {
				if (it->second.hasComponent(KComponentTypes::KCT_TRANSFORM, "Transform")) {
					KTransformCom *ptr = EManager.getComponent<KTransformCom>(&it->second, "Transform");
					this->computeMatrix(*ptr);
				}
			}
		}
	}

	void KTransformSys::inite() {}

	KRecieveTypes KTransformSys::onMessage(KMessage &Message, KMessageScopeTypes Scope) {
		return KRecieveTypes::KMR_IGNORED;
	}

	void KTransformSys::computeMatrix(KTransformCom &Component) {
		// calculate
		if (Component.getNeedUpdate()) {
			F32 angle = -Component._krotation * KMATH_PIsub180; // 3.14 \ 180;
			F32 skewX = Component._kskew.x * KMATH_PIsub180;
			F32 skewY = Component._kskew.y * KMATH_PIsub180;
			F32 cosine = (float)(std::cos(angle));
			F32 sine = (float)(std::sin(angle));
			F32 tanX = (float)(std::tan(skewX));
			F32 tanY = (float)(std::tan(skewY));
			F32 sxc = Component._kscale.x * cosine;
			F32 syc = Component._kscale.y * cosine;
			F32 sxs = Component._kscale.x * sine + tanX;
			F32 sys = Component._kscale.y * sine + tanY;
			F32 tx = -Component._kcenter.x * sxc - Component._kcenter.y * sys + Component._kposition.x;
			F32 ty = Component._kcenter.x * sxs - Component._kcenter.y * syc + Component._kposition.y;

			Component._kmatrix = KMatrix3(sxc, sys, tx,
											  -sxs, syc, ty,
											  0.f, 0.f, 1.f);
			Component.setNeedUpdate(false);
		}
	}
}
