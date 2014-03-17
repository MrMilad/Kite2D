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
#ifndef KGL2DRENDER_H
#define KGL2DRENDER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include "Kite/Core/graphic/kglrender.h"
#include "Kite/Core/graphic/kcamera.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kshader.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KGL2DRender : public KGLRender{
    public:
        KGL2DRender();
        KGL2DRender(const KCamera &Camera);
        ~KGL2DRender();

        /// initialize renderer
        virtual bool initialize();

        /// clear the scene
        void clear();

        /// draw vbo (ranged)
        /// start: Range.x
        /// lenght: Range.y
        void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive);
        void draw(const KVector2U32 &Range, KGeoPrimitiveTypes Primitive);

        /// draw vbo (indexed)
        void draw(U32 Count, const std::vector<U32> &Indices, KGeoPrimitiveTypes Primitive);

        /// point sprite
        void setPointSprite(bool PointSprite);

        /// clear color
        void setClearColor(const KColor &Color);

        /// set line/point options (size/smooth)
        /// only KGP_POINTS and KGP_LINES is valid for type
        void setLPOptions(KGeoPrimitiveTypes Type, const KLPOption &Options);

        /// 2D camera
        inline void setCamera(const KCamera &Camera) {_kcurrentCam = &Camera;}
        void updateCamera();


        /// set vertex buffer
        void setVertexBuffer(const KVertexBuffer *Buffer);
        inline const KVertexBuffer *getVertexBuffer() const {return _kbuffer;}

        /// set shader
        /// pass 0 = disable shader
        void setShader(const KShader *Shader);
        inline const KShader *getShader() const {return _kshader;}

        /// set texture
        /// pass 0 = disable texture
        void setTexture(const KTexture *Texture);
        inline const KTexture *getTexture() const {return _ktexture;}

        //void setTransform(const KTransform &Transform);
//        void setBlendMode(KBlendMode BlendMode);

        //void unbindBuffers();

        virtual std::string getRendererName();
        virtual KRendererFlags getRendererFlag();

    private:
        const KShader *_kshader;
        const KVertexBuffer *_kbuffer;
        const KTexture *_ktexture;
        const KCamera _kdefaultCam;
        const KCamera *_kcurrentCam;
        KGeoPrimitiveTypes _kgeoType;
        Kite::Internal::KCatchState _kcatch;
    };
}

#endif // KGL2DRENDER_H
