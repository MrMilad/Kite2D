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
        buttomLeft(-1.0, -1.0),
        topRight(0.0,0.0),
        color(),
        buttomLeftUV(0,0),
        topRightUV(1,1),
        _kshader(0),
        _ktextur(0)
    {}

    KQuad::KQuad(const KVector2F32 &ButtomLeft, const KVector2F32 &TopRight, const KColor &Color, const KShader *Shader,
          const KTexture *Texture, const KVector2F32 &ButtomLeftUV, const Kite::KVector2F32 &TopRightUV):
        buttomLeft(ButtomLeft), topRight(TopRight),
        color(Color),
        buttomLeftUV(ButtomLeftUV), topRightUV(TopRightUV),
        _kshader(Shader), _ktextur(Texture)
    {}

    void KQuad::pushVertex(KVertexVector &Vector){
        std::vector<KVertex> vertex;

        // buttom left
        _kvertex[0].x = buttomLeft.x;
        _kvertex[0].y = buttomLeft.y;
        _kvertex[0].u = buttomLeftUV.x;
        _kvertex[0].v = topRightUV.y;


        // top left
        _kvertex[1].x = buttomLeft.x;
        _kvertex[1].y = topRight.y;
        _kvertex[1].u = buttomLeftUV.x;
        _kvertex[1].v = buttomLeftUV.y;

        // top right
        _kvertex[2].x = topRight.x;
        _kvertex[2].y = topRight.y;
        _kvertex[2].u = topRightUV.x;
        _kvertex[2].v = buttomLeftUV.y;


        // buttom right
        _kvertex[3].x = topRight.x;
        _kvertex[3].y = buttomLeft.y;
        _kvertex[3].u = topRightUV.x;
        _kvertex[3].v = topRightUV.y;

        _kvertex[0].r = _kvertex[1].r = _kvertex[2].r = _kvertex[3].r = color.r / 255.0f;
        _kvertex[0].g = _kvertex[1].g = _kvertex[2].g = _kvertex[3].g = color.g / 255.0f;
        _kvertex[0].b = _kvertex[1].b = _kvertex[2].b = _kvertex[3].b = color.b / 255.0f;
        _kvertex[0].a = _kvertex[1].a = _kvertex[2].a = _kvertex[3].a = color.a / 255.0f;
        // assign vertex data to the vector
        vertex.assign(_kvertex, _kvertex+4);

        // push
        Vector.addVertex(vertex, _krange);
    }

    void KQuad::update(KVertexBuffer &Buffer){
        // buttom left
        _kvertex[0].x = buttomLeft.x;
        _kvertex[0].y = buttomLeft.y;
        _kvertex[0].u = buttomLeftUV.x;
        _kvertex[0].v = buttomLeftUV.y;


        // top left
        _kvertex[1].x = buttomLeft.x;
        _kvertex[1].y = topRight.y;
        _kvertex[1].u = buttomLeftUV.x;
        _kvertex[1].v = topRightUV.y;

        // top right
        _kvertex[2].x = topRight.x;
        _kvertex[2].y = topRight.y;
        _kvertex[2].u = topRightUV.x;
        _kvertex[2].v = topRightUV.y;


        // buttom right
        _kvertex[3].x = topRight.x;
        _kvertex[3].y = buttomLeft.y;
        _kvertex[3].u = topRightUV.x;
        _kvertex[3].v = buttomLeftUV.y;


        _kvertex[0].r = _kvertex[1].r = _kvertex[2].r = _kvertex[3].r = color.r / 255.0f;
        _kvertex[0].g = _kvertex[1].g = _kvertex[2].g = _kvertex[3].g = color.g / 255.0f;
        _kvertex[0].b = _kvertex[1].b = _kvertex[2].b = _kvertex[3].b = color.b / 255.0f;
        _kvertex[0].a = _kvertex[1].a = _kvertex[2].a = _kvertex[3].a = color.a / 255.0f;

        Buffer.update(_krange.x, _krange.y, _kvertex);
    }


    void KQuad::draw(KGL2DRender &Renderer, const KVertexBuffer &Buffer){
        // set texture
        if(_ktextur)
            Renderer.setTexture(*_ktextur);

        // set shader
        if(_kshader)
            Renderer.setSheader(*_kshader);

        // ser transform
        Renderer.setTransform(this->getTransform());

        // set buffer
        Renderer.setVertexBuffer(Buffer);

        // disable point sprite
        Renderer.setPointSprite(false);

        // draw
        Renderer.draw(_krange.x, _krange.y, KGP_QUADS);
    }

}
