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
#ifndef KBASESERIAL_H
#define KBASESERIAL_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/memory/kbasestorage.h"
#include <type_traits>

#define CHECK_VALIDATION(T) static_assert(std::is_arithmetic<T>::value, "non-pod, unregistered or unserializable types and raw pointers are not allowed for serialization.")

namespace Kite {

	class KITE_FUNC_EXPORT KBaseSerial {
	public:
		// use default standard allocator
		KBaseSerial();

		// use custom allocator and deleter
		KBaseSerial(KBaseStorage *Allocator);
		virtual void writePOD(const void *Value, SIZE Size, bool Str) = 0;
		virtual void readPOD(void *Value, SIZE Size, bool Str) = 0;

		inline KBaseStorage *getAllocator() const { return _kallocator; }
	protected:
		KBaseStorage *_kallocator;
	};

// type validation 
// only non-pointer POD types allowed

	namespace Internal {
		// for PODs by default
		template <typename T1>
		struct serialHelper1 {
			static void write(KBaseSerial &Out, const T1 &Value) {
				CHECK_VALIDATION(T1);

				Out.writePOD((const void *)&Value, sizeof(T1), false);
			}

			static void read(KBaseSerial &In, T1 &Value) {
				CHECK_VALIDATION(T1);

				In.readPOD((void *)&Value, sizeof(T1), false);
			}
		};

		// continers with 1 templated argument
		template <template<typename>typename T1, typename T2>
		struct serialHelper2 {};

		// continers with 2 templated argument
		template <template<typename, typename>typename T1, typename T2, typename T3>
		struct serialHelper3 {};
	}

	// Serialization
	// pod types
	template <typename T1>
	KBaseSerial &operator<<(KBaseSerial &Out, const T1 &Value) {
		Internal::serialHelper1<T1>::write(Out, Value);
		return Out;
	}

	// continers with 1 templated argument
	template <template<typename>typename T1, typename T2>
	KBaseSerial &operator<<(KBaseSerial &Out, const T1<T2> &Value) {
		Internal::serialHelper2<T1, T2>::write(Out, Value);
		return Out;
	}

	// continers with 2 templated argument
	template <template<typename, typename>typename T1, typename T2, typename T3>
	KBaseSerial &operator<<(KBaseSerial &Out, const T1<T2, T3> &Value) {
		Internal::serialHelper3<T1, T2, T3>::write(Out, Value);
		return Out;
	}

	// Deserialization
	// POD
	template <typename T1>
	KBaseSerial &operator>>(KBaseSerial &In, T1 &Value) {
		Internal::serialHelper1<T1>::read(In, Value);
		return In;
	}

	// continers with 1 templated argument
	template <template<typename>typename T1, typename T2>
	KBaseSerial &operator>>(KBaseSerial &In, T1<T2> &Value) {
		Internal::serialHelper2<T1, T2>::read(In, Value);
		return In;
	}

	// continers with 2 templated argument
	template <template<typename, typename>typename T1, typename T2, typename T3>
	KBaseSerial &operator>>(KBaseSerial &In, T1<T2, T3> &Value) {
		Internal::serialHelper3<T1, T2, T3>::read(In, Value);
		return In;
	}

	// 1-dimensional array
	template<typename T, SIZE N>
	KBaseSerial &operator<<(KBaseSerial &Out, const T(&Value)[N]) {

		for (SIZE i = 0; i < N; i++) {
			Out << Value[i];
		}
		
		return Out;
	}

	template<typename T, SIZE N>
	KBaseSerial &operator>>(KBaseSerial &In, T(&Value)[N]) {

		for (SIZE i = 0; i < N; i++) {
			In >> Value[i];
		}

		return In;
	}

	// 2-dimensional array
	template<typename T, SIZE N, SIZE Y>
	KBaseSerial &operator<<(KBaseSerial &Out, const T(&Value)[N][Y]) {
		for (SIZE i = 0; i < N; i++) {
			for (SIZE j = 0; j < Y; j++) {
				Out << Value[i][j];
			}
		}

		return Out;
	}

	template<typename T, SIZE N, SIZE Y>
	KBaseSerial &operator>>(KBaseSerial &In, T(&Value)[N][Y]) {
		for (SIZE i = 0; i < N; i++) {
			for (SIZE j = 0; j < Y; j++) {
				In >> Value[i][j];
			}
		}

		return In;
	}
}

#endif // KBASESERIAL_H
