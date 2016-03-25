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
#ifndef KMATHSTRUCTS_H
#define KMATHSTRUCTS_H

#include <cmath>
#include "Kite/core/kcoredef.h"

namespace Kite{

	// KVector2 Template
	template <typename T>
	class KVector2{
	public:
		T x, y;

		explicit KVector2(T X = 0, T Y = 0) :
			x(X), y(Y)
		{}

		inline KVector2<T> operator-(const KVector2<T>& right) {
			return KVector2<T>(-right.x, -right.y);
		}

		inline KVector2<T> operator-(const KVector2<T>& right) const {
			return KVector2<T>(x - right.x, y - right.y);
		}

		inline KVector2<T> operator+(const KVector2<T>& right) {
			return KVector2<T>(x + right.x, y + right.y);
		}

		inline KVector2<T> operator+(T right) {
			return KVector2<T>(x + right, y + right);
		}

		inline KVector2<T> &operator+=(const KVector2<T>& right) {
			x += right.x;
			y += right.y;
			return *this;
		}

		inline KVector2<T> &operator+=(T right) {
			x += right;
			y += right;
			return *this;
		}

		inline KVector2<T> &operator-=(const KVector2<T>& right) {
			x -= right.x;
			y -= right.y;
			return *this;
		}

		inline KVector2<T> operator*(T right) {
			return KVector2<T>(x * right, y * right);
		}

		inline KVector2<T> operator*(const KVector2<T>& right) {
			return KVector2<T>(right.x * x, right.y * y);
		}

		inline KVector2<T>& operator*=(T right) {
			x *= right;
			y *= right;
			return *this;
		}

		inline KVector2<T>& operator*=(const KVector2<T>& right) {
			x *= right.x;
			y *= right.y;
			return *this;
		}

		inline KVector2<T> operator/(T right) {
			return KVector2<T>(x / right, y / right);
		}

		inline KVector2<T>& operator/=(T right) {
			x /= right;
			y /= right;

			return *this;
		}

		inline KVector2<T>& operator/=(const KVector2<T>& right) {
			x /= right.x;
			y /= right.y;

			return *this;
		}

		inline bool operator==(const KVector2<T>& right) {
			return (x == right.x) && (y == right.y);
		}

		inline bool operator!=(const KVector2<T>& right) {
			return (x != right.x) || (y != right.y);
		}

		/// distance only coded for 2D
		static inline T distance(const KVector2<T> &v1, const KVector2<T> &v2) {
			return sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
		}

		/// Vector3 length is distance from the origin
		static inline T length(const KVector2<T> &v) {
			return sqrt(v.x*v.x + v.y*v.y);
		}

		/// dot/scalar product: difference between two directions
		static inline T dotProduct(const KVector2<T> &v1, const KVector2<T> &v2) {
			return (v1.x*v2.x + v1.y*v2.y);
		}

		static inline T perpDot(const KVector2<T> &v1, const KVector2<T> &v2) {
			return (v1.y * v2.x) - (v1.x * v2.y);
		}

		/// calculate normal angle of the Vector
		static inline KVector2<T> normal(const KVector2<T> &v) {
			KD_ASSERT((length(v) == 0));
			T len = 1 / length(v);
			return KVector2<T>((v.x * len), (v.y * len));
		}

		static inline void move(KVector2<T>& v, T mx, T my) { v.x += mx; v.y += my; }
	}; 

	/// matrix 3x3 by 2x1 !
	/// shrinked for 2D purpose
	/*template <typename T>
	static inline KVector2<T> operator*(const KVector2<T> left, const T *right){
		return KVector2<T>(left.x * right[0] + left.y * right[1] + 1 * right[2],
			left.x * right[3] + left.y * right[4] + 1 * right[5]);
	}*/

	typedef KVector2<U8>  KVector2U8;
	typedef KVector2<U16> KVector2U16;
	typedef KVector2<U32> KVector2U32;
	typedef KVector2<U64> KVector2U64;
	typedef KVector2<I8>  KVector2I8;
	typedef KVector2<I16> KVector2I16;
	typedef KVector2<I32> KVector2I32;
	typedef KVector2<I64> KVector2I64;
	typedef KVector2<F32> KVector2F32;
	typedef KVector2<F64> KVector2F64;

	// KRect Template
	template <typename T>
	struct KRect{
		T left, right, bottom, top;

		KRect(T Left = 0, T Right = 0, T Bottom = 0, T Top = 0) :
			left(Left), right(Right), bottom(Bottom), top(Top)
		{}

		inline KRect<T> operator+(const KVector2<T>& right) {
			return KRect<T>(left + right.x, right + right.x,
							bottom + right.y, top + right.y);
		}

		inline KRect<T> operator-(const KVector2<T>& right) {
			return KRect<T>(left - right.x, right - right.x,
							bottom - right.y, top - right.y);
		}
	};

	typedef KRect<U8>  KRectU8;
	typedef KRect<U16> KRectU16;
	typedef KRect<U32> KRectU32;
	typedef KRect<I8>  KRectI8;
	typedef KRect<I16> KRectI16;
	typedef KRect<I32> KRectI32;
	typedef KRect<F32> KRectF32;
	typedef KRect<F64> KRectF64;


	// KRect2 Template
	template <typename T>
	struct KRect2{
		KVector2<T> leftBottom, leftTop, rightBottom, rightTop;

		KRect2() :
			leftBottom(0, 0), leftTop(0, 0), rightBottom(0, 0), rightTop(0, 0)
		{}

		KRect2(KVector2<T> LeftBottom, KVector2<T> LeftTop, KVector2<T> RightBottom, KVector2<T> RightTop) :
			leftBottom(LeftBottom), leftTop(LeftTop), rightBottom(RightBottom), rightTop(RightTop)
		{}

		inline KRect2<T> operator+(const KVector2<T>& right) {
			return KRect2<T>(leftBottom + right, leftTop + right,
							 rightBottom + right, rightTop + right);
		}

		inline KRect2<T> operator-(const KVector2<T>& right) {
			return KRect2<T>(leftBottom - right, leftTop - right,
							 rightBottom - right, rightTop - right);
		}
	};

	typedef KRect2<U8>  KRect2U8;
	typedef KRect2<U16> KRect2U16;
	typedef KRect2<U32> KRect2U32;
	typedef KRect2<I8>  KRect2I8;
	typedef KRect2<I16> KRect2I16;
	typedef KRect2<I32> KRect2I32;
	typedef KRect2<F32> KRect2F32;
	typedef KRect2<F64> KRect2F64;
}

#endif // KMATHSTRUCTS_H