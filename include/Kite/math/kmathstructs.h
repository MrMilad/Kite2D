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
#include "Kite/meta/kmetadef.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/kbaseserial.h"
#include "luaintf\LuaIntf.h"
#include "kmathstructs.khgen.h"

namespace Kite{

	/// KVector2 Template
	template <typename T>
	KM_CLASS(POD)
	class KVector2{
		KM_TEM_PARAM(T);
		KM_TEM_DEF("KVector2F32", F32);
		KM_TEM_DEF("KVector2I32", I32);
		KM_TEM_DEF("KVector2U32", U32);
		KMETA_KVECTOR2_BODY();

	public:

		KM_VAR() T x;
		KM_VAR() T y;

		KM_CON(T, T)
		explicit KVector2(T X = 0, T Y = 0) :
			x(X), y(Y)
		{}

		inline KVector2<T> operator-(const KVector2<T>& right) {
			return KVector2<T>(-right.x, -right.y);
		}

		inline KVector2<T> operator-(const KVector2<T>& right) const {
			return KVector2<T>(x - right.x, y - right.y);
		}

		inline KVector2<T> operator+(const KVector2<T>& right) const {
			return KVector2<T>(x + right.x, y + right.y);
		}

		inline KVector2<T> operator+(T right) const {
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

		inline KVector2<T> operator*(const KVector2<T>& right) const {
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

		inline KVector2<T> operator/(const KVector2<T>& right) const {
			return KVector2<T>(x / right.x, y / right.y);
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

		inline bool operator==(const KVector2<T>& right) const {
			return (x == right.x) && (y == right.y);
		}

		inline bool operator!=(const KVector2<T>& right) const {
			return (x != right.x) || (y != right.y);
		}

		/// matrix 3x3 by 2x1 !
		/// shrinked for 2D purpose
		/*template <typename T>
		static inline KVector2<T> operator*(const KVector2<T> left, const T *right){
		return KVector2<T>(left.x * right[0] + left.y * right[1] + 1 * right[2],
		left.x * right[3] + left.y * right[4] + 1 * right[5]);
		}*/

		/// distance only coded for 2D
		KM_FUN()
		static inline T distance(const KVector2<T> &v1, const KVector2<T> &v2) {
			return (T)sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
		}

		/// Vector3 length is distance from the origin
		KM_FUN()
		static inline T length(const KVector2<T> &v) {
			return (T)sqrt(v.x*v.x + v.y*v.y);
		}

		/// dot/scalar product: difference between two directions
		KM_FUN()
		static inline T dotProduct(const KVector2<T> &v1, const KVector2<T> &v2) {
			return (v1.x*v2.x + v1.y*v2.y);
		}

		KM_FUN()
		static inline T perpDot(const KVector2<T> &v1, const KVector2<T> &v2) {
			return (v1.y * v2.x) - (v1.x * v2.y);
		}

		/// calculate normal angle of the Vector
		KM_FUN()
		static inline KVector2<T> normal(const KVector2<T> &v) {
			KD_ASSERT((length(v) == 0));
			T len = 1 / length(v);
			return KVector2<T>((v.x * len), (v.y * len));
		}

		KM_FUN()
		static inline void move(KVector2<T>& v, T mx, T my) { v.x += mx; v.y += my; }

		private:
			KM_PRO_GET("x", T, "value of x")
				inline T getX() const { return x; }

			KM_PRO_SET("x")
				inline void setX(T X) { x = X; }

			KM_PRO_GET("y", T, "value of y")
				inline T getY() const { return y; }

			KM_PRO_SET("y")
				inline void setY(T Y) { y = Y; }

			KM_OPE(KO_ADD)
				KVector2<T> luaAddOpr(const KVector2<T> &right) const { return operator+(right); }

			KM_OPE(KO_SUB)
				KVector2<T> luaSubOpr(const KVector2<T> &right) const { return operator-(right); }

			KM_OPE(KO_MUL)
				KVector2<T> luaMulOpr(const KVector2<T> &right) const { return operator*(right); }

			KM_OPE(KO_DIV)
				KVector2<T> luaDivOpr(const KVector2<T> &right) const { return operator/(right); }
	}; 

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
	KM_CLASS(POD)
	struct KRect{
		KM_TEM_PARAM(T);
		KM_TEM_DEF("KRectF32", F32);
		KM_TEM_DEF("KRectI32", I32);
		KM_TEM_DEF("KRectU32", U32);
		KMETA_KRECT_BODY();

		KM_VAR() T left;
		KM_VAR() T right;
		KM_VAR() T bottom;
		KM_VAR() T top;

		KM_CON(T, T, T, T)
		KRect(T Left = 0, T Right = 0, T Bottom = 0, T Top = 0) :
			left(Left), right(Right), bottom(Bottom), top(Top)
		{}

		inline KRect<T> operator+(const KVector2<T>& Right) const {
			return KRect<T>(left + Right.x, right + Right.x,
							bottom + Right.y, top + Right.y);
		}

		inline KRect<T> operator-(const KVector2<T>& Right) const {
			return KRect<T>(left - Right.x, right - Right.x,
							bottom - Right.y, top - Right.y);
		}

	private:
		KM_PRO_GET("left", T, "value of left")
			inline T getLeft() const { return left; }

		KM_PRO_SET("left")
			inline void setLeft(T Left) { left = Left; }

		KM_PRO_GET("right", T, "value of right")
			inline T getRight() const { return right; }

		KM_PRO_SET("right")
			inline void setRight(T Right) { right = Right; }

		KM_PRO_GET("bottom", T, "value of bottom")
			inline T getBottom() const { return bottom; }

		KM_PRO_SET("bottom")
			inline void setBottom(T Bottom) { bottom = Bottom; }

		KM_PRO_GET("top", T, "value of top")
			inline T getTop() const { return top; }

		KM_PRO_SET("top")
			inline void setTop(T Top) { top = Top; }

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
	KM_CLASS(POD)
	struct KRect2{
		KM_TEM_PARAM(T);
		KM_TEM_DEF("KRect2F32", F32);
		KM_TEM_DEF("KRect2I32", I32);
		KM_TEM_DEF("KRect2U32", U32);
		KMETA_KRECT2_BODY();

		KM_VAR() KVector2<T> leftBottom;
		KM_VAR() KVector2<T> leftTop;
		KM_VAR() KVector2<T> rightBottom;
		KM_VAR() KVector2<T> rightTop;

		KM_CON(KVector2<T>, KVector2<T>, KVector2<T>, KVector2<T>)
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

	private:
		KM_PRO_GET("leftBottom", KVector2<T>, "value of leftBottom")
			inline KVector2<T> getLeftBottom() const { return leftBottom; }

		KM_PRO_SET("leftBottom")
			inline void setLeftBottom(KVector2<T> LeftBottom) { leftBottom = LeftBottom; }

		KM_PRO_GET("rightBottom", KVector2<T>, "value of rightBottom")
			inline KVector2<T> getRightBottom() const { return rightBottom; }

		KM_PRO_SET("rightBottom")
			inline void setRightBottom(KVector2<T> RightBottom) { rightBottom = RightBottom; }

		KM_PRO_GET("leftTop", KVector2<T>, "value of leftTop")
			inline KVector2<T> getLeftTop() const { return leftTop; }

		KM_PRO_SET("leftTop")
			inline void setLeftTop(KVector2<T> LeftTop) { leftTop = LeftTop; }

		KM_PRO_GET("rightTop", KVector2<T>, "value of rightTop")
			inline KVector2<T> getRightTop() const { return rightTop; }

		KM_PRO_SET("rightTop")
			inline void setRightTop(KVector2<T> RightTop) { rightTop = RightTop; }
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