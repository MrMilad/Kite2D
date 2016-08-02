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
#include "Kite/graphic/ktexture.h"
#include "src/Kite/graphic/glcall.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/core/kfostream.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
    U32 Kite::KTexture::_klastTexId = 0;
	KTexture::KTexture(const std::string &Name) :
		KResource(Name, false),
        _ktexId(0),
        _kfilter(TextureFilter::LINEAR),
        _kwrap(TextureWrap::CLAMP_TO_EDGE),
        _ksize(KVector2U32(0,0))
    {
		_kimage.create(1, 1, KColor(0, 0, 0, 255));
	}

    KTexture::~KTexture(){
		if (_ktexId != 0) {
			DGL_CALL(glDeleteTextures(1, &_ktexId));
		}
        if (_klastTexId == _ktexId){
            _klastTexId = 0;
        }
    }

	bool KTexture::_saveStream(KOStream *Stream, const std::string &Address) {
		// texture information
		KBinarySerial bserial;
		bserial << std::string("KTex");
		bserial << _kfilter;
		bserial << _kwrap;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream->close();
			return false;
		}

		Stream->close();

		// texture pixels (we always pixels in png format)
		// online pixel data (opengl side)
		if (isInite()) {
			// save currently binded texture then bind our texture temporary
			Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
			DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

			U8 *pdata = new U8[_ksize.x * _ksize.y];
			DGL_CALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA8, GL_UNSIGNED_BYTE, (GLvoid *)pdata));
			_kimage.create(_ksize.x, _ksize.y, pdata);
			_kimage.saveStream(Stream, Address + ".png");
			delete[] pdata;

		// offline pixel data (memory side)
		} else {
			if (!_kimage.saveStream(Stream, Address + ".png")) {
				KD_FPRINT("cant save image. rname: %s", getName().c_str() );
				return false;
			}
		}

		return true;
	}

	bool KTexture::_loadStream(KIStream *Stream, const std::string &Address) {
		setModified(true);
		// load texture info 
		if (!Stream->isOpen()) {
			Stream->close();
		}

		if (!Stream->open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_PRINT("can't load stream");
			Stream->close();
			return false;
		}
		std::string format;

		bserial >> format;

		if (format != "KTex") {
			KD_PRINT("incorrect file format");
			Stream->close();
			return false;
		}

		bserial >> _kfilter;
		bserial >> _kwrap;

		Stream->close();

		// load image
		if (!_kimage.loadStream(Stream, Address + ".png")) {
			KD_PRINT("cant load image file");
			Stream->close();
			return false;
		}

		return true;
	}

    void KTexture::create(const Kite::KVector2U32 &Size, TextureFilter Filter, TextureWrap Wrap){
		setModified(true);
		_kfilter = Filter;
		_kwrap = Wrap;
		_kimage.create(Size.x, Size.y, KColor(0, 0, 0, 255));
    }

    void KTexture::create(const KImage &Image, TextureFilter Filter, TextureWrap Wrap){
		setModified(true);
		_kfilter = Filter;
		_kwrap = Wrap;
		_kimage = Image;
    }

	void KTexture::getImage(KImage &ImageOutput) {
		// online pixel data (opengl side)
		if (isInite()) {
			// save currently binded texture then bind our texture temporary
			Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
			DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

			U8 *pdata = new U8[_ksize.x * _ksize.y];
			DGL_CALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA8, GL_UNSIGNED_BYTE, (GLvoid *)pdata));
			ImageOutput.create(_ksize.x, _ksize.y, pdata);
			delete[] pdata;

		// offline pixel data (memory side)
		} else {
			ImageOutput.create(_kimage.getSize().x, _kimage.getSize().y, _kimage.getPixelsData());
		}
	}

	bool KTexture::inite() {
		// generate texture
		DGL_CALL(glGenTextures(1, &_ktexId));

		// transfer pixel data from ram to vram (opengl texture)
		_create(_kimage.getPixelsData(), _kimage.getSize(), _kfilter, _kwrap, *this);

		// release pixel data
		_kimage.clear();

		setInite(true);
		return true;
	}

    void KTexture::update(const KImage &Image, const KVector2U32 &Position){
		if (!isInite()) {
			KD_FPRINT("texture not initialized. rname: %s", getName().c_str());
			return;
		}

        if (_ktexId > 0){

            // check new image size with texture size
			KD_ASSERT((Position.x + Image.getSize().x) <= _ksize.x);
			KD_ASSERT((Position.y + Image.getSize().y) <= _ksize.y);

            // save currently bound texture then bind our texture temporary
            Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

            // update pixels
			DGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, Position.x, Position.y,
                                     Image.getSize().x, Image.getSize().y,
                                     GL_RGBA, GL_UNSIGNED_BYTE, Image.getPixelsData()));
        }else{
            KD_FPRINT("texture is not created. rname: %s", getName().c_str());
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

    void KTexture::setFilter(TextureFilter Filter){
		setModified(true);
		if (!isInite()) {
			KD_FPRINT("texture not initialized. rname: %s", getName().c_str());
			return;
		}

        if (_ktexId > 0){

            if (Filter != _kfilter){
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // set filter type
                int filterType[] = {GL_NEAREST, GL_LINEAR};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[(U8)Filter]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[(U8)Filter]));
                _kfilter = Filter;
            }
        }else{
            KD_FPRINT("texture is not created. rname: %s", getName().c_str());
        }
    }

    void KTexture::setWrap(TextureWrap Wrap){
		setModified(true);
		if (!isInite()) {
			KD_FPRINT("texture not initialized. rname: %s", getName().c_str());
			return;
		}

        if (_ktexId > 0){

            if (Wrap != _kwrap){
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // setup texture parameters
                int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[(U8)Wrap]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[(U8)Wrap]));
                _kwrap = Wrap;
            }
        }else{
            KD_FPRINT("texture is not created. rname: %s", getName().c_str());
        }
    }

    void KTexture::_create(const U8 *Data, const KVector2U32 &Size,
                        TextureFilter Filter, TextureWrap Wrap, KTexture &Instance){

		if (!Instance.isInite()) {
			KD_FPRINT("texture not initialized. rname: %s", Instance.getName().c_str());
			return;
		}

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
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[(U8)Wrap]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[(U8)Wrap]));
        Instance._kwrap = Wrap;

        int filterType[] = {GL_NEAREST, GL_LINEAR};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[(U8)Filter]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[(U8)Filter]));
        Instance._kfilter = Filter;
    }

	KMETA_KTEXTURE_SOURCE();
}
