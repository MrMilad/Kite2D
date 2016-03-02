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
#ifndef KMATHDEF_H
#define KMATHDEF_H

#include "Kite/core/kcoredef.h"

namespace Kite {

	// Common math constants
	#define KMATH_PI	3.14159265358979323846f
	#define KMATH_PI_2	1.57079632679489661923f
	#define KMATH_PI_4	0.785398163397448309616f

	#define KMATH_PIsub45  0.06981317008f
	#define KMATH_PIsub90  0.03490658504f
	#define KMATH_PIsub180 0.01745329252f
	#define KMATH_PIsub240 0.01308996939f
	#define KMATH_PIsub360 0.00872664626f

	template <typename T, typename P>
	using KCallTween = P(*)(T Time, P Start, P End, T Duration);
	typedef F64(*KCallEasingFloat)(F64 Time, F32 Start, F32 End, F32 Duration);
	typedef I64(*KCallEasingInt)(I64 Time, I32 Start, I32 End, I32 Duration);
}

#endif // KMATHDEF_H