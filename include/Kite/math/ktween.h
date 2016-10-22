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

#include "Kite/core/kcoredef.h"
#include "Kite/math/kmathdef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathtypes.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/kbaseserial.h"
#include "luaintf\LuaIntf.h"
#include <cmath>
#include "ktween.khgen.h"

namespace Kite{
	// TIME: F64 or I64     PARAM: F32 or I32
	template <typename TIME, typename PARAM>
	KM_CLASS(POD)
	class KITE_FUNC_EXPORT KTween{
		KM_TEM_PARAM(TIME, PARAM);
		KM_TEM_DEF("KTweenF32", F64, F32);
		KM_TEM_DEF("KTweenI32", F64, I32);

		KMETA_KTWEEN_BODY();
	public:
		// Time = current time
		// Start = start point
		// End = end point
		// Duration = duration between start and end point
		KM_FUN()
		static PARAM linear(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return (PARAM)((End - Start) * Time + Start);
		}

		KM_FUN()
		static PARAM quadraticIn(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return (PARAM)((End - Start) * (Time * Time) + Start);
		}

		KM_FUN()
		static PARAM quadraticOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return ((PARAM)(End - Start) * (Time * (2 - Time)) + Start);
		}

		KM_FUN()
		static PARAM quadraticInOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration / 2;
			if (Time < 1) return (((End - Start) / 2) * (Time * Time)) + Start;
			return (PARAM)(-(End - Start) / 2 * (((Time - 2) * (--Time)) - 1) + Start);
		}

		KM_FUN()
		static PARAM sinusoidalIn(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return (PARAM)(-(End - Start) * cos(Time * KMATH_PI_2) + (End - Start) + Start);
		}

		KM_FUN()
		static PARAM sinusoidalOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return (PARAM)((End - Start) * sin(Time * KMATH_PI_2) + Start);
		}

		KM_FUN()
		static PARAM sinusoidalInOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			Time /= Duration;
			return (PARAM)(-(End - Start) / 2 * (cos(KMATH_PI * Time) - 1) + Start);
		}

		KM_FUN()
		static PARAM exponentialIn(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			return (PARAM)((Time == 0) ? Start : (End - Start) * pow(2, 10 * (Time / Duration - 1)) + Start);
		}

		KM_FUN()
		static PARAM exponentialOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			return (PARAM)((Time == Duration) ? Start + (End - Start) : (End - Start) * (-pow(2, -10 * Time / Duration) + 1) + Start);
		}

		KM_FUN()
		static PARAM exponentialInOut(TIME Time, PARAM Start, PARAM End, TIME Duration) {
			if (Time == 0) return Start;
			if (Time == Duration) return Start + (End - Start);
			if ((Time /= Duration / 2) < 1) return (End - Start) / 2 * pow(2, 10 * (Time - 1)) + Start;
			return (PARAM)((End - Start) / 2 * (-pow(2, -10 * --Time) + 2) + Start);

		}
	};
}

#endif // KTWEEN_H