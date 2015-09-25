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
#include "Kite/Core/Math/ktransformable.h"
#include "Kite/Core/graphic/kbatchobject.h"
#include "Kite/Core/graphic/katlas.h"
#include <cstring>

namespace Kite{
    class KITE_FUNC_EXPORT KText : public KBatchObject{
	public:
		// default max size = 100
		KText(const KAtlas &Font);

		// custom max size
		KText(U32 MaximumSize, const KAtlas &Font);

		// max size = Text size
		KText(const std::string &Text, const KAtlas &Font, const KColor &Color);

		void setFont(const KAtlas &Font);
        inline const KAtlas &getFont() const {return *_kfont;}

		void setText(const std::string &Text);
        inline const std::string &getText() const {return _ktext;}

		// 100 by default
		void setMaximumSize(U32 Size);
		inline U32 getMaximumSize() const { return _kmaxSize; }
		
		// 30 by default
		inline void setLineSpace(U32 Size) { _kpgraph = Size; }
		inline U32 getLineSpace() const { return _kpgraph; }

		// 0 by default
		void setMiddleSpace(U32 Size);
		inline U32 getMiddleSpace() const { return _kmid; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

		inline F32 getWidth() const { return _kwidth; }
		inline F32 getHeight() const { return _kheight; }

		// from KTileMapObject (output/send)
		const KVector2F32 *getTilePosition() const;

		// from KIndexBatchObject (output/send)
		const KTransform &getModelViewTransform() const;

	protected:

		// from KAnimeObject (input/recive)
		void animeUpdate(const KAnimeValue *AnimatedValue);

    private:
		void _fillIndex();
		void _reshape();
        const KAtlas *_kfont;
        std::string _ktext;
		F32 _kwidth;
		F32 _kheight;
		U32 _kmaxSize;
		U32 _kmid;
		U32 _kpgraph;
		KColor _kcolor;
    };
}

#endif // KTEXT_H
