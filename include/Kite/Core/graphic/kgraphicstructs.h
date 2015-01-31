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
#ifndef KGRAPHICSTRUCTS_H
#define KGRAPHICSTRUCTS_H

#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/math/kvector2.h"
#include "Kite/Core/system/ksystemdef.h"
#include <vector>

namespace Kite{
    struct KColor{
        F32 r,g,b,a;

		KColor(F32 R = 255, F32 G = 255, F32 B = 255, F32 A = 255) :
            r(R/255.0f), g(G/255.0f), b(B/255.0f), a(A/255.0f)
        {}
    };

	/*static inline U64 getColorHexCode(const KColor &Color){
		U64 hexCod = (Color.r << 24) | (Color.g << 16) | (Color.b << 8) | Color.a;
		return hexCod;
	}

	static inline void setHexCodeToKColor(KColor &Color, UL32 HexCode){
		Color.r = (U8)((HexCode >> 16) & 0xFF);  // Extract the RR byte
		Color.g = (U8)((HexCode >> 8) & 0xFF);   // Extract the GG byte
		Color.b = (U8)((HexCode)& 0xFF);        // Extract the BB byte
	}*/

    /// position, texture, color
    struct KVertex{
        KVector2F32 pos;
        KVector2F32 uv;
        KColor color;

        KVertex():
            pos(), uv(), color()
        {}

        KVertex(KVector2F32 Pos, KVector2F32 UV, KColor Color):
            pos(Pos), uv(UV), color(Color)
        {}
    };

    /// position, texture
    /*struct KVBPack2{
        F32 x,y;
        F32 u,v;

        KVBPack2(F32 X = -1.0, F32 Y = -1.0, F32 U = 0.0, F32 V = 0.0):
            x(X), y(Y), u(U), v(V)
        {}
    };

    /// position, color
    struct KVBPack3{
        F32 x,y;
        F32 r,g,b,a;

        KVBPack3(F32 X = -1.0, F32 Y = -1.0,
                 F32 R = 0.0, F32 G = 0.0, F32 B = 0.0, F32 A = 1.0):
            x(X), y(Y),
            r(R), g(G), b(B), a(A)
        {}
    };

	// uv, color
	struct KVBPack4{
		F32 u,v;
		F32 r,g,b,a;

		KVBPack4(F32 U = 0.0f, F32 V = 0.0f,
				 F32 R = 0.0, F32 G = 0.0, F32 B = 0.0, F32 A = 1.0) :
			u(U), v(V),
			r(R), g(G), b(B), a(A)
		{}
	};

    /// position (Point)
    struct KVBPack5{
        F32 x,y;

        KVBPack5(F32 X = -1.0, F32 Y = -1.0):
            x(X), y(Y)
        {}
    };*/

    template <typename T>
    struct KRect{
		T left, right, bottom, top;

		KRect(T Left = 0, T Right = 0, T Bottom = 0, T Top = 0) :
			left(Left), right(Right), bottom(Bottom), top(Top)
        {}
    };

	template <typename T>
	inline KRect<T> operator+(const KRect<T>& left, const KVector2<T>& right){
		return KRect<T>(left.left + right.x, left.right + right.x,
			left.bottom + right.y, left.top + right.y);
	}

	template <typename T>
	inline KRect<T> operator-(const KRect<T>& left, const KVector2<T>& right){
		return KRect<T>(left.left - right.x, left.right - right.x,
			left.bottom - right.y, left.top - right.y);
	}

    typedef KRect<U8>  KRectU8;
    typedef KRect<U16> KRectU16;
    typedef KRect<U32> KRectU32;
    typedef KRect<I8>  KRectI8;
    typedef KRect<I16> KRectI16;
    typedef KRect<I32> KRectI32;
    typedef KRect<F32> KRectF32;
    typedef KRect<F64> KRectF64;

	struct KAtlasObject{
		U32 id;
		F32 blu, blv; // bottom left texture position (-1 to 1)
		F32 tru, trv; // top right texture position (-1 to 1)
		F32 w, h; // size (in pixel)

