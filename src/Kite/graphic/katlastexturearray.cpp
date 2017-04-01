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
#include "Kite/graphic/katlastexturearray.h"
#include "src/Kite/graphic/glcall.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/core/kfostream.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	U32 Kite::KAtlasTextureArray::_klastTexId = 0;
	KAtlasTextureArray::KAtlasTextureArray(const std::string &Name) :
		KResource(Name, false, true) ,
		_ktexId(0),
		_kfilter(TextureFilter::LINEAR),
		_kwrap(TextureWrap::CLAMP_TO_EDGE),
		_ksize(KVector2U32(0, 0))
	{}

	KAtlasTextureArray::~KAtlasTextureArray() {
		if (_ktexId != 0) {
			DGL_CALL(glDeleteTextures(1, &_ktexId));
		}
		if (_klastTexId == _ktexId) {
			_klastTexId = 0;
		}
	}


	bool KAtlasTextureArray::_saveStream(KOStream &Stream, const std::string &Address) {
		// texture information
		KBinarySerial bserial;

		bserial << std::string("KATexArr");
		bserial << _ksize;
		bserial << _kfilter;
		bserial << _kwrap;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}
		Stream.close();

		// save composite list (texture)
		std::vector<KResource *> composite;
		for (auto it = _karray.begin(); it != _karray.end(); ++it) {
			composite.push_back((KResource *)(*it));
		}
		setCompositeList(composite);

		return true;
	}

	bool KAtlasTextureArray::_loadStream(KIStream &Stream, const std::string &Address) {
		// load texture info 
		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_PRINT("can't load stream");
			Stream.close();
			return false;
		}
		std::string format;

		bserial >> format;

		if (format != "KATexArr") {
			KD_PRINT("incorrect file format");
			Stream.close();
			return false;
		}

		bserial >> _ksize;
		bserial >> _kfilter;
		bserial >> _kwrap;

		Stream.close();

		// load composite resources (texture)
		_karray.clear();
		auto slist = getCompositeList();
		for (auto it = slist.begin(); it != slist.end(); ++it) {
			_karray.push_back((KAtlasTexture *)(*it));
		}

		return true;
	}

	bool KAtlasTextureArray::inite() {
		if (isInite()) {
			return true;
		}
		bool ret = true;

		// generate texture
		DGL_CALL(glGenTextures(1, &_ktexId));

		// create pixel buffer
		U8 *pbuf = new U8[(_ksize.x * _ksize.y * 4) * _karray.size()];

		U32 offset = 0;
		KImage image;
		for (auto it = _karray.begin(); it != _karray.end(); ++it) {
			if ((*it)->getTexture()->getSize() != _ksize) {
				ret = false;
				KD_PRINT("texture size is not match with array size");
				break;
			}

			(*it)->getTexture()->getImage(image);
			memcpy(pbuf + offset, image.getPixelsData(), image.getPixelsDataSize());
			offset += image.getPixelsDataSize();
		}

		// transfer pixels data from ram to vram (opengl texture array)
		if (ret) {
			_create(pbuf, _ksize, _kfilter, _kwrap, this);
		}

		// cleanup buffer
		delete[] pbuf;

		setInite(ret);
		return ret;
	}

	void KAtlasTextureArray::_create(const U8 *Data, const KVector2U32 &Size,
						   TextureFilter Filter, TextureWrap Wrap, KAtlasTextureArray *Instance) {

		// save currently binded texture then bind our texture temporary
		Internal::GLBindGuard guard(Internal::KBG_TEXTURE_ARRAY, _klastTexId, Instance->_ktexId);
		DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, Instance->_ktexId));

		// allocate the storage.
		//DGL_CALL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, Size.x, Size.y, Instance->_karray.size())); // opengl 4.2 (dont work in 3.3)
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, Size.x, Size.y,
					 Instance->_karray.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // 3.3 version

		// Upload pixel data.
		// The first 0 refers to the mipmap level (level 0, since there's only 1)
		// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		// The final 0 refers to the layer index offset (we start from index 0).
		// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		DGL_CALL(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, Size.x, Size.y, Instance->_karray.size(),
								 GL_RGBA, GL_UNSIGNED_BYTE, Data));

		// setup texture parameters
		int wrapType[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE };
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType[(U8)Wrap]));
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType[(U8)Wrap]));

		int filterType[] = { GL_NEAREST, GL_LINEAR };
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterType[(U8)Filter]));
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterType[(U8)Filter]));
	}

	void KAtlasTextureArray::setTextureSize(const KVector2U32 &Size) {
		if (Size != _ksize) {
			_ksize = Size;
			_karray.clear();
		}
	}

	bool KAtlasTextureArray::addItem(KAtlasTexture *Atlas) {
		if (Atlas->getTexture()) {
			if (Atlas->getTexture()->getSize() == _ksize) {
				_karray.push_back(Atlas);
				return true;
			}
		}
		return false;
	}

	void KAtlasTextureArray::setFilter(TextureFilter Filter) {
		if (_ktexId > 0) {
			if (Filter != _kfilter) {
				// save currently bound texture then bind our texture temporary
				Internal::GLBindGuard guard(Internal::KBG_TEXTURE_ARRAY, _klastTexId, _ktexId);
				DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _ktexId));

				// set filter type
				int filterType[] = { GL_NEAREST, GL_LINEAR };
				DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterType[(U8)Filter]));
				DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterType[(U8)Filter]));
			}
		}
		_kfilter = Filter;
	}

	void KAtlasTextureArray::setWrap(TextureWrap Wrap) {
		if (_ktexId > 0) {
			if (Wrap != _kwrap) {
				// save currently bound texture then bind our texture temporary
				Internal::GLBindGuard guard(Internal::KBG_TEXTURE_ARRAY, _klastTexId, _ktexId);
				DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _ktexId));

				// setup texture parameters
				int wrapType[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE };
				DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType[(U8)Wrap]));
				DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType[(U8)Wrap]));
			}
		}
		_kwrap = Wrap;
	}

	void KAtlasTextureArray::bind() const {
		// bind texture
		if (_klastTexId != _ktexId) {
			DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _ktexId));
			_klastTexId = _ktexId;
		}
	}

	void KAtlasTextureArray::unbind() {
		if (_klastTexId == _ktexId) {
			DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
			_klastTexId = 0;
		}
	}

	void KAtlasTextureArray::unbindTextureArray() {
		if (_klastTexId != 0) {
			DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
			_klastTexId = 0;
		}
	}

	KMETA_KATLASTEXTUREARRAY_SOURCE();
}