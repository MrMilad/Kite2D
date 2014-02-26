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
#ifndef KRENDERSTRUCTS_H
#define KRENDERSTRUCTS_H

#include "Kite/system/ksystemtypes.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/system/kvector2.h"
#include "Kite/system/ksystemdef.h"

namespace Kite{
    struct KColor{
        U32 r,g,b,a;

        KColor(U32 R = 1, U32 G = 1, U32 B = 1, U32 A = 0):
            r(R), g(G), b(B), a(A)
        {}

        static inline U64 getColorHexCode(const KColor &Color){
            U64 hexCod = (Color.r << 24) | (Color.g << 16) | (Color.b << 8) | Color.a;
            return hexCod;
        }

        static inline void setHexCodeToKColor(KColor &Color, U64 HexCode){
            Color.r = (HexCode & 0xFF000000) >> 24;
            Color.g = (HexCode & 0x00FF0000) >> 16;
            Color.b = (HexCode & 0x0000FF00) >> 8;
            Color.b = (HexCode & 0x000000FF);
        }
    };

    struct KPointOption{
        F32 size;
        KPointDrawTypes drawTypes;
    };

    struct KVertex{
        F32 x,y;
        F32 u,v;
        F32 r,g,b,a;
    };

    template <typename T>
    struct KRect{
        T x, y, width, height;

        KRect(T XPos = 0, T YPos = 0, T Width = 0, T Heigt = 0):
            x(XPos), y(YPos), width(Width), height(Heigt)
        {}
    };

    typedef KRect<U8>  KRectU8;
    typedef KRect<U16> KRectU16;
    typedef KRect<U32> KRectU32;
    typedef KRect<I8>  KRectI8;
    typedef KRect<I16> KRectI16;
    typedef KRect<I32> KRectI32;
    typedef KRect<F32> KRectF32;
    typedef KRect<F64> KRectF64;

    struct KOGLVersion{
        U8 major;
        U8 minor;
    };

    typedef void (KCallVBUpdate)(KVertex *BufferData, U32 Size);

    namespace Internal{
    struct KCatchState{
        /// need fix
        KRenderMode render;
        KBlendMode blend;
        U16 lastBufId;
        U16 lastTexId;
        U16 lastShId;

        KCatchState(KRenderMode Render = KRM_UNSET,
                    KBlendMode Blend = KB_ALPHA,
                    U16 LastBufferID  = 0,
                    U16 LastTextureID = 0,
                    U16 LastSeaderID  = 0):
            render(Render),
            blend(Blend),
            lastBufId(LastBufferID),
            lastTexId(LastTextureID),
            lastShId(LastSeaderID)
        {}
    };
    }
}

#endif // KRENDERSTRUCTS_H
