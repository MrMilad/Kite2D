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
#include "Kite/Core/kcoredef.h"

namespace Kite{

	// KVector2 Template
	template <typename T>
	class KVector2{
	public:
		explicit KVector2(T X = 0, T Y = 0) :
			x(X), y(Y)
		{}

		T x, y;
	}; //class

	template <typename T>
	static inline KVector2<T> operator-(const KVector2<T>& right){
		return KVector2<T>(-right.x, -right.y);
	}

	template <typename T>
	static inline KVector2<T>& operator+=(KVector2<T>& left, const KVector2<T>& right){
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	template <typename T>
	static inline KVector2<T>& operator+=(KVector2<T>& left, T right) {
		left.x += right;
		left.y += right;
		return left;
	}

	template <typename T>
	static inline KVector2<T>& operator-=(KVector2<T>& left, const KVector2<T>& right){
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	template <typename T>
	static inline KVector2<T> operator+(const KVector2<T>& left, const KVector2<T>& right){
		return KVector2<T>(left.x + right.x, left.y + right.y);
	}

	template <typename T>
	static inline KVector2<T> operator+(const KVector2<T>& left, T right) {
		return KVector2<T>(left.x + right, left.y + right);
	}

	template <typename T>
	static inline KVector2<T> operator-(const KVector2<T>& left, const KVector2<T>& right){
		return KVector2<T>(left.x - right.x, left.y - right.y);
	}

	template <typename T>
	static inline KVector2<T> operator*(const KVector2<T>& left, T right){
		return KVector2<T>(left.x * right, left.y * right);
	}

	template <typename T>
	static inline KVector2<T> operator*(T left, const KVector2<T>& right){
		return KVector2<T>(right.x * left, right.y * left);
	}

	template <typename T>
	static inline KVector2<T>& operator*=(KVector2<T>& left, T right){
		left.x *= right;
		left.y *= right;
		return left;
	}

	template <typename T>
	static inline KVector2<T>& operator*=(KVector2<T>& left, const KVector2<T>& right){
		left.x *= right.x;
		left.y *= right.y;
		return left;
	}

	/*
	/// matrix 3x3 by 2x1 !
	/// shrinked for 2D purpose
	template <typename T>
	static inline KVector2<T> operator*(const KVector2<T> left, const T *right){
		return KVector2<T>(left.x * right[0] + left.y * right[1] + 1 * right[2],
			left.x * right[3] + left.y * right[4] + 1 * right[5]);
	}*/


	template <typename T>
	static inline KVector2<T> operator/(const KVector2<T>& left, T right){
		return KVector2<T>(left.x / right, left.y / right);
	}

	template <typename T>
	static inline KVector2<T>& operator/=(KVector2<T>& left, T right){
		left.x /= right;
		left.y /= right;

		return left;
	}

	template <typename T>
	static inline KVector2<T>& operator/=(KVector2<T>& left, const KVector2<T>& right){
		left.x /= right.x;
		left.y /= right.y;

		return left;
	}

	template <typename T>
	static inline bool operator==(const KVector2<T>& left, const KVector2<T>& right){
		return (left.x == right.x) && (left.y == right.y);
	}

	template <typename T>
	static inline bool operator!=(const KVector2<T>& left, const KVector2<T>& right){
		return (left.x != right.x) || (left.y != right.y);
	}

	// distance only coded for 2D
	template <typename T>
	static inline T distance(const KVector2<T> &v1, const KVector2<T> &v2){
		return sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
	}

	// Vector3 length is distance from the origin
	template <typename T>
	static inline T length(const KVector2<T> &v){
		return sqrt(v.x*v.x + v.y*v.y);
	}

	// dot/scalar product: difference between two directions
	template <typename T>
	static inline T dotProduct(const KVector2<T> &v1, const KVector2<T> &v2){
		return (v1.x*v2.x + v1.y*v2.y);
	}

	template <typename T>
	static inline T perpDot(const KVector2<T> &v1, const KVector2<T> &v2){
		return (v1.y * v2.x) - (v1.x * v2.y);
	}

	// calculate normal angle of the Vector
	template <typename T>
	static inline KVector2<T> normal(const KVector2<T> &v){
		KDEBUG_ASSERT_T((length(v) == 0));
		T len = 1 / length(v);
		return KVector2<T>((v.x * len), (v.y * len));
	}

	template <typename T>
	static inline void move(KVector2<T>& v, T mx, T my) { v.x += mx; v.y += my; }

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


	// KVector3 Template
	template <typename T>
	class KVector3{
	public:
		explicit KVector3(T X = 0, T Y = 0, T Z = 0) :
			x(X), y(Y), z(Z)
		{}

		T x, y, z;
	};

	template <typename T>
	static inline KVector3<T> operator-(const KVector3<T>& right){
		return KVector3<T>(-right.x, -right.y);
	}

	template <typename T>
	static inline KVector3<T>& operator+=(KVector3<T>& left, const KVector3<T>& right){
		left.x += right.x;
		left.y += right.y;
		left.z += right.z;
		return left;
	}

	template <typename T>
	static inline KVector3<T>& operator-=(KVector3<T>& left, const KVector3<T>& right){
		left.x -= right.x;
		left.y -= right.y;
		left.z -= right.z;
		return left;
	}

	template <typename T>
	static inline KVector3<T> operator+(const KVector3<T>& left, const KVector3<T>& right){
		return KVector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	template <typename T>
	static inline KVector3<T> operator-(const KVector3<T>& left, const KVector3<T>& right){
		return KVector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	template <typename T>
	static inline KVector3<T> operator*(const KVector3<T>& left, T right){
		return KVector3<T>(left.x * right, left.y * right, left.z * right);
	}

	template <typename T>
	static inline KVector3<T> operator*(T left, const KVector3<T>& right){
		return KVector3<T>(right.x * left, right.y * left, right.z * left);
	}

	/// matrix 3x3 by 3x1
	/// shrinked for 2D purpose
	template <typename T>
	static inline KVector3<T> operator*(const KVector3<T> &left, const T *right){
		/*return KVector3<T>( left.x * right[0] + left.y * right[1] + left.z * right[2],
		left.x * right[3] + left.y * right[4] + left.z * right[5],
		left.x * right[6] + left.y * right[7] + left.z * right[8]);*/

		return KVector3<T>(left.x * right[0] + left.y * right[1] + left.z * right[2],
			left.x * right[3] + left.y * right[4] + left.z * right[5],
			1);
	}

	template <typename T>
	static inline KVector3<T>& operator*=(KVector3<T>& left, T right){
		left.x *= right;
		left.y *= right;
		left.z *= right;
		return left;
	}

	template <typename T>
	static inline KVector3<T>& operator*=(KVector3<T>& left, const KVector3<T>& right){
		left.x *= right.x;
		left.y *= right.y;
		left.z *= right.z;
		return left;
	}


	template <typename T>
	static inline KVector3<T> operator/(const KVector3<T>& left, T right){
		return KVector3<T>(left.x / right, left.y / right, left.z / right);
	}

	template <typename T>
	static inline KVector3<T>& operator/=(KVector3<T>& left, T right){
		left.x /= right;
		left.y /= right;
		left.z /= right;
		return left;
	}

	template <typename T>
	static inline KVector3<T>& operator/=(KVector3<T>& left, const KVector3<T>& right){
		left.x /= right.x;
		left.y /= right.y;
		left.z /= right.z;
		return left;
	}

	template <typename T>
	static inline bool operator==(const KVector3<T>& left, const KVector3<T>& right){
		return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
	}

	template <typename T>
	static inline bool operator!=(const KVector3<T>& left, const KVector3<T>& right){
		return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
	}

	template <typename T>
	static inline void move(KVector3<T>& v, T mx, T my, T mz) { v.x += mx; v.y += my; v.z += mz; }

	typedef KVector3<U8>  KVector3U8;
	typedef KVector3<U16> KVector3U16;
	typedef KVector3<U32> KVector3U32;
	typedef KVector3<I8>  KVector3I8;
	typedef KVector3<I16> KVector3I16;
	typedef KVector3<I32> KVector3I32;
	typedef KVector3<F32> KVector3F32;
	typedef KVector3<F64> KVector3F64;

	// KRect Template
	template <typename T>
	struct KRect{
		T left, right, bottom, top;

		KRect(T Left = 0, T Right = 0, T Bottom = 0, T Top = 0) :
			left(Left), right(Right), bottom(Bottom), top(Top)
		{}
	};

	template <typename T>
	inline KRect<T> operator+(const KRect<T>& left, const KVector2<T>& right){
		return KRect<T>(left.left + right.x, left.right + right.x,
			left.bottom + right.y, left.top + right.y);
	}

	template <typename T>
	inline KRect<T> operator-(const KRect<T>& left, const KVector2<T>& right){
		return KRect<T>(left.left - right.x, left.right - right.x,
			left.bottom - right.y, left.top - right.y);
	}

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
	};

	template <typename T>
	inline KRect2<T> operator+(const KRect2<T>& left, const KVector2<T>& right){
		return KRect2<T>(left.leftBottom + right, left.leftTop + right,
			left.rightBottom + right, left.rightTop + right);
	}

	template <typename T>
	inline KRect2<T> operator-(const KRect2<T>& left, const KVector2<T>& right){
		return KRect2<T>(left.leftBottom - right, left.leftTop - right,
			left.rightBottom - right, left.rightTop - right);
	}

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