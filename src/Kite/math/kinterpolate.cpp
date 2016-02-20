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
#include "Kite/Core/math/kinterpolate.h"

namespace Kite {
	// interpolate primitive types
	template<typename T>
	void interpolate(T &Inst, F32 Time, T Start, T End, F32 Duration, KInterpolationTypes FType) {
		static KCallTween<F32, T> tweenFunction[] = { KTween<F32, T>::linear, KTween<F32, T>::quadraticIn,
			KTween<F32, T>::quadraticOut, KTween<F32, T>::quadraticInOut, KTween<F32, T>::sinusoidalIn,
			KTween<F32, T>::sinusoidalOut, KTween<F32, T>::sinusoidalInOut, KTween<F32, T>::exponentialIn,
			KTween<F32, T>::exponentialOut, KTween<F32, T>::exponentialInOut };
		Inst = tweenFunction[(U8)FType](Time, Start, End, Duration);
	}

	// bool
	void interpolate(bool &Inst, F32 Time, bool Start, bool End, F32 Duration, KInterpolationTypes FType) {
		if (Time >= Duration) { Inst = End; } else { Inst = Start; }
	}

}