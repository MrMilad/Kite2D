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
#ifndef KFRAMEBUFFER_H
#define KFRAMEBUFFER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/ktexture.h"

namespace Kite{
    class KITE_FUNC_EXPORT KFrameBuffer : KNonCopyable{
        KFrameBuffer();
        ~KFrameBuffer();

        /// attach a texture to color buffer (render to texture)
        void attachTexture(const KTexture *Texture);

        // void attachRenderBuffer(const KRenderBuffer *RenderBuffer);

        void bind();

        /// unbind the frame buffer if it is currently in use
        void unbind();

        /// unbind currently frame buffer
        static void unbindFrameBuffer();

    private:
        U32 _kbufId;
        static U32 _klastBufId;
    };
}

#endif // KFRAMEBUFFER_H
