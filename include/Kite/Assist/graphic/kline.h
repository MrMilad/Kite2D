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
#ifndef KLINE_H
#define KLINE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Assist/graphic/kbatchobject.h"
#include "Kite/Assist/graphic/ktransformable.h"

namespace Kite{
    class KITE_FUNC_EXPORT KLine :public KTransformable, public KBatchObject{
    public:
        KLine();
        KLine(const KVector2F32 &Start, const KVector2F32 &End, const KColor &Color);
        ~KLine();

		inline void setStart(const KVector2F32 &Start) { _kstart = Start; }
		inline const KVector2F32 &getStart() const { return _kstart; }

		inline void setEnd(const KVector2F32 &End) { _kend = End; }
		inline const KVector2F32 &getEnd() const { return _kend; }

		inline void setColor(const KColor &color) { _kcolor = color; }
		inline const KColor &getColor() const { return _kcolor; }

		const KVertex *getVertex() const;

	private:
		KVector2F32 _kstart;
		KVector2F32 _kend;
		KColor _kcolor;
    };
}

#endif // KLINE_H
