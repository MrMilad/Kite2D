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
		KBatch(const std::vector<KQuad> &Quads,
			KVertexBufferTypes Position, KVertexBufferTypes UV, KVertexBufferTypes Color);

        inline void setShader(const KShader *Shader) {_kshader = Shader;}
        inline const KShader *getShader() const {return _kshader;}

        inline void setTexture(const KTexture *Texture) {_ktexture = Texture;}
        inline const KTexture *getTexture() const {return _ktexture;}

        /// update positions
        void updatePosition();
		void updatePosition(U32 FirstIndex, U32 Size);

		/// update uv
		void updateUV();
		void updateUV(U32 FirstIndex, U32 Size);

		/// update color
		void updateColor();
		void updateColor(U32 FirstIndex, U32 Size);

        /// draw all quads
        void draw(KRender &Renderer);

		// draw a part of quads
		void draw(KRender &Renderer, U32 FirstIndex, U32 Size);

    private:
        static void _updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender);
        const std::vector<KQuad> *_kquads;
        KVertexArray _kvao;
		KVertexBuffer _kvboIndx;	/// static index
		KVertexBuffer _kvboXY;		/// xy (position)
        KVertexBuffer _kvboUV;		/// uv (texture uv)
		KVertexBuffer _kvboCol;		/// rgba (color)
        const KShader *_kshader;
        const KTexture *_ktexture;
    };
}

#endif // KBATCH_H
