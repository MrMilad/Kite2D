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
#include "Kite/math/kmatrix3.h"

namespace Kite {
	KMatrix3::KMatrix3(F32 E00, F32 E01, F32 E02,
			 F32 E10, F32 E11, F32 E12 ,
			 F32 E20, F32 E21, F32 E22) {

		_kmatrix[0] = E00; _kmatrix[1] = E01;  _kmatrix[2] = E02;
		_kmatrix[3] = E10; _kmatrix[4] = E11;  _kmatrix[5] = E12;
		_kmatrix[6] = E20; _kmatrix[7] = E21;  _kmatrix[8] = E22;

	}

	KMatrix3 KMatrix3::operator+(const KMatrix3 &right) const {
		return KMatrix3(_kmatrix[0] + right._kmatrix[0], _kmatrix[1] + right._kmatrix[1], _kmatrix[2] + right._kmatrix[2],
						_kmatrix[3] + right._kmatrix[3], _kmatrix[4] + right._kmatrix[4], _kmatrix[5] + right._kmatrix[5],
						_kmatrix[6] + right._kmatrix[6], _kmatrix[7] + right._kmatrix[7], _kmatrix[8] + right._kmatrix[8]);
	}

	KMatrix3 KMatrix3::operator-(const KMatrix3 &right) const {
		return KMatrix3(_kmatrix[0] - right._kmatrix[0], _kmatrix[1] - right._kmatrix[1], _kmatrix[2] - right._kmatrix[2],
						_kmatrix[3] - right._kmatrix[3], _kmatrix[4] - right._kmatrix[4], _kmatrix[5] - right._kmatrix[5],
						_kmatrix[6] - right._kmatrix[6], _kmatrix[7] - right._kmatrix[7], _kmatrix[8] - right._kmatrix[8]);
	}

	KMatrix3 &KMatrix3::operator+=(const KMatrix3 &right) {
		_kmatrix[0] += right._kmatrix[0];  _kmatrix[1] += right._kmatrix[1];  _kmatrix[2] += right._kmatrix[2];
		_kmatrix[3] += right._kmatrix[3];  _kmatrix[4] += right._kmatrix[4];  _kmatrix[5] += right._kmatrix[5];
		_kmatrix[6] += right._kmatrix[6];  _kmatrix[7] += right._kmatrix[7];  _kmatrix[8] += right._kmatrix[8];
		return *this;
	}

	KMatrix3 &KMatrix3::operator-=(const KMatrix3 &right) {
		_kmatrix[0] -= right._kmatrix[0];  _kmatrix[1] -= right._kmatrix[1];  _kmatrix[2] -= right._kmatrix[2];
		_kmatrix[3] -= right._kmatrix[3];  _kmatrix[4] -= right._kmatrix[4];  _kmatrix[5] -= right._kmatrix[5];
		_kmatrix[6] -= right._kmatrix[6];  _kmatrix[7] -= right._kmatrix[7];  _kmatrix[8] -= right._kmatrix[8];
		return *this;
	}

	KMatrix3 KMatrix3::operator*(const KMatrix3& right) const {
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

	KMatrix3& KMatrix3::operator*=(const KMatrix3& right) {
		*this = *this * right;
		return *this;
	}

	bool KMatrix3::operator==(const KMatrix3& right) const {
		return (_kmatrix[0] == right._kmatrix[0]) && (_kmatrix[1] == right._kmatrix[1]) &&
			(_kmatrix[2] == right._kmatrix[2]) && (_kmatrix[3] == right._kmatrix[3]) &&
			(_kmatrix[4] == right._kmatrix[4]) && (_kmatrix[5] == right._kmatrix[5]) &&
			(_kmatrix[6] == right._kmatrix[6]) && (_kmatrix[7] == right._kmatrix[7]) &&
			(_kmatrix[8] == right._kmatrix[8]);
	}

	bool KMatrix3::operator!=(const KMatrix3& right) const {
		return (_kmatrix[0] != right._kmatrix[0]) || (_kmatrix[1] != right._kmatrix[1]) || (_kmatrix[2] != right._kmatrix[2]) ||
			(_kmatrix[3] != right._kmatrix[3]) || (_kmatrix[4] != right._kmatrix[4]) || (_kmatrix[5] != right._kmatrix[5]) ||
			(_kmatrix[6] != right._kmatrix[6]) || (_kmatrix[7] != right._kmatrix[7]) || (_kmatrix[8] != right._kmatrix[8]);
	}

	KMatrix3 KMatrix3::operator-(const KMatrix3& right) {
		return KMatrix3(-right._kmatrix[0], -right._kmatrix[1], -right._kmatrix[2], -right._kmatrix[3], -right._kmatrix[4],
						-right._kmatrix[5], -right._kmatrix[6], -right._kmatrix[7], -right._kmatrix[8]);
	}
}