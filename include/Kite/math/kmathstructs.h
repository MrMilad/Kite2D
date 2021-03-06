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
		KM_TEM_DEF("Vector2f", F32);
		KM_TEM_DEF("Vector2i", I32);
		KM_TEM_DEF("Vector2u", U32);
		KVECTOR2_BODY();

	public:

		KM_VAR() T x;
		KM_VAR() T y;

		KM_CON(T, T)
		explicit KVector2(T X = 0, T Y = 0) :
			x(X), y(Y)
		{}

		inline KVector2<T> operator-() {
			return KVector2<T>(-x, -y);
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

		inline bool operator==(const KVector2<T> &right) const {
			return (x == right.x) && (y == right.y);
		}

		inline bool operator!=(const KVector2<T> &right) const {
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
		inline T distance(const KVector2<T> &v2) const {
			return (T)sqrt((v2.x - x)*(v2.x - x) + (v2.y - y)*(v2.y - y));
		}

		/// Vector3 length is distance from the origin
		KM_PRO_GET(KP_NAME = "length", KP_TYPE = T)
		inline T length() const {
			return (T)sqrt(x*x + y*y);
		}

		/// dot/scalar product: difference between two directions
		KM_FUN()
		inline T dotProduct(const KVector2<T> &v2) const {
			return (x*v2.x + y*v2.y);
		}

		KM_FUN()
		inline T perpDot(const KVector2<T> &v2) const {
			return (y * v2.x) - (x * v2.y);
		}

		/// calculate normal angle of the Vector
		KM_PRO_GET(KP_NAME = "length", KP_TYPE = T)
		inline KVector2<T> normal() const{
			KD_ASSERT((length() == 0));
			T len = 1 / length();
			return KVector2<T>((x * len), (y * len));
		}

		KM_FUN()
		inline void move(T mx, T my) { x += mx; y += my; }

		private:
			KM_OPE(KO_ADD)
				KVector2<T> luaAddOpr(const KVector2<T> &right) const { return operator+(right); }

			KM_OPE(KO_SUB)
				KVector2<T> luaSubOpr(const KVector2<T> &right) const { return operator-(right); }

			KM_OPE(KO_MUL)
				KVector2<T> luaMulOpr(const KVector2<T> &right) const { return operator*(right); }

			KM_OPE(KO_DIV)
				KVector2<T> luaDivOpr(const KVector2<T> &right) const { return operator/(right); }

			KM_OPE(KO_EQ)
				bool luaEqOpr(const KVector2<T> &right) const { return operator==(right); }
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
		KM_TEM_DEF("Rectf", F32);
		KM_TEM_DEF("Recti", I32);
		KM_TEM_DEF("Rectu", U32);
		KRECT_BODY();

		KM_VAR() T left;
		KM_VAR() T right;
		KM_VAR() T bottom;
		KM_VAR() T top;

		KM_CON(T, T, T, T)
		KRect(T Left = 0, T Right = 0, T Bottom = 0, T Top = 0) :
			left(Left), right(Right), bottom(Bottom), top(Top)
		{}

		KM_FUN()
		inline void correctSides() {
			if (left > right) { T temp = left; left = right; right = temp; }
			if (bottom > top) { T temp = bottom; bottom = top; top = temp; }
		}

		KM_FUN()
		bool continePoint(const KVector2<T> &HitPoint) const{
			if (HitPoint.x < left) return false;
			if (HitPoint.y < bottom) return false;
			if (HitPoint.x > right) return false;
			if (HitPoint.y > top) return false;
			return true;
		}

		KM_FUN()
		inline bool contineRect(const KRect<T> &Rect) const {
			if ((Rect.left >= left) && (Rect.right <= right)) {
				if ((Rect.bottom >= bottom) && (Rect.top <= top)) {
					return true;
				}
			}
			return false;
		}


		KM_FUN()
		bool overlapRect(const KRect<T> &Rect) const {
			if (valueInRange(left, Rect.left, Rect.right) || valueInRange(Rect.left, left, right)) {
				if (valueInRange(bottom, Rect.bottom, Rect.top) || valueInRange(Rect.bottom, bottom, top)) {
					return true;
				}
			}
			return false;;
		}

		KM_FUN()
		bool intersectLine(const KVector2F32 &LineStart, const KVector2F32 &LineEnd) const {
			if (lineIntersect(LineStart, LineEnd, KVector2F32((F32)left, (F32)bottom), KVector2F32((F32)right, (F32)bottom))) return true;
			if (lineIntersect(LineStart, LineEnd, KVector2F32((F32)left, (F32)top), KVector2F32((F32)right, (F32)top))) return true;
			if (lineIntersect(LineStart, LineEnd, KVector2F32((F32)left, (F32)bottom), KVector2F32((F32)left, (F32)top))) return true;
			if (lineIntersect(LineStart, LineEnd, KVector2F32((F32)right, (F32)bottom), KVector2F32((F32)right, (F32)top))) return true;
			return false;
		}

		KM_FUN()
		bool intersectCircle(const KVector2F32 &Center, float Radius) const {
			float testX = Center.x;
			float testY = Center.y;
			const float width = (F32)right - (F32)left;
			const float height = (F32)top - (F32)bottom;

			if (testX < left)
				testX = left;
			if (testX >(left + width))
				testX = (left + width);
			if (testY < bottom)
				testY = bottom;
			if (testY >(bottom + height))
				testY = (bottom + height);

			return ((Center.x - testX) * (Center.x - testX) + (Center.y - testY) * (Center.y - testY)) < Radius * Radius;
		}

		inline void operator+=(const KVector2<T>& Right) {
			left += Right.x;
			right += Right.x;
			bottom += Right.y;
			top += Right.y;
		}

		inline void operator-=(const KVector2<T>& Right) {
			left -= Right.x;
			right -= Right.x;
			bottom -= Right.y;
			top -= Right.y;
		}

		inline KRect<T> operator+(const KVector2<T>& Right) const {
			return KRect<T>(left + Right.x, right + Right.x,
							bottom + Right.y, top + Right.y);
		}

		inline KRect<T> operator-(const KVector2<T>& Right) const {
			return KRect<T>(left - Right.x, right - Right.x,
							bottom - Right.y, top - Right.y);
		}

		inline bool operator==(const KRect<T>& Right) const {
			return (left == Right.left && right == Right.right
					&& bottom == Right.bottom && top == Right.top);
		}

		inline bool operator!=(const KRect<T>& Right) const {
			return (left != Right.left || right != Right.right
					|| bottom != Right.bottom || top != Right.top);
		}

	private:
		bool lineIntersect(const KVector2F32 &Line1Start, const KVector2F32 &Line1End,
									   const KVector2F32 &Line2Start, const KVector2F32 &Line2End,
									   KVector2F32 *OutPoint = nullptr) {
			float s1_x, s1_y, s2_x, s2_y;
			s1_x = Line1End.x - Line1Start.x;     s1_y = Line1End.y - Line1Start.y;
			s2_x = Line2End.x - Line2Start.x;     s2_y = Line2End.y - Line2Start.y;

			float s, t;
			s = (-s1_y * (Line1Start.x - Line2Start.x) + s1_x * (Line1Start.y - Line2Start.y)) / (-s2_x * s1_y + s1_x * s2_y);
			t = (s2_x * (Line1Start.y - Line2Start.y) - s2_y * (Line1Start.x - Line2Start.x)) / (-s2_x * s1_y + s1_x * s2_y);

			if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
				// Collision detected
				if (OutPoint != nullptr) {
					OutPoint->x = Line1Start.x + (t * s1_x);
					OutPoint->y = Line1Start.y + (t * s1_y);
				}
				return true;
			}

			return false; // No collision
		}

		inline bool valueInRange(T Value, T Min, T Max) const {
			return (Value >= Min) && (Value <= Max);
		}

		KM_OPE(KO_ADD)
			inline KRect<T> luaAddOpr(const KVector2<T> &right) const { return operator+(right); }

		KM_OPE(KO_SUB)
			inline KRect<T> luaSubOpr(const KVector2<T> &right) const { return operator-(right); }
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
		KM_TEM_DEF("Rect2f", F32);
		KM_TEM_DEF("Rect2i", I32);
		KM_TEM_DEF("Rect2u", U32);
		KRECT2_BODY();

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

		KM_FUN()
		bool continePoint(const KVector2F32 &Point) {
			U32 i, j;
			KVector2F32 Points[4];
			bool c = false;
			Points[0] = Box.leftBottom;
			Points[1] = Box.leftTop;
			Points[2] = Box.rightTop;
			Points[3] = Box.rightBottom;

			for (i = 0, j = 4 - 1; i < 4; j = i++) {
				if (((Points[i].y > HitPoint.y) != (Points[j].y > HitPoint.y)) &&
					(HitPoint.x < (Points[j].x - Points[i].x) * (HitPoint.y - Points[i].y) / (Points[j].y - Points[i].y) + Points[i].x))
					c = !c;
			}
			return c;
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

	KM_CLASS(POD)
	struct KParallaxRatio {
		KM_INFO(KI_NAME = "ParallaxRatio");
		KPARALLAXRATIO_BODY();

		KM_VAR() KVector2F32 center;
		KM_VAR() F32 zoom;
		KM_VAR() F32 rotation;

		KParallaxRatio(KVector2F32 Center , F32 Zoom, F32 Rotation) :
			center(Center), zoom(Zoom), rotation(Rotation) {}

		KM_CON()
			KParallaxRatio() :
			center(1,1), zoom(1), rotation(1) {} // normal ratio

		bool operator==(const KParallaxRatio &Right) {
			if (center != Right.center) return false;
			if (zoom != Right.zoom) return false;
			if (rotation != Right.rotation) return false;
			return true;
		}
	};

	KM_CLASS(POD)
	struct KParallaxIndex {
		KM_INFO(KI_NAME = "ParallaxIndex");
		KPARALLAXINDEX_BODY();

		KM_VAR() bool enable;
		KM_VAR() U8 index;

		KM_CON(bool, U8)
		KParallaxIndex(bool Enable = false, U8 Index = 0) :
			enable(Enable), index(Index) {}
	};
}

#endif // KMATHSTRUCTS_H