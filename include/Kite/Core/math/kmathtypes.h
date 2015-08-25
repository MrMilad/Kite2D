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
#ifndef KMATHTYPES_H
#define KMATHTYPES_H

#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
	enum KOrientationTypes : I8{
		KO_COLLINEAR = 0,
		KO_CLOCKWISE = -1,
		KO_CCLOCKWISE = 1
	};

	enum KInterpolationTypes : U8{
		KIN_LINEAR = 0,
		KIN_QUADRATICIN,
		KIN_QUADRATICOUT,
		KIN_QUADRATICINOUT,
		KIN_SINUSOIDALIN,
		KIN_SINUSOIDALOUT,
		KIN_SINUSOIDALINOUT,
		KIN_EXPONENTIALIN,
		KIN_EXPONENTIALOUT,
		KIN_EXPONENTIALINOUT
	};
}


#endif // KMATHTYPES_H