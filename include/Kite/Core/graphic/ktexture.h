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
#ifndef KTEXTURE_H
#define KTEXTURE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/system/kvector2.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kimage.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTexture : public KNonCopyable{
    public:
        KTexture();
        ~KTexture();

        /// create texture and load image pixels
        void create(const KImage &Image, KTextureFilterTypes Filter, KTextureWrapTypes Wrap);

        /// update whole or piece of texture with image
        /// image size + position must equal or smaller than texture size
        void update(const KImage &Image, U32 XPos, U32 YPos);

        /// bind the texture and ready for render
        /// (handle autimatic by internal render system)
        void bind() const;

        static void unbind();

        inline KTextureFilterTypes getFilter() const {return _kfilter;}
        inline KTextureWrapTypes getWrap() const {return _kwrap;}
        inline U32 getID() const {return _ktexId;}

        void setFilter(KTextureFilterTypes Filter);
        void setWrap(KTextureWrapTypes Wrap);

    private:
        //U64 _kuid; // unique texture id (use in state catch)
        U32 _ktexId; // ogl texture name
        KTextureFilterTypes _kfilter; // texture interpolation
        KTextureWrapTypes _kwrap; // texture wrapping
        KVector2U32 _ksize; // size of texture
        static U32 _klastTexId;
    };
}

#endif // KTEXTURE_H

/* Remark
 * always "update" an existing texture is faster than "re-create"
 * update() overload with ImageArea param is slow because pixels data will copy row-by-row into texture
 */
