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
#ifndef KTRANSFORMSYS_H
#define KTRANSFORMSYS_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/ksystem.h"
#include "Kite/math/kmathdef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/ktransformcom.h"
#include "ktransformsys.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(SYSTEM)
	class KITE_FUNC_EXPORT KTransformSys: public KSystem {
		KMETA_KTRANSFORMSYS_BODY();
	public:
		void update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager);

		bool inite(void *Data);

		KM_FUN()
		void computeMatrix(KTransformCom &Component);
	};
}

#endif // KTRANSFORMSYS_H