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
#ifndef KBOXCULLABLE_H
#define KBOXCULLABLE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/bsp/kspatialhash.h"
#include "kboxcullable.khgen.h"

namespace Kite {
	class KBSPSys;
	KM_CLASS(INTERFACE)
	class KITE_FUNC_EXPORT KBoxCullable {
		KM_INFO(KI_NAME = "BoxCullable");
		KBOXCULLABLE_BODY();
		friend class KBSPSys;
	public:
		virtual ~KBoxCullable() {}

	protected:
		virtual void getBoundingBox(KRectU32 &Output) const = 0;
		virtual const KBitset32 &getChannel() const = 0; /// active culling channels (filters)

	private:
		KSHObject proxy;
	};
	
	KBOXCULLABLE_SOURCE();
}

#endif // KBOXCULLABLE_H
