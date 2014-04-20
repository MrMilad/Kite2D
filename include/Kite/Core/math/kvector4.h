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
#ifndef KVECTOR4_H
#define KVECTOR4_H

#include "Kite/core/system/ksystemdef.h"

namespace Kite {
	template <typename T>
	class KVector4{
	public:
		explicit KVector4(T X = 0, T Y = 0, T Z = 0, T W = 0) :
			x(X), y(Y), z(Z)
		{}

		T x, y, z, w;
	};

	template <typename T>
	static inline KVector4<T> operator-(const KVector4<T>& right){
		return KVector4<T>(-right.x, -right.y);
	}

	template <typename T>
	static inline KVector4<T>& operator+=(KVector4<T>& left, const KVector4<T>& right){
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		left.w += right.w;
		return left;
	}

	template <typename T>
	static inline KVector4<T>& operator-=(KVector4<T>& left, const KVector4<T>& right){
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		left.w -= right.w;
		return left;
	}

	template <typename T>
	static inline KVector4<T> operator+(const KVector4<T>& left, const KVector4<T>& right){
		return KVector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	template <typename T>
	static inline KVector4<T> operator-(const KVector4<T>& left, const KVector4<T>& right){
		return KVector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	template <typename T>
	static inline KVector4<T> operator*(const KVector4<T>& left, T right){
		return KVector4<T>(left.x * right, left.y * right, left.z * right, left.w * right);
	}

	// optimized for 2d purpose
	template <typename T>
	static inline KVector4<T> operator*(const KVector4<T>& left, const F32 *Matrix4){
		/*return KVector4<T>(	Matrix4[0] * left.x + Matrix4[4] * left.y + Matrix4[12],
							Matrix4[1] * left.x + Matrix4[5] * left.y + Matrix4[13],
							Matrix4[2] * left.x + Matrix4[6] * left.y + Matrix4[14],
							Matrix4[3] * left.x + Matrix4[7] * left.y + Matrix4[15]);*/
		return KVector4<T>(	Matrix4[0] * left.x + Matrix4[4] * left.y + Matrix4[12],
							Matrix4[1] * left.x + Matrix4[5] * left.y + Matrix4[13],
							0,
							1);
	}

	template <typename T>
	static inline KVector4<T> operator*(T left, const KVector4<T>& right){
		return KVector4<T>(right.x * left, right.y * left, right.z * left, right.w * left);
	}

	template <typename T>
	static inline KVector4<T>& operator*=(KVector4<T>& left, T right){
		left.x *= right;
		left.y *= right;
		left.z *= right;
		left.w *= right;
		return left;
	}

	template <typename T>
	static inline KVector4<T>& operator*=(KVector4<T>& left, const KVector4<T>& right){
		left.x *= right.x;
		left.y *= right.y;
		left.z *= right.z;
		left.w *= right.w;
		return left;
	}


	template <typename T>
	static inline KVector4<T> operator/(const KVector4<T>& left, T right){
		return KVector4<T>(left.x / right, left.y / right, left.z / right, left.w / right);
	}

	template <typename T>
	static inline KVector4<T>& operator/=(KVector4<T>& left, T right){
		left.x /= right;
		left.y /= right;
		left.z /= right;
		left.w /= right;
		return left;
	}

	template <typename T>
	static inline KVector4<T>& operator/=(KVector4<T>& left, const KVector4<T>& right){
		left.x /= right.x;
		left.y /= right.y;
		left.z /= right.z;
		left.w /= right.w;
		return left;
	}

	template <typename T>
	static inline bool operator==(const KVector4<T>& left, const KVector4<T>& right){
		return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
	}

	template <typename T>
	static inline bool operator!=(const KVector4<T>& left, const KVector4<T>& right){
		return (left.x != right.x) || (left.y != right.y) || (left.z != right.z) || (left.w != right.w);
	}

	template <typename T>
	static inline void move(KVector4<T>& v, T mx, T my, T mz, T mw) { v.x += mx; v.y += my; v.z += mz; v.w += mw; }

	typedef KVector4<U8>  KVector4U8;
	typedef KVector4<U16> KVector4U16;
	typedef KVector4<U32> KVector4U32;
	typedef KVector4<I8>  KVector4I8;
	typedef KVector4<I16> KVector4I16;
	typedef KVector4<I32> KVector4I32;
	typedef KVector4<F32> KVector4F32;
	typedef KVector4<F64> KVector4F64;

} //namespace
#endif // KVECTOR4_H