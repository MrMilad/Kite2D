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
#ifndef KTWEEN_H
#define KTWEEN_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathdef.h"
#include "Kite/Core/math/kmathtypes.h"
#include <cmath>

namespace Kite{
	// TIME: F64 or I64     PARAM: F32 or I32
	template <typename TIME, typename PARAM>
	class KITE_FUNC_EXPORT KTween{
	public:
		// Time = current time
		// Start = start point
		// End = end point
		// Duration = duration between start and end point
		static inline TIME linear(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return (End - Start) * Time + Start;
		}

		static inline TIME quadraticIn(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return (End - Start) * (Time * Time) + Start;
		}


		static inline TIME quadraticOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return (End - Start) * (Time * (2 - Time)) + Start;
		}

		static inline TIME quadraticInOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration / 2;
			if (Time < 1) return (((End - Start) / 2) * (Time * Time)) + Start;
			return -(End - Start) / 2 * (((Time - 2) * (--Time)) - 1) + Start;
		}

		static inline TIME sinusoidalIn(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return -(End - Start) * cos(Time * KMATH_PI_2) + (End - Start) + Start;
		}

		static inline TIME sinusoidalOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return (End - Start) * sin(Time * KMATH_PI_2) + Start;
		}

		static inline TIME sinusoidalInOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			Time /= Duration;
			return -(End - Start) / 2 * (cos(KMATH_PI * Time) - 1) + Start;
		}

		static inline TIME exponentialIn(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			return (Time == 0) ? Start : (End - Start) * pow(2, 10 * (Time / Duration - 1)) + Start;
		}

		static inline TIME exponentialOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			return (Time == Duration) ? Start + (End - Start) : (End - Start) * (-pow(2, -10 * Time / Duration) + 1) + Start;
		}

		static inline TIME exponentialInOut(TIME Time, PARAM Start, PARAM End, PARAM Duration) {
			if (Time == 0) return Start;
			if (Time == Duration) return Start + (End - Start);
			if ((Time /= Duration / 2) < 1) return (End - Start) / 2 * pow(2, 10 * (Time - 1)) + Start;
			return (End - Start) / 2 * (-pow(2, -10 * --Time) + 2) + Start;

		}
	};
}

#endif // KTWEEN_H