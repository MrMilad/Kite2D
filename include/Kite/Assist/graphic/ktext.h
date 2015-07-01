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
#ifndef KTEXT_H
#define KTEXT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Assist/graphic/kindexbatchobject.h"
#include "Kite/Core/Math/ktransformable.h"
#include "kite/Assist/graphic/kanimeobject.h"
#include <cstring>

namespace Kite{
    class KITE_FUNC_EXPORT KText : public KIndexBatchObject, public KTransformable, public KAnimeObject{
	public:
		KText(U32 MaxSize);
		KText(const std::string &Text, const std::vector<KAtlas> &Font, const KColor &Color);

		void setFont(const std::vector<KAtlas> &Font);
        inline const std::vector<KAtlas> &getFont() const {return *_kfont;}

		void setText(const std::string &Text);
        inline const std::string &getText() const {return _ktext;}

		void setMiddleSpace(U32 Size);
		inline U32 getMiddleSpace() const { return _kmid; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

		inline F32 getWidth() const { return _kwidth; }

	protected:
		// from KIndexBatchObject (output/send)
		const KTransform &getModelViewTransform() const;

		// from KAnimeObject (input/recive)
		void animeUpdate(const KAnimeValue *AnimatedValue);

    private:
		void _fillIndex();
		void _reshape();
        const std::vector<KAtlas> *_kfont;
        std::string _ktext;
		F32 _kwidth;
		U32 _ksize;
		U32 _kmid;
		KColor _kcolor;
    };
}

#endif // KTEXT_H
