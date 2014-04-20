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
#ifndef KBATCH_H
#define KBATCH_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kshader.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kvertexarray.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Assist/graphic/kdrawable.h"
#include "Kite/Assist/graphic/kvertexvector.h"
#include "Kite/Assist/graphic/kquad.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KBatch : public KDrawable{
    public:
        KBatch(const std::vector<KQuad> &Quads);

        inline void setShader(const KShader *Shader) {_kshader = Shader;}
        inline const KShader *getShader() const {return _kshader;}

        inline void setTexture(const KTexture *Texture) {_ktexture = Texture;}
        inline const KTexture *getTexture() const {return _ktexture;}

        /// update model-view matrix
        void update();

        /// draw all quads
        void draw(KRender &Renderer);

    private:
        static void _update(void *Data, I64 DataSize, void *Sender);
        const std::vector<KQuad> *_kquads;
		std::vector<KVBPack5> _kposArray;
        KVertexArray _kvao;
		KVertexBuffer _kvboIndx;	/// static index
		KVertexBuffer _kvboXY;		/// stream xy (position)
        KVertexBuffer _kvboUVC;		/// static uv (texture uv) , rgba (color)
        const KShader *_kshader;
        const KTexture *_ktexture;
    };
}

#endif // KBATCH_H
