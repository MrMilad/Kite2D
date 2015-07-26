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
#ifndef KLINE_H
#define KLINE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Assist/graphic/karraybatchobject.h"
#include "Kite/Core/math/ktransformable.h"
#include "kite/Assist/graphic/kanimeobject.h"

namespace Kite{
    class KITE_FUNC_EXPORT KLine : public KArrayBatchObject, public KTransformable, public KAnimeObject{
    public:
        KLine();
        KLine(const KVector2F32 &Start, const KVector2F32 &End, const KColor &Color);

		void setStart(const KVector2F32 &Start);
		inline const KVector2F32 &getStart() const { return _kstart; }

		void setEnd(const KVector2F32 &End);
		inline const KVector2F32 &getEnd() const { return _kend; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

	protected:
		// from KBatchObject (output/send)
		const KTransform &getModelViewTransform() const;

		// from KAnimeObject (input/recive)
		void animeUpdate(const KAnimeValue *AnimatedValue);

	private:
		KVector2F32 _kstart;
		KVector2F32 _kend;
		KColor _kcolor;
    };
}

#endif // KLINE_H
