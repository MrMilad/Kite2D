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
#ifndef KCULLABLE_H
#define KCULLABLE_H

#include "Kite/core/kcoredef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"

namespace Kite {
	class KGCullingSys;
	class KITE_FUNC_EXPORT KCullable {
		friend class KGCullingSys;
	public:
		virtual ~KCullable() {}

	protected:
		virtual void getBoundingRect(KRectF32 &Output) const = 0;
		virtual bool isActiveCulling() const = 0;
	};
}

#endif // KCULLABLE_H