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
#ifndef KVECTOR2_H
#define KVECTOR2_H

#include <cmath>
#include "Kite/core/system/ksystemdef.h"

namespace Kite {
	template <typename T>
    class KVector2{
        public:
            explicit KVector2(T X = 0, T Y = 0):
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
    static inline KVector2<T> operator-(const KVector2<T>& left, const KVector2<T>& right){
        return KVector2<T>(left.x - right.x, left.y - right.y);
    }

    template <typename T>
    static inline KVector2<T> operator*(const KVector2<T>& left,T right){
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

    /// matrix 3x3 by 2x1 !
    /// shrinked for 2D purpose
    template <typename T>
    static inline KVector2<T> operator*(const KVector2<T> left, const T *right){
        return KVector2<T>(left.x * right[0] + left.y * right[1] + 1 * right[2],
                            left.x * right[3] + left.y * right[4] + 1 * right[5]);
    }


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
    static inline KVector2<T>& operator/=(KVector2<T>& left,  const KVector2<T>& right){
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
        return sqrt((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y));
    }

    // Vector3 length is distance from the origin
    template <typename T>
    static inline T length(const KVector2<T> &v){
        return sqrt(v.x*v.x + v.y*v.y);
    }

    // dot/scalar product: difference between two directions
    template <typename T>
    static inline T dotProduct( const KVector2<T> &v1 , const KVector2<T> &v2){
    return (v1.x*v2.x + v1.y*v2.y);
    }

    // calculate normal angle of the Vector
    template <typename T>
    static inline KVector2<T> normal(const KVector2<T> &v){
        KDEBUG_ASSERT_T((length(v) == 0));
        T len = 1 / length(v);
        return KVector2<T>((v.x * len), (v.y * len));
    }

    template <typename T>
    static inline void move(KVector2<T>& v, T mx,T my) {v.x += mx; v.y += my;}

    typedef KVector2<U8>  KVector2U8;
    typedef KVector2<U16> KVector2U16;
    typedef KVector2<U32> KVector2U32;
	typedef KVector2<UL32> KVector2UL32;
	typedef KVector2<U64> KVector2U64;
    typedef KVector2<I8>  KVector2I8;
    typedef KVector2<I16> KVector2I16;
    typedef KVector2<I32> KVector2I32;
	typedef KVector2<IL32> KVector2IL32;
	typedef KVector2<I64> KVector2I64;
    typedef KVector2<F32> KVector2F32;
    typedef KVector2<F64> KVector2F64;

} //namespace 
#endif // KVECTOR_H

