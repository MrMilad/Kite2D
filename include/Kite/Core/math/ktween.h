/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KTWEEN_H
#define KTWEEN_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathdef.h"
#include "Kite/Core/math/kmathtypes.h"
#include <cmath>

namespace Kite{
	class KTween{
	public:
		// Time = current time
		// Start = start point
		// End = end point
		// Duration = duration between start and end point
		static inline F32 linear(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return (End - Start) * Time + Start;
		}

		static inline F32 quadraticIn(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return (End - Start) * (Time * Time) + Start;
		}


		static inline F32 quadraticOut(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return (End - Start) * (Time * (2 - Time)) + Start;
		}

		static inline F32 quadraticInOut(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration / 2;
			if (Time < 1) return (((End - Start) / 2) * (Time * Time)) + Start;
			return -(End - Start) / 2 * (((Time - 2) * (--Time)) - 1) + Start;
		}

		static inline F32 sinusoidalIn(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return -(End - Start) * cos(Time * KMATH_PI_2) + (End - Start) + Start;
		}

		static inline F32 sinusoidalOut(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return (End - Start) * sin(Time * KMATH_PI_2) + Start;
		}

		static inline F32 sinusoidalInOut(F32 Time, F32 Start, F32 End, F32 Duration){
			Time /= Duration;
			return -(End - Start) / 2 * (cos(KMATH_PI * Time) - 1) + Start;
		}
	};
}

#endif // KTWEEN_H