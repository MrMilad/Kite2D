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
#include "Kite/Assist/graphic/kquad.h"

namespace Kite{

    KQuad::KQuad():
        _kquadattr(),
        _kshader(0),
        _ktextur(0),
        _kpushed(false)
    {
        _kvertex.resize(4);
    }

    KQuad::KQuad(const Kite::KQuadAttrib &QuadAttribute, const KShader *Shader,
          const KTexture *Texture):
        _kquadattr(QuadAttribute),
        _kshader(Shader), _ktextur(Texture),
        _kpushed(false)
    {
        _kvertex.resize(4);
    }

    void KQuad::setQuadAttribute(const KQuadAttrib &QuadAttribute){
        _kquadattr = QuadAttribute;
        if (_kpreVBuffer != 0 && _kpushed){
            // update vertex
            updateVertex();

            // push
            _kpreVBuffer->updateVertex(_kvertex, _krange);
            return;
        }
    }

    void KQuad::pushVertex(){
        if (_kpreVBuffer != 0){
            if (!_kpreVBuffer->getLock() && !_kpushed){
                // update vertex
                updateVertex();

                // push
                _kpreVBuffer->addVertex(_kvertex, _krange);
                _kpushed = true;
                return;
            }else{
                KDEBUG_PRINT("pre vertex buffer is locked or pushed befor.");
            }
        }else{
            KDEBUG_PRINT("pre vertex buffer not set yet.");
        }
    }

    void KQuad::draw(KGL2DRender &Renderer){
        if (_kpreVBuffer != 0 && _kvbuffer != 0){
            if (_kvisible){

                // set buffer
                Renderer.setVertexBuffer(*_kvbuffer);

                // set texture
                if(_ktextur)
                    Renderer.setTexture(*_ktextur);

                // set shader
                if(_kshader)
                    Renderer.setSheader(*_kshader);

                // ser transform
                Renderer.setTransform(this->getTransform());

                // disable point sprite
                Renderer.setPointSprite(false);

                // draw
                Renderer.draw(_krange.x, _krange.y, KGP_QUADS);
            }
        }else{
            KDEBUG_PRINT("pre vertex buffer or vertex buffer not set yet.");
        }
    }

    void KQuad::updateVertex(){
        // buttom left
        _kvertex[0].x = _kquadattr.buttomLeft.x;
        _kvertex[0].y = _kquadattr.buttomLeft.y;
        _kvertex[0].u = _kquadattr.buttomLeftUV.x;
        _kvertex[0].v = _kquadattr.topRightUV.y;


        // top left
        _kvertex[1].x = _kquadattr.buttomLeft.x;
        _kvertex[1].y = _kquadattr.topRight.y;
        _kvertex[1].u = _kquadattr.buttomLeftUV.x;
        _kvertex[1].v = _kquadattr.buttomLeftUV.y;

        // top right
        _kvertex[2].x = _kquadattr.topRight.x;
        _kvertex[2].y = _kquadattr.topRight.y;
        _kvertex[2].u = _kquadattr.topRightUV.x;
        _kvertex[2].v = _kquadattr.buttomLeftUV.y;


        // buttom right
        _kvertex[3].x = _kquadattr.topRight.x;
        _kvertex[3].y = _kquadattr.buttomLeft.y;
        _kvertex[3].u = _kquadattr.topRightUV.x;
        _kvertex[3].v = _kquadattr.topRightUV.y;

        _kvertex[0].r = _kvertex[1].r = _kvertex[2].r = _kvertex[3].r = _kquadattr.color.r / 255.0f;
        _kvertex[0].g = _kvertex[1].g = _kvertex[2].g = _kvertex[3].g = _kquadattr.color.g / 255.0f;
        _kvertex[0].b = _kvertex[1].b = _kvertex[2].b = _kvertex[3].b = _kquadattr.color.b / 255.0f;
        _kvertex[0].a = _kvertex[1].a = _kvertex[2].a = _kvertex[3].a = _kquadattr.color.a / 255.0f;
    }

}
