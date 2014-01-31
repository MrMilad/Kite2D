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
#include "kite/graphic/ktexture.h"
#include "Kite/graphic/glcall.h"

namespace Kite{
    U32 Kite::KTexture::_klastTexId = 0;
    KTexture::KTexture():
        _ktexId(0),
        _kfilter(KTF_LINEAR),
        _kwrap(KTW_CLAMP_TO_EDGE),
        _ksize(KVector2U32(0,0))
    {}

    KTexture::~KTexture(){
        if (_ktexId != 0){
            DGL_CALL(glDeleteTextures(1, &_ktexId));
            if (_klastTexId == _ktexId){
                _klastTexId = 0;
            }
        }
    }

    void KTexture::create(const KImage &Image, KTextureFilterTypes Filter, KTextureWrapTypes Wrap){
        // generate texture
        if (_ktexId <= 0)
            DGL_CALL(glGenTextures(1, &_ktexId));

        // save currently binded texture then bind our texture temporary
        Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId);
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

        // fill texture with image pixel data
        DGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image.getSize().x, Image.getSize().y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsData()));

        // save texture size
        _ksize.x = Image.getSize().x;
        _ksize.y = Image.getSize().y;

        // setup texture parameters
        int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[Wrap]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[Wrap]));
        _kwrap = Wrap;

        int filterType[] = {GL_NEAREST, GL_LINEAR};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[Filter]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[Filter]));
        _kfilter = Filter;
    }

    void KTexture::update(const KImage &Image, U32 XPos, U32 YPos){
        if (_ktexId > 0){

            // check new image size with texture size
            KDEBUG_ASSERT((XPos + Image.getSize().x) <= _ksize.x);
            KDEBUG_ASSERT((YPos + Image.getSize().y) <= _ksize.y);

            // save currently binded texture then bind our texture temporary
            Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId);
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

            // update pixels
            DGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, XPos, YPos,
                                     Image.getSize().x, Image.getSize().y,
                                     GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsData()));
        }else{
            KDEBUG_PRINT("texture is not created.");
        }
    }

    void KTexture::bind(){
        // bind texture
        if (_ktexId > 0){
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));
            _klastTexId = _ktexId;
        }
    }

    void KTexture::setFilter(KTextureFilterTypes Filter){
        if (_ktexId > 0){

            if (Filter != _kfilter){
                // save currently binded texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // set filter type
                int filterType[] = {GL_NEAREST, GL_LINEAR};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[Filter]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[Filter]));
                _kfilter = Filter;
            }
        }else{
            KDEBUG_PRINT("texture is not created.");
        }
    }

    void KTexture::setWrap(KTextureWrapTypes Wrap){
        if (_ktexId > 0){

            if (Wrap != _kwrap){
                // save currently binded texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // setup texture parameters
                int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[Wrap]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[Wrap]));
                _kwrap = Wrap;
            }
        }else{
            KDEBUG_PRINT("texture is not created.");
        }
    }

}
