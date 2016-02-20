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
#ifndef KRANDOM_H
#define KRANDOM_H

/*! \file krandom.h */

#include "Kite/Core/system/ksystemdef.h"

namespace Kite {
	class KITE_FUNC_EXPORT KRandom {
	public:
		KRandom() :
			_kseed(1),
			_kmin(0),
			_klen(100)
		{}

		KRandom(U32 Seed) :
			_kseed(Seed),
			_kmin(0),
			_klen(100) 
		{}

		KRandom(U32 Seed, U32 Minimum, U32 Lenght) :
			_kseed(Seed),
			_kmin(Minimum),
			_klen(Lenght)
		{}

		inline void setSeed(U32 Seed) { _kseed = Seed; }

		inline void setRange(U32 Minimum, U32 Lenght) { _kmin = Minimum, _klen = Lenght + 1; }

		inline U32 getRandomInt() { return _getRnd() % _klen + _kmin; }

	private:
		inline U32 _getRnd() { _kseed = (214013 * _kseed + 2531011); return (_kseed >> 16) & 0x7FFF; }
		U32 _kseed;
		U32 _kmin;
		U32 _klen;
	};
}

#endif // KRANDOM_H