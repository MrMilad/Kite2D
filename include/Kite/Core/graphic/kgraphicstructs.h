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

#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/system/kvector2.h"
#include "Kite/Core/system/ksystemdef.h"

namespace Kite{
    struct KColor{
        U8 r,g,b,a;

        KColor(U8 R = 255, U8 G = 255, U8 B = 255, U8 A = 255):
            r(R), g(G), b(B), a(A)
        {}

        static inline U64 getColorHexCode(const KColor &Color){
            U64 hexCod = (Color.r << 24) | (Color.g << 16) | (Color.b << 8) | Color.a;
            return hexCod;
        }

        static inline void setHexCodeToKColor(KColor &Color, UL32 HexCode){
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

        KVertex(F32 X = -1.0, F32 Y = -1.0, F32 U = 0.0, F32 V = 0.0
                , F32 R = 0.0, F32 G = 0.0, F32 B = 0.0, F32 A = 1.0):
            x(X), y(Y), u(U), v(V),
            r(R), g(G), b(B), a(A)
        {}
    };

    struct KAtlasObject{
        U32 id;
        F32 u,v; // texture position
        U32 w,h; // size

        KAtlasObject(U32 ID = 0, F32 U = 0, F32 V = 0, U32 W = 0, U32 H = 0):
            id(ID),
            u(U), v(V),
            w(W), h(H)
        {}
    };

    struct KAtlasHeader{
        char format[7]; // = {'k', 'a', 't', 'l', 'a', 's', '\0'};
        U32 objCount;
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

    typedef void (KCallVBUpdate)(KVertex *BufferData, U32 Size, void *Sender);

    namespace Internal{
    struct KCatchState{
        /// need fix
        KRenderMode render;
        KBlendMode blend;
        U32 lastBufId;
        U32 lastTexId;
        U32 lastShId;
        bool pointSpr;

        KCatchState(KRenderMode Render = KRM_UNSET,
                    KBlendMode Blend = KB_ALPHA,
                    U16 LastBufferID  = 0,
                    U16 LastTextureID = 0,
                    U16 LastSeaderID  = 0,
                    bool PointSprite = false):
            render(Render),
            blend(Blend),
            lastBufId(LastBufferID),
            lastTexId(LastTextureID),
            lastShId(LastSeaderID),
            pointSpr(PointSprite)
        {}
    };
    }
}

#endif // KRENDERSTRUCTS_H
