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
#include "Kite/core/graphic/ktexture.h"
#include "src/Kite/core/graphic/glcall.h"

namespace Kite{
    U32 Kite::KTexture::_klastTexId = 0;
	KTexture::KTexture() :
		KCoreInstance(KCI_TEXTURE),
        _ktexId(0),
        _kfilter(KTF_LINEAR),
        _kwrap(KTW_CLAMP_TO_EDGE),
        _ksize(KVector2U32(0,0))
    {
        // generate texture
        DGL_CALL(glGenTextures(1, &_ktexId));
    }

    KTexture::~KTexture(){
        DGL_CALL(glDeleteTextures(1, &_ktexId));
        if (_klastTexId == _ktexId){
            _klastTexId = 0;
        }
    }

    void KTexture::create(const Kite::KVector2U32 &Size, KTextureFilterTypes Filter, KTextureWrapTypes Wrap){
        _create(0, Size, Filter, Wrap, *this);
    }

    void KTexture::create(const KImage &Image, KTextureFilterTypes Filter, KTextureWrapTypes Wrap){
        _create(Image.getPixelsData(), Image.getSize(), Filter, Wrap, *this);
    }

    void KTexture::update(const KImage &Image, const KVector2U32 &Position){
        if (_ktexId > 0){

            // check new image size with texture size
			KDEBUG_ASSERT_T((Position.x + Image.getSize().x) <= _ksize.x);
			KDEBUG_ASSERT_T((Position.y + Image.getSize().y) <= _ksize.y);

            // save currently bound texture then bind our texture temporary
            Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

            // update pixels
			DGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, Position.x, Position.y,
                                     Image.getSize().x, Image.getSize().y,
                                     GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsData()));
        }else{
            KDEBUG_PRINT("texture is not created.");
        }
    }

    void KTexture::bind() const{
        // bind texture
        if (_klastTexId != _ktexId){
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));
            _klastTexId = _ktexId;
        }
    }

    void KTexture::unbind(){
        if (_klastTexId == _ktexId){
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
            _klastTexId = 0;
        }
    }

    void KTexture::unbindTexture(){
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        _klastTexId = 0;
    }

    void KTexture::setFilter(KTextureFilterTypes Filter){
        if (_ktexId > 0){

            if (Filter != _kfilter){
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
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
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
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

    void KTexture::_create(const U8 *Data, const KVector2U32 &Size,
                        KTextureFilterTypes Filter, KTextureWrapTypes Wrap, KTexture &Instance){

        // save currently binded texture then bind our texture temporary
		Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, Instance._ktexId);
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, Instance._ktexId));

        // fill texture with image pixel data
        DGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Size.x, Size.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, Data));

        // save texture size
        Instance._ksize = Size;

        // setup texture parameters
        int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[Wrap]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[Wrap]));
        Instance._kwrap = Wrap;

        int filterType[] = {GL_NEAREST, GL_LINEAR};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[Filter]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[Filter]));
        Instance._kfilter = Filter;
    }

	U64 KTexture::getInstanceSize() const{
		return _ksize.x * _ksize.y * 4;
	}

}
