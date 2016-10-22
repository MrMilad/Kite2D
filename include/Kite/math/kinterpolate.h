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
#ifndef KINTERPOLATE_H
#define KINTERPOLATE_H

#include "Kite/core/kcoredef.h"
#include "Kite/math/kmathdef.h"
#include "kite/math/ktween.h"

namespace Kite {

	// interpolate integer primitive types 
	template<typename T>
	KITE_FUNC_EXPORT extern void interpolate(T &Inst, F32 Time, T Start, T End, F32 Duration, KInterpolationTypes FType);

	// bool
	template<>
	KITE_FUNC_EXPORT extern void interpolate<bool>(bool &Inst, F32 Time, bool Start, bool End, F32 Duration, KInterpolationTypes FType);


}

#endif // KINTERPOLATE_H
