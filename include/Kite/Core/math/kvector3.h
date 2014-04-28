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
#ifndef KVECTOR3_H
#define KVECTOR3_H

#include "Kite/core/system/ksystemdef.h"

namespace Kite {
    template <typename T>
    class KVector3{
        public:
            explicit KVector3(T X = 0, T Y = 0, T Z = 0):
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
    static inline KVector3<T> operator*(const KVector3<T>& left,T right){
        return KVector3<T>(left.x * right, left.y * right, left.z * right);
    }

    template <typename T>
    static inline KVector3<T> operator*(T left, const KVector3<T>& right){
        return KVector3<T>(right.x * left, right.y * left, right.z * left);
    }

    /// matrix 3x3 by 3x1
	/// shrinked for 2D purpose
    template <typename T>
    static inline KVector3<T> operator*(const KVector3<T> left, const T *right){
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
    static inline KVector3<T>& operator/=(KVector3<T>& left,  const KVector3<T>& right){
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
    static inline void move(KVector3<T>& v, T mx,T my, T mz) {v.x += mx; v.y += my; v.z += mz;}

    typedef KVector3<U8>  KVector3U8;
    typedef KVector3<U16> KVector3U16;
    typedef KVector3<U32> KVector3U32;
    typedef KVector3<I8>  KVector3I8;
    typedef KVector3<I16> KVector3I16;
    typedef KVector3<I32> KVector3I32;
    typedef KVector3<F32> KVector3F32;
    typedef KVector3<F64> KVector3F64;

} //namespace
#endif // KVECTOR3_H
