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
#include "Kite/graphic/katlastexture.h"
#include "src/Kite/graphic/glcall.h"
#include "Kite/graphic/katlasimage.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
    U32 Kite::KAtlasTexture::_klastTexId = 0;
	KAtlasTexture::KAtlasTexture(const KSharedResource &AtlasImage) :
		KResource(),
		_kclearImage(false),
		_ktexId(0),
		_kfilter(TextureFilter::LINEAR),
		_kwrap(TextureWrap::CLAMP_TO_EDGE),
		_kisCreated(false),
		_kimage(AtlasImage)
	{
		auto atlas = static_cast<KAtlasImage *>(_kimage.get());
		auto image = static_cast<KImage *>(atlas->getImage().get());

		_ksize = image->getSize();
	}

	KAtlasTexture::KAtlasTexture(const std::string &Name, const std::string &Address) :
		KResource(Name, Address),
		_kclearImage(false),
        _ktexId(0),
        _kfilter(TextureFilter::LINEAR),
        _kwrap(TextureWrap::CLAMP_TO_EDGE),
        _ksize(KVector2U32(0,0)),
		_kisCreated(false)
    {}

	KSharedResource luaConstruct(const KSharedResource &AtlasImage) {
		return KSharedResource(new KAtlasTexture(AtlasImage));
	}

	KAtlasTexture::~KAtlasTexture(){
		if (_ktexId != 0) {
			DGL_CALL(glDeleteTextures(1, &_ktexId));
		}
        if (_klastTexId == _ktexId){
            _klastTexId = 0;
        }
    }

	bool KAtlasTexture::saveStream(KIOStream &Stream, const std::string &Address) {
		// check composite resource
		if (_kimage->getResourceName().empty()) {
			KD_PRINT("unregistered composite");
			return false;
		}

		// save data
		KBinarySerial bserial;
		bserial << std::string("kate");
		bserial << _kfilter;
		bserial << _kwrap;
		bserial << _ksize;
		bserial << _kimage->getResourceName();

		return bserial.saveStream(Stream, Address, 0);
	}

	bool KAtlasTexture::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) {
		KBinarySerial bserial;
		if (!bserial.loadStream(*Stream, getAddress())) {
			KD_PRINT("can't load stream");
			return false;
		}

		std::string format;
		bserial >> format;
		if (format != "kate") {
			KD_PRINT("incorrect file format");
			return false;
		}

		bserial >> _kfilter;
		bserial >> _kwrap;
		bserial >> _ksize;

		// load atlas image
		std::string iname;
		bserial >> iname;
		_kimage = RManager->load(iname);
		if (_kimage.isNull()) {
			KD_FPRINT("cant load composite resource: resource name: %s", iname.c_str());
			return false;
		}

		return true;
	}

	KSharedResource KAtlasTexture::getTextureImage() const{
		// online pixel data (opengl side)
		if (_kisCreated) {
			// save currently binded texture then bind our texture temporary
			Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
			DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

			U8 *pdata = new U8[_ksize.x * _ksize.y];
			DGL_CALL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA8, GL_UNSIGNED_BYTE, (GLvoid *)pdata));
			KSharedResource sh(new KImage(_ksize.x, _ksize.y, pdata));
			delete[] pdata;
			return sh;
		// offline pixel data (memory side)
		} else {
			auto atlas = static_cast<const KAtlasImage *>(_kimage.constGet());
			return atlas->getImage();
		}
	}

    void KAtlasTexture::update(const KSharedResource &Image, const KVector2U32 &Position){
		if (!_kisCreated) {
			KD_FPRINT("texture is not created");
		}

		auto image = static_cast<const KImage *>(Image.constGet());
        // check new image size with texture size
		if ((Position.x + image->getWidth()) > _ksize.x ||
			(Position.y + image->getHeight()) > _ksize.y) 
		{
			KD_PRINT("image position/size is out of range");
			return;
		}

        // save currently bound texture then bind our texture temporary
        Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

        // update pixels
		DGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, Position.x, Position.y,
			image->getWidth(), image->getHeight(),
			GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsData()));
    }

    bool KAtlasTexture::bind(){
		// create texture only first time
		if (!_kisCreated) {
			if (!_create()) {
				KD_PRINT("can't initialize atlas texture");
				return false;
			}
		}
			
        // bind texture
        if (_klastTexId != _ktexId){
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));
            _klastTexId = _ktexId;
        }

		return true;
    }

    void KAtlasTexture::unbind(){
        if (_klastTexId == _ktexId){
            DGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
            _klastTexId = 0;
        }
    }

    void KAtlasTexture::unbindTexture(){
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        _klastTexId = 0;
    }

    void KAtlasTexture::setFilter(TextureFilter Filter){
        if (_ktexId > 0){
            if (Filter != _kfilter){
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // set filter type
                int filterType[] = {GL_NEAREST, GL_LINEAR};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[(U8)Filter]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[(U8)Filter]));
            }
        }
		_kfilter = Filter;
    }

    void KAtlasTexture::setWrap(TextureWrap Wrap){
        if (_kisCreated){
            if (Wrap != _kwrap){
                // save currently bound texture then bind our texture temporary
                Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
                DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

                // setup texture parameters
                int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[(U8)Wrap]));
                DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[(U8)Wrap]));
            }
        }
		_kwrap = Wrap;
    }

	void KAtlasTexture::setCleanupImage(bool CleanUp) {
		if (CleanUp && _kisCreated) {
			_kimage = KSharedResource();
		}

		_kclearImage = CleanUp;
	}

    bool KAtlasTexture::_create(){
		auto atlas = static_cast<KAtlasImage *>(_kimage.get());
		auto image = static_cast<KImage *>(atlas->getImage().get());
		_ksize = image->getSize();

		// generate texture
		if (_ktexId == 0) {
			DGL_CALL(glGenTextures(1, &_ktexId));
		}

        // save currently binded texture then bind our texture temporary
		Internal::GLBindGuard guard(Internal::KBG_TEXTURE, _klastTexId, _ktexId);
        DGL_CALL(glBindTexture(GL_TEXTURE_2D, _ktexId));

        // fill texture with image pixel data
        DGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _ksize.x, _ksize.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsData()));

        // setup texture parameters
        int wrapType[] = {GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType[(U8)_kwrap]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType[(U8)_kwrap]));

        int filterType[] = {GL_NEAREST, GL_LINEAR};
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType[(U8)_kfilter]));
        DGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType[(U8)_kfilter]));

		// cleanup image
		if (_kclearImage) {
			_kimage = KSharedResource();
		}

		_kisCreated = true;
		return true;
    }

	KATLASTEXTURE_SOURCE();
}
