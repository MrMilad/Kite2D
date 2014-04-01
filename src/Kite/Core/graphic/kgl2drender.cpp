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
#include "Kite/core/graphic/kgl2drender.h"
#include "src/Kite/core/graphic/glcall.h"

namespace Kite{

    // gl geometric types
    const U32 KGL2DRender::geoTypes[] = {GL_POINTS, GL_LINES,
                                    GL_LINE_STRIP, GL_LINE_LOOP,
                                    GL_TRIANGLES, GL_TRIANGLE_STRIP,
                                    GL_TRIANGLE_FAN, GL_LINE_STRIP_ADJACENCY,
                                    GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY,
                                    GL_TRIANGLES_ADJACENCY};

    KGL2DRender::KGL2DRender():
      _kshader(0),
      _kbuffer(0),
      _ktexture(0),
      _kviewport(),
      _kgeoType(KGP_POINTS)
    {}

    KGL2DRender::KGL2DRender(const Kite::KRectI32 &Viewport):
        _kshader(0),
        _kbuffer(0),
        _ktexture(0),
        _kviewport(Viewport),
        _kgeoType(KGP_POINTS)
    {}

    KGL2DRender::~KGL2DRender(){}

    bool KGL2DRender::initialize(){
        if (init() == false){ // Initialize GLEW
            KDEBUG_PRINT("Initialization Failed");
            return false;
        }

        DGL_CALL(glDisable(GL_DEPTH_TEST));

        // enable blend
        DGL_CALL(glEnable(GL_BLEND));
        DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // initialize camera
        updateCamera();

        // main vao
        GLuint vao;
        DGL_CALL(glGenVertexArrays(1, &vao));
        DGL_CALL(glBindVertexArray(vao));

        // attribute buffer : vertices coordinate (x,y)
        DGL_CALL(glEnableVertexAttribArray(0));

        // attribute buffer : texture coordinate (u,v)
        DGL_CALL(glEnableVertexAttribArray(1));

        // attribute buffer : color (r,g,b,a)
        DGL_CALL(glEnableVertexAttribArray(2));

        return true;
     }

    void KGL2DRender::clear(){
        DGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }


    void KGL2DRender::draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive){
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0 && _kcatch.lastShId > 0)
            DGL_CALL(glDrawArrays(type, FirstIndex, Size));
    }

    void KGL2DRender::draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive, U32 InstanceCount){
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0 && _kcatch.lastShId > 0)
            DGL_CALL(glDrawArraysInstanced(type, FirstIndex, Size, InstanceCount));
    }

    void KGL2DRender::draw(U32 Count, const std::vector<U32> &Indices, KGeoPrimitiveTypes Primitive){
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0)
            DGL_CALL(glDrawElements(type, Count, GL_UNSIGNED_SHORT, &Indices[0]));
    }

    void KGL2DRender::draw(U32 Count, const std::vector<U32> &Indices, KGeoPrimitiveTypes Primitive, U32 InstanceCount){
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0)
            DGL_CALL(glDrawElementsInstanced(type, Count, GL_UNSIGNED_SHORT, &Indices[0], InstanceCount));
    }

    void KGL2DRender::setVertexBuffer(const Kite::KVertexBuffer *Buffer){
        if (Buffer != 0){
            // check last render state for avoide extra ogl state-change
            if (_kcatch.render != Internal::KRM_VBO || _kcatch.lastBufId != Buffer->getID()){
                Buffer->bind();

                // vertex coordinate (x,y)
                DGL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(KVertex), KBUFFER_OFFSET(0)));
                //DGL_CALL(glVertexPointer(2, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(0)));

                // texture coordinate (u,v)
                DGL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(KVertex), KBUFFER_OFFSET(8)));
                //DGL_CALL(glTexCoordPointer(2, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(8)));

                // color component (r,g,b,a)
                DGL_CALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(KVertex), KBUFFER_OFFSET(16)));
                //DGL_CALL(glColorPointer(4, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(16)));

                _kcatch.render = Internal::KRM_VBO;
                _kcatch.lastBufId = Buffer->getID();
                _kbuffer = Buffer;
            }
        }else{
            KVertexBuffer::unbind();
            _kcatch.lastBufId = 0;
            _kbuffer = 0;
        }
    }

    void KGL2DRender::setPointSprite(bool PointSprite){
        _kcatch.pointSpr = PointSprite;
    }

    void KGL2DRender::setClearColor(const KColor &Color){
        DGL_CALL(glClearColor((GLclampf)((F32)Color.r/255.0f),
                              (GLclampf)((F32)Color.g/255.0f),
                              (GLclampf)((F32)Color.b/255.0f),
                              (GLclampf)((F32)Color.a/255.0f)));
    }

    void KGL2DRender::setLPOptions(KGeoPrimitiveTypes Type, const KLPOption &Options){
        switch (Type){
        // line
        case KGP_LINES:
            DGL_CALL(glLineWidth(Options.size));
            break;

        // point
        case KGP_POINTS:
            DGL_CALL(glPointSize(Options.size));
            break;

        // invalid ...
        default:
            KDEBUG_PRINT("invalid type");
            break;
        }
    }

    void KGL2DRender::setViewport(const KRectI32 &Viewport){
        _kviewport = Viewpoert;
        DGL_CALL(glViewport(_kviewport.x,
                            _kviewport.y,
                            _kviewport.width,
                            _kviewport.height));
    }

    void KGL2DRender::setTexture(const Kite::KTexture *Texture){
        if (Texture != 0){
            if (_kcatch.lastTexId != Texture->getID()){
                Texture->bind();
                _kcatch.lastTexId = Texture->getID();
                _ktexture = Texture;
            }
        }else{
            KTexture::unbind();
            _kcatch.lastTexId = 0;
            _ktexture = 0;
        }
    }

    void KGL2DRender::setShader(const Kite::KShader *Shader){
        if (Shader != 0){
            if (_kcatch.lastShId != Shader->getID()){
                Shader->bind();
                _kshader = Shader;
                _kcatch.lastShId = Shader->getID();
            }
        }else{
            KShader::unbind();
            _kcatch.lastShId = 0;
            _kshader = 0;
        }
    }

    std::string KGL2DRender::getRendererName(){
        return "Kite2D Fixed (default) 2D Renderer.";
    }

    KRendererFlags KGL2DRender::getRendererFlag(){
        return KITE2D_FIXED_2D;
    }
}
