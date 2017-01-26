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
#include "Kite/ecs/kresource.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/serialization/kbinaryserial.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KResource::KResource(const std::string &Name, bool CatchStream, bool IsComposite) :
		_kisCatchStream(CatchStream),
		_kisInite(false),
		_kcomposite(IsComposite),
		_kname(Name),
		_kcatchStream(nullptr),
		_kisOnFly(true),
		_kref(0){}

	KResource::~KResource() {
		// free catched stream
		if (_kcatchStream != nullptr) {
			_kcatchStream->close();
			delete _kcatchStream;
		}
	};

	bool KResource::saveStream(KOStream &Stream, const std::string &Address) {
		_kclist.clear();
		if (!_saveStream(Stream, Address)) {
			return false;
		}

		if (isComposite()) {
			if (!saveDepList(Stream, Address)) {
				KD_FPRINT("cant save dep list. resource name: %s", getName().c_str());
				return false;
			}
		}
		return true;
	}

	bool KResource::loadStream(KIStream &Stream, const std::string &Address) {
		if (isComposite()) {
			KD_FPRINT("try to loading composite resource without resource manager. resource name: %s", getName().c_str());
		}

		// catch stream
		if (isCatchStream()) {
			delete _kcatchStream;
			_kcatchStream = Stream.clone();
			if (!_loadStream(*_kcatchStream, Address)) {
				return false;
			}
		} else {
			if (!_loadStream(Stream, Address)) {
				return false;
			}
		}

		return true;
	}

	bool KResource::saveDepList(KOStream &Stream, const std::string &Address) {
		std::vector<std::string> plist; // <name>
		if (!_kclist.empty()) {
			// save dependency key-names
			// we cant save dependency itself 
			// because a depenedency with 2 or more parrent will saved multiple time
			// so we save only informations for loading phase
			for (auto it = _kclist.cbegin(); it != _kclist.cend(); ++it) {
				// catch resources info for saving in next phase
				plist.push_back((*it)->getName());
			}
		} 

		// save resources info
		KBinarySerial serializer;
		serializer << plist;
		if (!serializer.saveStream(Stream, Address + ".dep", 0)) {
			KD_FPRINT("cant save stream. address: %s", Address.c_str());
			return false;
		}
		return true;
	}

	I32 KResource::addDependency(KResource *Resource) {
		if (!_kcomposite) {
			KD_FPRINT("you cant add dependency in a non-composite resource. rname: %s", getName().c_str());
			return -1;
		}

		// nullptr 
		if (!Resource) {
			KD_FPRINT("nullptr passed. rname: %s", getName().c_str());
			return -1;
		}

		// check list for name duplication
		for (auto it = _kclist.begin(); it != _kclist.end(); ++it) {
			if ((*it)->getNameID() == Resource->getNameID()) {
				KD_FPRINT("duplicate detected. rname: %s duplicate: %s", getName().c_str(), Resource->getName().c_str());
				return -1;
			}
		}

		_kclist.push_back(Resource);
		return _kclist.size() - 1;
	}

	KResource *KResource::getDependency(I32 Index) const {
		if (!_kcomposite) {
			KD_FPRINT("this is not a composite resource. rname: %s", getName().c_str());
			return nullptr;
		}

		if (Index < 0 || Index >= _kclist.size()) {
			KD_FPRINT("list is empty or invalid index. rname: %s", getName().c_str());
			return nullptr;
		}

		return _kclist[Index];
	}

	KMETA_KRESOURCE_SOURCE();
}