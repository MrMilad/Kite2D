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
#include "Kite/graphic/katlasimage.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KAtlasImage::KAtlasImage(const KSharedResource &Image):
		KResource(),
		_kimage(Image)
	{
		auto image = static_cast<const KImage *>(Image.constGet());
		// create default item (same size as image)
		KAtlasItem def;
		def.width = image->getWidth();
		def.height = image->getHeight();
		def.id = 0;
		def.blu = 0;
		def.blv = 0;
		def.tru = 1;
		def.trv = 1;
		def.xpos = 0;
		def.ypos = 0;
		_kitems.push_back(def);
	}

	KAtlasImage::KAtlasImage(const std::string &Name, const std::string &Address) :
		KResource(Name, Address),
		_kimage(nullptr)
	{}

	KSharedResource KAtlasImage::luaConstruct(const KSharedResource &Image) {
		return KSharedResource(new KAtlasImage(Image));
	}

	bool KAtlasImage::saveStream(KIOStream &Stream, const std::string &Address) {
		// check composite resources
		if (_kimage->getResourceName().empty()) {
			KD_PRINT("unregistered composite");
			return false;
		}

		// save data
		KBinarySerial bserial;
		bserial << std::string("kati");
		bserial << _kitems;
		bserial << _kimage->getResourceName();

		return bserial.saveStream(Stream, Address, 0);
	}

	bool KAtlasImage::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) {
		KBinarySerial bserial;
		if (!bserial.loadStream(*Stream, getAddress())) {
			KD_PRINT("can't load stream");
			Stream->close();
			return false;
		}
		std::string format;
		bserial >> format;

		// check format
		if (format != "kati") {
			KD_PRINT("incorrect file format");
			Stream->close();
			return false;
		}

		// load items
		bserial >> _kitems;

		// load image
		std::string iname;
		bserial >> iname;
		_kimage = RManager->load(iname);

		if (_kimage.isNull()) {
			KD_FPRINT("can't load composite resource: resource name: %s", iname.c_str());
			return false;
		}

		return true;
	}

	U32 KAtlasImage::addItem(const KAtlasItem &Item) {
		_kitems.push_back(Item);
		_kitems.back().id = _kitems.size() - 1;
		return _kitems.size() - 1;
	}

	KAtlasItem KAtlasImage::getItem(U32 ID) const {
		if (ID < _kitems.size()) {
			return _kitems[ID];
		}
		KD_FPRINT("there is no atlas item with this id: id: ", std::to_string(ID).c_str());
		return KAtlasItem();
	}

	void KAtlasImage::clearItems() {
		_kitems.clear();
	}

	KATLASIMAGE_SOURCE();
}
