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
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	U32 Kite::KAtlasTextureArray::_klastTexId = 0;
	KAtlasTextureArray::KAtlasTextureArray(const std::string &Name, const std::string &Address) :
		KResource(Name, Address) ,
		_ktexId(0),
		_kfilter(TextureFilter::LINEAR),
		_kwrap(TextureWrap::CLAMP_TO_EDGE),
		_ksize(KVector2U32(0, 0)),
		_kisCreated(false),
		_kclearImage(false)
	{}

	KAtlasTextureArray::KAtlasTextureArray(const KSharedResource &AtlasImage):
		KResource(),
		_ktexId(0),
		_kfilter(TextureFilter::LINEAR),
		_kwrap(TextureWrap::CLAMP_TO_EDGE),
		_kisCreated(false)
	{
		auto aimage = static_cast<const KAtlasImage *>(AtlasImage.constGet());
		auto image = static_cast<const KImage *>(aimage->getImage().constGet());
		_kimageArray.push_back(AtlasImage);
		_ksize = image->getSize();
	}

	KAtlasTextureArray::~KAtlasTextureArray() {
		if (_ktexId != 0) {
			DGL_CALL(glDeleteTextures(1, &_ktexId));
		}
		if (_klastTexId == _ktexId) {
			_klastTexId = 0;
		}
	}

	KSharedResource KAtlasTextureArray::luaConstruct(const KSharedResource &AtlasImage) {
		return KSharedResource(new KAtlasTextureArray(AtlasImage));
	}

	bool KAtlasTextureArray::saveStream(KIOStream &Stream, const std::string &Address) {
		// check composite resources
		for (auto it = _kimageArray.begin(); it != _kimageArray.end(); ++it) {
			if (it->get()->getResourceName().empty()) {
				KD_PRINT("unregistered composite");
				return false;
			}
		}

		// texture information
		KBinarySerial bserial;
		bserial << std::string("kata");
		bserial << _ksize;
		bserial << _kfilter;
		bserial << _kwrap;
		bserial << _kimageArray.size();

		for (auto it = _kimageArray.begin(); it != _kimageArray.end(); ++it) {
			bserial << it->get()->getResourceName();
		}

		return bserial.saveStream(Stream, Address, 0);
	}

	bool KAtlasTextureArray::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) {
		KBinarySerial bserial;
		if (!bserial.loadStream(*Stream, getAddress())) {
			KD_PRINT("can't load stream");
			return false;
		}

		// check format
		std::string format;
		bserial >> format;
		if (format != "kata") {
			KD_PRINT("incorrect file format");
			Stream->close();
			return false;
		}

		// load info
		bserial >> _ksize;
		bserial >> _kfilter;
		bserial >> _kwrap;

		// load image array
		SIZE itemSize = 0;
		std::string itemName;
		bserial >> itemSize;
		for (SIZE i = 0; i < itemSize; ++i) {
			bserial >> itemName;
			auto item = RManager->load(itemName);
			if (item.isNull()) {
				KD_PRINT("cant load composite resource: resource name: %s", itemName.c_str());
				return false;
			}

			_kimageArray.push_back(item);
		}

		return true;
	}

	bool KAtlasTextureArray::_create() {
		// create pixel buffer
		U8 *pbuf = new U8[(_ksize.x * _ksize.y * 4) * _kimageArray.size()];

		// fill buffer
		U32 offset = 0;
		for (auto it = _kimageArray.begin(); it != _kimageArray.end(); ++it) {
			auto atlas = static_cast<KAtlasImage *>(it->get());
			auto image = static_cast<KImage *>(atlas->getImage().get());

			// check images size before filling buffer
			if (image->getSize() != _ksize) {
				KD_PRINT("incorrect image size: removed from atlas array");
				continue;
			}

			// fill buffer
			memcpy(pbuf + offset, image->getPixelsData(), image->getPixelsDataSize());
			offset += image->getPixelsDataSize();
		}

		// clean up image array
		if (_kclearImage) {
			_kimageArray.clear();
		}

		// generate texture
		if (_ktexId == 0) {
			DGL_CALL(glGenTextures(1, &_ktexId));
		}

		// save currently binded texture then bind our texture temporary
		Internal::GLBindGuard guard(Internal::KBG_TEXTURE_ARRAY, _klastTexId, _ktexId);
		DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _ktexId));

		// allocate the storage.
		//DGL_CALL(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, Size.x, Size.y, Instance->_karray.size())); // opengl 4.2 (dont work in 3.3)
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, _ksize.x, _ksize.y,
					 _kimageArray.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // 3.3 version

		// Upload pixel data.
		// The first 0 refers to the mipmap level (level 0, since there's only 1)
		// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		// The final 0 refers to the layer index offset (we start from index 0).
		// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		DGL_CALL(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, _ksize.x, _ksize.y, _kimageArray.size(),
								 GL_RGBA, GL_UNSIGNED_BYTE, pbuf));

		// setup texture parameters
		int wrapType[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE };
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapType[(U8)_kwrap]));
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapType[(U8)_kwrap]));

		int filterType[] = { GL_NEAREST, GL_LINEAR };
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterType[(U8)_kfilter]));
		DGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterType[(U8)_kfilter]));

		// cleanup buffer
		delete[] pbuf;
		_kisCreated = true;
		return true;
	}

	U32 KAtlasTextureArray::addItem(const KSharedResource &AtlasImage) {
		auto id = _kimageArray.size();
		_kimageArray.push_back(AtlasImage);
		return id;
	}

	KSharedResource KAtlasTextureArray::getItem(U32 ID) const {
		if (ID < _kimageArray.size()) {
			return _kimageArray[ID];
		}
		KD_PRINT("id is out of range");
		return KSharedResource();
	}

	void KAtlasTextureArray::clearItems() {
		_kimageArray.clear();
	}

	void KAtlasTextureArray::setFilter(TextureFilter Filter) {
		if (_kisCreated) {
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
		if (_kisCreated) {
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

	void KAtlasTextureArray::setCleanupImage(bool CleanUp) {
		if (CleanUp && _kisCreated) {
			_kimageArray.clear();
		}

		_kclearImage = CleanUp;
	}

	bool KAtlasTextureArray::bind() {
		// create texture only first time
		if (!_kisCreated) {
			if (!_create()) {
				KD_PRINT("can't initialize atlas texture array");
				return false;
			}
		}

		// bind texture
		if (_klastTexId != _ktexId) {
			// bind texture
			DGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _ktexId));
			_klastTexId = _ktexId;
		}

		return true;
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

	KATLASTEXTUREARRAY_SOURCE();
}