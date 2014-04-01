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
#include "Kite/Assist/graphic/kbufferable.h"
#include "Kite/Assist/graphic/ktransformable.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshader.h"
#include "Kite/Core/system/kvector2.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KQuad : public KDrawable, public KBufferable, public KTransformable{
    public:
        KQuad();
        KQuad(const KQuadAttrib &QuadAttribute, KShader *Shader, const KTexture *Texture);

        //virtual ~KQuad();

        void setQuadAttribute(const KQuadAttrib &QuadAttribute);
        inline const KQuadAttrib *getQuadAttribute() const {return &_kquadattr;}

        inline void setShader(KShader *Shader) {_kshader = Shader;}
        inline KShader *getShader() const {return _kshader;}

        inline void setTexture(const KTexture *Texture) {_ktexture = Texture;}
        inline const KTexture *getTexture() const {return _ktexture;}

        /// push vertex data to pre vertex buffer
        void pushVertex();

        /// draw
        void draw(KGL2DRender &Renderer);

    private:
        void _updateVertex();
        KQuadAttrib _kquadattr;
        KShader *_kshader;
        const KTexture *_ktexture;

        KVector2U32 _krange;
        std::vector<KVertex> _kvertex;
        bool _kpushed;
    };
}

#endif // KQUAD_H
