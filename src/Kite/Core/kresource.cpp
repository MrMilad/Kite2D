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
#include "Kite/core/kresource.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/serialization/kbinaryserial.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KResource::KResource(const std::string &Name, bool IsComposite) :
		_kisModified(false),
		_kisInite(false),
		_kcomposite(IsComposite),
		_kname(Name),
		_kref(0){}

	KResource::~KResource() {};

	bool KResource::saveStream(KOStream *Stream, const std::string &Address, bool SaveDependency) {
		if (!_saveStream(Stream, Address)) {
			return false;
		}

		if (isComposite()) {
			if (!saveCompositeList(Stream, Address, SaveDependency)) {
				KD_PRINT("cant save composite resource list.");
				return false;
			}
		}
		return true;
	}

	bool KResource::saveCompositeList(KOStream *Stream, const std::string &Address, bool SaveDependency) {
		if (!_kclist.empty()) {
			// address info
			KFileInfo finfo;
			Stream->getFileInfoStr(Address, finfo);

			// save composite infomations (name and type)
			std::vector<std::pair<std::string, std::string>> plist;
			for (auto it = _kclist.cbegin(); it != _kclist.cend(); ++it) {
				if ((*it) != nullptr) {

					// save resource itself
					if (SaveDependency) {
						if (!(*it)->saveStream(Stream, finfo.path + "\\" + (*it)->getName())) {
							KD_FPRINT("cant save composite resource. rname: %s", (*it)->getName().c_str());
							return false;
						}
					}

					// save resource info
					plist.push_back({ (*it)->getName(), (*it)->getType() });
				} else {
					KD_FPRINT("warning: an nullptr composite resource detected. rname: %s", getName().c_str());
				}
			}

			KBinarySerial serializer;
			serializer << plist;
			if (!serializer.saveStream(Stream, Address + ".dep", 0)) {
				KD_FPRINT("cant save stream. address: %s", Address.c_str());
				return false;
			}

			return true;
		} 

		KD_FPRINT("save composite called but composite list is empty. rname: %s", getName().c_str());
		return false;
	}

	KMETA_KRESOURCE_SOURCE();
}