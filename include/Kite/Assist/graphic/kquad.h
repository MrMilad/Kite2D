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
#ifndef KQUAD_H
#define KQUAD_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Assist/graphic/kdrawable.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshader.h"
#include "Kite/Core/graphic/ktransform.h"
#include "Kite/Core/system/kvector2.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KQuad : public KDrawable{
    public:
        KQuad();
        KQuad(const KVector2F32 &ButtomLeft, const KVector2F32 &TopRight, const KColor &Color, const KShader *Shader,
              const KTexture *Texture, const KVector2F32 &ButtomLeftUV, const KVector2F32 &TopRightUV);

        //virtual ~KQuad();

//        inline void setButtomLeft(const KVector2F32 &ButtomLeft) {_kbuttomLeft = ButtomLeft;}
//        inline KVector2F32 getButtomLeft() const {return _kbuttomLeft;}

//        inline void setTopRight(const KVector2F32 &TopRight) {_ktopRight = TopRight;}
//        inline KVector2F32 getTopRight() const {return _ktopRight;}

//        inline void setColor(const KColor &Color) { _kcolor = Color;}
//        inline KColor getColor() const {return _kcolor;}

//        inline void setShader(const KShader *Shader) {_kshader = Shader;}
//        inline const KShader *getShader() const {return _kshader;}

//        inline void setTexture(const KTexture *Texture) {_ktextur = Texture;}
//        inline const KTexture *getTexture() const {return _ktextur;}

//        inline void setTextureUV(const KVector2F32 &TextureUV) {_kuv = TextureUV;}
//        inline KVector2F32 getTextureUV() const {return _kuv;}

        void pushVertex(KVertexVector &Vector);
        void update(KVertexBuffer &Buffer);
        void draw(KGL2DRender &Renderer, const KVertexBuffer &Buffer);

        KVector2F32 buttomLeft;
        KVector2F32 topRight;
        KColor color;
        const KShader *shader;
        const KTexture *textur;
        const KTransform *transform;
        KVector2F32 buttomLeftUV;
        KVector2F32 topRightUV;

    private:
        KVector2U32 _krange;
        KVertex _kvertex[4];
    };
}

#endif // KQUAD_H
