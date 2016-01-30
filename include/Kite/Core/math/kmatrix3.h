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
#ifndef KMATRIX3_H
#define KMATRIX3_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
    class KMatrix3{
		/*friend KBytesArray &operator<<(KBytesArray &Out, const KMatrix3 &Value) {
			Out << Value[0] << Value[1] << Value[2];
			Out << Value[3] << Value[4] << Value[5];
			Out << Value[6] << Value[7] << Value[8];
			return Out;
		}

		friend KBytesArray &operator>>(KBytesArray &In, KMatrix3 &Value) {
			In >> Value[0] >> Value[1] >> Value[2];
			In >> Value[3] >> Value[4] >> Value[5];
			In >> Value[6] >> Value[7] >> Value[8];
			return In;
		}*/
    public:
		KMatrix3(F32 E00 = 1.0f, F32 E01 = 0.0f, F32 E02 = 0.0f,
			F32 E10 = 0.0f, F32 E11 = 1.0f, F32 E12 = 0.0f,
			F32 E20 = 0.0f, F32 E21 = 0.0f, F32 E22 = 1.0f){

			_kmatrix[0] = E00; _kmatrix[1] = E01;  _kmatrix[2] = E02;
			_kmatrix[3] = E10; _kmatrix[4] = E11;  _kmatrix[5] = E12;
			_kmatrix[6] = E20; _kmatrix[7] = E21;  _kmatrix[8] = E22;

		}

        inline const F32 *getArray() const {return _kmatrix;}

		inline KMatrix3 operator+(const KMatrix3 &right) const{
			return KMatrix3(_kmatrix[0] + right._kmatrix[0], _kmatrix[1] + right._kmatrix[1], _kmatrix[2] + right._kmatrix[2],
							_kmatrix[3] + right._kmatrix[3], _kmatrix[4] + right._kmatrix[4], _kmatrix[5] + right._kmatrix[5],
							_kmatrix[6] + right._kmatrix[6], _kmatrix[7] + right._kmatrix[7], _kmatrix[8] + right._kmatrix[8]);
		}



		inline KMatrix3 operator-(const KMatrix3 &right) const
		{
			return KMatrix3(_kmatrix[0] - right._kmatrix[0], _kmatrix[1] - right._kmatrix[1], _kmatrix[2] - right._kmatrix[2],
							_kmatrix[3] - right._kmatrix[3], _kmatrix[4] - right._kmatrix[4], _kmatrix[5] - right._kmatrix[5],
							_kmatrix[6] - right._kmatrix[6], _kmatrix[7] - right._kmatrix[7], _kmatrix[8] - right._kmatrix[8]);
		}



		inline KMatrix3 &operator+=(const KMatrix3 &right)
		{
			_kmatrix[0] += right._kmatrix[0];  _kmatrix[1] += right._kmatrix[1];  _kmatrix[2] += right._kmatrix[2];
			_kmatrix[3] += right._kmatrix[3];  _kmatrix[4] += right._kmatrix[4];  _kmatrix[5] += right._kmatrix[5];
			_kmatrix[6] += right._kmatrix[6];  _kmatrix[7] += right._kmatrix[7];  _kmatrix[8] += right._kmatrix[8];
			return *this;
		}



		inline KMatrix3 &operator-=(const KMatrix3 &right)
		{
			_kmatrix[0] -= right._kmatrix[0];  _kmatrix[1] -= right._kmatrix[1];  _kmatrix[2] -= right._kmatrix[2];
			_kmatrix[3] -= right._kmatrix[3];  _kmatrix[4] -= right._kmatrix[4];  _kmatrix[5] -= right._kmatrix[5];
			_kmatrix[6] -= right._kmatrix[6];  _kmatrix[7] -= right._kmatrix[7];  _kmatrix[8] -= right._kmatrix[8];
			return *this;
		}

		inline KMatrix3 operator*(const KMatrix3& right) const
		{
			return KMatrix3(_kmatrix[0] * right._kmatrix[0] + _kmatrix[1] * right._kmatrix[3] + _kmatrix[2] * right._kmatrix[6],
				_kmatrix[0] * right._kmatrix[1] + _kmatrix[1] * right._kmatrix[4] + _kmatrix[2] * right._kmatrix[7],
				_kmatrix[0] * right._kmatrix[2] + _kmatrix[1] * right._kmatrix[5] + _kmatrix[2] * right._kmatrix[8],
				_kmatrix[3] * right._kmatrix[0] + _kmatrix[4] * right._kmatrix[3] + _kmatrix[5] * right._kmatrix[6],
				_kmatrix[3] * right._kmatrix[1] + _kmatrix[4] * right._kmatrix[4] + _kmatrix[5] * right._kmatrix[7],
				_kmatrix[3] * right._kmatrix[2] + _kmatrix[4] * right._kmatrix[5] + _kmatrix[5] * right._kmatrix[8],
				_kmatrix[6] * right._kmatrix[0] + _kmatrix[7] * right._kmatrix[3] + _kmatrix[8] * right._kmatrix[6],
				_kmatrix[6] * right._kmatrix[1] + _kmatrix[7] * right._kmatrix[4] + _kmatrix[8] * right._kmatrix[7],
				_kmatrix[6] * right._kmatrix[2] + _kmatrix[7] * right._kmatrix[5] + _kmatrix[8] * right._kmatrix[8]);
		}



        inline KMatrix3& KMatrix3::operator*=(const KMatrix3& right)
		{
            *this = *this * right;
			return *this;
		}



        inline bool KMatrix3::operator==(const KMatrix3& right) const
		{
            return (_kmatrix[0] == right._kmatrix[0]) && (_kmatrix[1] == right._kmatrix[1]) &&
                    (_kmatrix[2] == right._kmatrix[2]) && (_kmatrix[3] == right._kmatrix[3]) &&
                    (_kmatrix[4] == right._kmatrix[4]) && (_kmatrix[5] == right._kmatrix[5]) &&
                    (_kmatrix[6] == right._kmatrix[6]) && (_kmatrix[7] == right._kmatrix[7]) &&
                    (_kmatrix[8] == right._kmatrix[8]);
		}



        inline bool KMatrix3::operator!=(const KMatrix3& right) const
		{
            return (_kmatrix[0] != right._kmatrix[0]) || (_kmatrix[1] != right._kmatrix[1]) || (_kmatrix[2] != right._kmatrix[2]) ||
                (_kmatrix[3] != right._kmatrix[3]) || (_kmatrix[4] != right._kmatrix[4]) || (_kmatrix[5] != right._kmatrix[5]) ||
                (_kmatrix[6] != right._kmatrix[6]) || (_kmatrix[7] != right._kmatrix[7]) || (_kmatrix[8] != right._kmatrix[8]);
		}



        inline float KMatrix3::operator[](int index) const
		{
            return _kmatrix[index];
		}



        inline float& KMatrix3::operator[](int index)
		{
            return _kmatrix[index];
		}



        inline KMatrix3 operator-(const KMatrix3& right){
            return KMatrix3(-right._kmatrix[0], -right._kmatrix[1], -right._kmatrix[2], -right._kmatrix[3], -right._kmatrix[4],
                            -right._kmatrix[5], -right._kmatrix[6], -right._kmatrix[7], -right._kmatrix[8]);
		}

    private:
        F32 _kmatrix[9];
    };
}

#endif // KMATRIX3_H