		KAtlasObject(U32 ID = 0, F32 BLU = 0, F32 BLV = 0,
			F32 TRU = 0, F32 TRV = 0, F32 W = 0, F32 H = 0) :
			id(ID),
			blu(BLU), blv(BLV),
			tru(TRU), trv(TRV),
			w(W), h(H)
		{}
	};

	struct KAtlasHeader{
		char format[7]; // = {'k', 'a', 't', 'l', 'a', 's', '\0'}; // file format
		U32 objCount; // number of atlas objects stored in the file
	};

	struct KAnimeState{
		KVector2F32 translate; // per frame
		KVector2F32 scale; // per frame
		F32 rotate; // per frame
		U32 uv; // per frame (index of atlas object)
		KAnimeState(KVector2F32 Translate, KVector2F32 Scale,
			F32 Rotate = 0.0f, U32 UV = 0) :
			translate(Translate), scale(Scale), 
			rotate(Rotate), uv(UV)
		{}
		KAnimeState() :
			translate(), scale(),
			rotate(0.0f), uv(0)
		{}
	};

	struct KAnimeHeader{
		char format[7]; // = {'k', 'a', 'n', 'i', 'm', 'e', '\0'}; // file format
		U8 animeCount; // number of animations stored in the file
		// U16 *jointCount; // = U16 jointCount[animeCount]; // number of joints per animation.
		// U16 *frameCount; // = U16 frameCount[animeCount]; // number of frames per animation.
		// KAnimeState *states;	// = KAnimeState states[size of total frames]; // animations states
	};

	struct KAnimeObject{
		U16 joints;							// number of joints
		U16 frames;							// number of frames
		std::vector<KAnimeState> states;	// joint states
	};

    struct KOGLVersion{
        U8 major;
        U8 minor;
    };

    typedef void (KCallVBUpdate)(void *Data, U32 Offset, U32 DataSize, void *Sender);

    /*struct KQuadAttribute{
        KVector2F32 bottomLeft;
        KVector2F32 topRight;
        KColor color;
        KVector2F32 bottomLeftUV;
        KVector2F32 topRightUV;

		KQuadAttribute() :
            bottomLeft(-1.0, -1.0),
            topRight(0.0,0.0),
            color(),
            bottomLeftUV(0,0),
            topRightUV(1,1)
        {}

		KQuadAttribute(const KVector2F32 &BottomLeft, const KVector2F32 &TopRight, const KColor &Color,
                     const KVector2F32 &BottomLeftUV, const KVector2F32 &TopRightUV):
            bottomLeft(BottomLeft), topRight(TopRight),
            color(Color),
            bottomLeftUV(BottomLeftUV), topRightUV(TopRightUV)
        {}
    };*/

    struct KParticle{
		KVector2F32 pos;
		KColor color;
        I32 angle;
        I32 speed;
        U32 life;
        U32 size;
    };

    struct KBatchConfig{
        KVertexBufferTypes position, uv, color;
        KBatchConfig(KVertexBufferTypes Position = KVB_STATIC, KVertexBufferTypes UV = KVB_STATIC,
                     KVertexBufferTypes Color = KVB_STATIC):
            position(Position), uv(UV), color(Color)
        {}
    };

    namespace Internal{
    struct KCatchState{
        /// need fix
        KBlendMode blend;
        U32 lastVAOId, lastVBOId, lastTexId, lastShdId;
        bool pointSpr;

        KCatchState(KBlendMode Blend = KB_ALPHA,
                    U32 LastVAOID = 0, U32 LastVBOID = 0,
                    U32 LastTextureID = 0, U32 LastSeaderID = 0,
                    bool PointSprite = false):
            blend(Blend),
            lastVAOId(LastVAOID),
            lastVBOId(LastVBOID),
            lastTexId(LastTextureID),
            lastShdId(LastSeaderID),
            pointSpr(PointSprite)
        {}
    };
    }
}

#endif // KGRAPHICSTRUCTS_H
