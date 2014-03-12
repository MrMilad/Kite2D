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

    KGL2DRender::KGL2DRender():
      _kcurrentCam(&_kdefaultCam),
      _kgeoType(KGP_POINTS)
    {}

    KGL2DRender::KGL2DRender(const KCamera &Camera):
        _kcurrentCam(&Camera),
        _kgeoType(KGP_POINTS)
    {}

    KGL2DRender::~KGL2DRender(){}

    bool KGL2DRender::initialize(){
        if (init() == false){ // Initialize GLEW
            return false;
        }

        // disable unnecessary things
        DGL_CALL(glDisable(GL_DEPTH_TEST));
        DGL_CALL(glDisable(GL_LIGHTING));
        DGL_CALL(glDisable(GL_ALPHA_TEST));

        // enable texture and blend
        DGL_CALL(glEnable(GL_TEXTURE_2D));
        DGL_CALL(glEnable(GL_BLEND));
        DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // initialize camera
        updateCamera();

        DGL_CALL(glEnableClientState(GL_VERTEX_ARRAY));
        DGL_CALL(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        DGL_CALL(glEnableClientState(GL_COLOR_ARRAY));
        return true;
     }

    void KGL2DRender::clear(){
        DGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }


    void KGL2DRender::draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive){
        // set geometric type
        static const GLenum geoTypes[] = {GL_POINTS, GL_LINES,
                                         GL_LINE_STRIP, GL_LINE_LOOP,
                                         GL_TRIANGLES, GL_TRIANGLE_STRIP,
                                         GL_TRIANGLE_FAN, GL_QUADS,
                                         GL_QUAD_STRIP, GL_POLYGON};
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0)
            DGL_CALL(glDrawArrays(type, FirstIndex, Size));
    }

    void KGL2DRender::draw(const KVector2U32 &Range, KGeoPrimitiveTypes Primitive){
        draw(Range.x, Range.y, Primitive);
    }

    void KGL2DRender::draw(U32 Count, const std::vector<Kite::U32> &Indices, KGeoPrimitiveTypes Primitive){
        // set geometric type
        static const GLenum geoTypes[] = {GL_POINTS, GL_LINES,
                                         GL_LINE_STRIP, GL_LINE_LOOP,
                                         GL_TRIANGLES, GL_TRIANGLE_STRIP,
                                         GL_TRIANGLE_FAN, GL_QUADS,
                                         GL_QUAD_STRIP, GL_POLYGON};
        _kgeoType = Primitive;
        static GLenum type;
        type = geoTypes[_kgeoType];

        // draw buffer
        if (_kcatch.lastBufId > 0)
            DGL_CALL(glDrawElements(type, Count, GL_UNSIGNED_SHORT, &Indices[0]));
    }

    void KGL2DRender::setVertexBuffer(const KVertexBuffer &Buffer){
        // check last render state for avoide extra ogl state-change
        if (_kcatch.render != Internal::KRM_VBO || _kcatch.lastBufId != Buffer.getID()){
            Buffer.bind();
            DGL_CALL(glVertexPointer(2, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(0)));
            DGL_CALL(glTexCoordPointer(2, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(8)));
            DGL_CALL(glColorPointer(4, GL_FLOAT, sizeof(KVertex), KBUFFER_OFFSET(16)));
            _kcatch.render = Internal::KRM_VBO;
            _kcatch.lastBufId = Buffer.getID();
        }
    }

    void KGL2DRender::setTextureEnv(KTextureEnvMode Mode){
        int envPar[] = {GL_ADD, GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE, GL_COMBINE};
        DGL_CALL(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envPar[Mode]));
    }

    void KGL2DRender::setPointSprite(bool PointSprite){
        if (_kcatch.pointSpr != PointSprite){
            if (PointSprite){
                // enable point sprite
                DGL_CALL(glEnable(GL_POINT_SPRITE));
                DGL_CALL(glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE));
                _kcatch.pointSpr = PointSprite;
            }else{
                DGL_CALL(glDisable(GL_POINT_SPRITE));
                _kcatch.pointSpr = PointSprite;
            }
        }
    }

    void KGL2DRender::setClearColor(const KColor &Color){
        DGL_CALL(glClearColor((GLclampf)((F32)Color.r/255.0f),
                              (GLclampf)((F32)Color.g/255.0f),
                              (GLclampf)((F32)Color.b/255.0f),
                              (GLclampf)((F32)Color.a/255.0f)));
    }

    void KGL2DRender::setPointsOption(const KPointOption &Option){
        DGL_CALL(glPointSize(Option.size));
        switch (Option.drawTypes) {
        case KPD_SMOOTH:
            DGL_CALL(glEnable(GL_POINT_SMOOTH));
            break;
        default:
            DGL_CALL(glDisable(GL_POINT_SMOOTH));
            break;
        }
    }

    void KGL2DRender::updateCamera(){
        // set viewport
        DGL_CALL(glViewport(_kcurrentCam->getViewport().x,
                            _kcurrentCam->getViewport().y,
                            _kcurrentCam->getViewport().width,
                            _kcurrentCam->getViewport().height));

        DGL_CALL(glMatrixMode(GL_PROJECTION));
        DGL_CALL(glLoadMatrixf(_kcurrentCam->getTransform().getMatrix()));

        // Go back to model-view mode
        DGL_CALL(glMatrixMode(GL_MODELVIEW));

    }

    void KGL2DRender::setTexture(const KTexture &Texture){
        if (_kcatch.lastTexId != Texture.getID()){
            Texture.bind();
            _kcatch.lastTexId = Texture.getID();
        }
    }

    void KGL2DRender::setSheader(const Kite::KShader &Sheader){
        if (_kcatch.lastShId != Sheader.getID()){
            Sheader.bind();
            _kcatch.lastShId = Sheader.getID();
        }
    }

    void KGL2DRender::setTransform(const KTransform &Transform){
        // it is always the GL_MODELVIEW
        DGL_CALL(glLoadMatrixf(Transform.getMatrix()));
    }

    std::string KGL2DRender::getRendererName(){
        return "Kite2D Fixed (default) 2D Renderer.";
    }

    KRendererFlags KGL2DRender::getRendererFlag(){
        return KITE2D_FIXED_2D;
    }
}
