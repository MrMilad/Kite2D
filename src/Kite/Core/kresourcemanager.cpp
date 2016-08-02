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
#include "Kite/core/kresourcemanager.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdumap.h"
#include "Kite/serialization/types/kstdpair.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KResourceManager::KResourceManager(){}

	KResourceManager::~KResourceManager(){
		clear();
	}

	bool KResourceManager::registerIStream(const std::string &SType, KIStream *(*Func)()) {
		auto found = _ksfactory.find(SType);
		if (found != _ksfactory.end()) {
			KD_FPRINT("this type has already been registered. stype: %s", SType.c_str());
			return false;
		}

		_ksfactory.insert({ SType, Func });
		return true;
	}

	bool KResourceManager::registerResource(const std::string &RType, KResource *(*Func)(const std::string &), bool CatchStream) {
		auto found = _krfactory.find(RType);
		if (found != _krfactory.end()) {
			KD_FPRINT("this type has already been registered. rtype: %s", RType.c_str());
			return false;
		}

		_krfactory.insert({ RType, {Func, CatchStream}});
		return true;
	}

	KResource *KResourceManager::create(const std::string &RType, const std::string &Name) {
		auto factory = _krfactory.find(RType);
		if (factory == _krfactory.end()) {
			KD_FPRINT("unregistered resource type. rtype: %s", RType.c_str());
			return nullptr;
		}

		// create new resource and return it
		return factory->second.first(Name);
	}

	KResource *KResourceManager::load(const std::string &SType, const std::string &RType,
									  const std::string &Address){
		// check for stream factory methode
		auto sfactory = _ksfactory.find(SType);
		if (sfactory == _ksfactory.end()) {
			KD_FPRINT("unregistered stream type. rtype: %s", RType.c_str());
			return nullptr;
		}

		// check for resource factory methode
		auto rfactory = _krfactory.find(RType);
		if (rfactory == _krfactory.end()) {
			KD_FPRINT("unregistered resource type. rtype: %s", RType.c_str());
			return nullptr;
		}

		bool CatchStream = rfactory->second.second;

		// checking file name
		if (Address.empty()) {
			KD_PRINT("empty Address is not valid");
			return nullptr;
		}

		// first check our dictionary
		bool isOnDict = false;
		std::string ResName = Address;
		auto dfound = _kdict.find(Address);

		// using dictionary key
		if (dfound != _kdict.end()) {
			isOnDict = true;
			ResName = dfound->second;
		}

		// create key from file name
		// always we save original file address (not name) as a key
		std::string tempKey(ResName);
		std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

		// first, check our resource catch
		auto found = _kmap.find(tempKey);
		if (found != _kmap.end()) {
			found->second.first->incRef();
			return found->second.first;
		}

		// create stream
		auto stream = sfactory->second();
		if (stream == nullptr) {
			KD_FPRINT("can't create stream. stream type: %s", SType.c_str());
			return nullptr;
		}

		// retrieving file name and path
		KFileInfo finfo;
		stream->getFileInfoStr(ResName, finfo);

		// create new resource and assocated input stream
		KResource *resource = rfactory->second.first(finfo.name);
		resource->setModified(true);

		// loading composite resources (recursive)
		if (resource->isComposite()) {
			if (!loadCompositeList(resource, stream, ResName)) {
				KD_FPRINT("can't load composite resource. rname: %s", ResName.c_str());
				delete resource;
				delete stream;
				return nullptr;
			}
		}

		if (!resource->_loadStream(stream, ResName)) {
			KD_FPRINT("can't load resource. rname: %s", ResName.c_str());
			delete resource;
			delete stream;
			return nullptr;
		}
		resource->setAddress(finfo.path);

		// stream lifetime
		std::pair<KResource *, KIStream *> pair;
		if (CatchStream) {
			pair = std::make_pair(resource, stream);
		} else {
			pair = std::make_pair(resource, nullptr);
			stream->close();
			delete stream;
		}

		// increment refrence count
		resource->incRef();

		// insert resource to address/map
		_kmap.insert({ tempKey, pair });

		// insert resource to name dictionary (will override last address with same name)
		_kdict.insert({ finfo.name, finfo.fullPath });

		// post a message about new resource
		KMessage msg;
		msg.setType("RES_LOAD");
		msg.setData((void *)resource, sizeof(resource));
		postMessage(&msg, MessageScope::ALL);

		return resource;
	}

	/*bool KResourceManager::add(const std::string &ResName, KResource *Resource, KIStream *CatchStream) {
		// checking file name
		if (ResName.empty()) {
			KD_PRINT("empty resource name is not valid");
			return false;
		}

		if (!Resource) {
			KD_FPRINT("bad or null resource pointer. rname: %s", ResName.c_str());
			return false;
		}

		// create key from file name
		std::string tempKey(ResName);
		std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

		// first, check our resource catch
		std::unordered_map<std::string, std::pair<KResource *, KIStream *>>::iterator found = _kmap.find(tempKey);
		if (found != _kmap.end()) {
			// replace current loaded resource not allowed
			KD_FPRINT("replace loaded resource not allowed. rname: %s", ResName.c_str());
			return false;
		}

		// stream lifetime
		std::pair<KResource *, KIStream *> pair;
		if (CatchStream) {
			pair = std::make_pair(Resource, CatchStream);
		} else {
			pair = std::make_pair(Resource, (KIStream *)nullptr);
		}

		// storing resource
		_kmap.insert({ tempKey, pair });
		return true;
	}*/

	KResource *KResourceManager::get(const std::string &Name) {
		// checking file name
		if (Name.empty()) {
			KD_PRINT("empty resource name is not valid");
			return nullptr;
		}

		// first check our dictionary
		std::string ResName = Name;

		auto dfound = _kdict.find(ResName);
		// using dictionary key
		if (dfound != _kdict.end()) {
			ResName = dfound->second;
		}

		// create key from file name
		std::string tempKey(ResName);
		std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

		// check our resource catch
		auto found = _kmap.find(tempKey);
		if (found != _kmap.end()) {
			return found->second.first;
		}

		KD_FPRINT("there is no resource with the given name. rname: %s", Name.c_str());
		return nullptr;
	}

	void KResourceManager::unload(const std::string &Name, bool Immediately) {
		// checking file name
		if (Name.empty()) {
			KD_PRINT("empty resource name is not valid");
			return;
		}

		// first check our dictionary
		std::string ResName = Name;
		
		auto dfound = _kdict.find(ResName);
		// using dictionary key
		if (dfound != _kdict.end()) {
			ResName = dfound->second;
		}


		// create key from file name
		std::string tempKey = ResName;
		std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

		// check resource catch
		auto found = _kmap.find(tempKey);
		if (found != _kmap.end()) {
			found->second.first->decRef();

			// check resource count
			if (found->second.first->getReferencesCount() == 0 || Immediately) {

				// post a message about unloaded resource
				KMessage msg;
				msg.setType("RES_UNLOAD");
				msg.setData((void *)found->second.first, sizeof(found->second.first));
				postMessage(&msg, MessageScope::ALL);

				// free resource
				delete found->second.first;

				// free associated input stream 
				if (found->second.second != nullptr) {
					found->second.second->close();
					delete found->second.second;
				}

				// erase pair from catch
				// we dont need remove it from name\dictionary
				_kmap.erase(found);
			}
		}
	}

	const std::vector<KResource *> &KResourceManager::dump() {
		static std::vector<KResource *> res;
		res.clear();
		res.reserve(_kmap.size());

		for (auto it = _kmap.cbegin(); it != _kmap.cend(); ++it) {	
			res.push_back(it->second.first);
		}

		return res;
	}

	bool KResourceManager::saveDictionary(KOStream *Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << _kdict;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_FPRINT("cant save resource dictionary. address: %s", Address.c_str());
			return false;
		}

		return true;
	}

	bool KResourceManager::loadDictionary(KIStream *Stream, const std::string &Address) {
		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_FPRINT("cant load resource dictionary. address: %s", Address.c_str());
			return false;
		}

		bserial >> _kdict;

		return true;
	}

	const std::vector<std::string> &KResourceManager::getRegisteredTypes() {
		static std::vector<std::string> rlist;
		rlist.clear();
		rlist.reserve(_krfactory.size());

		for (auto it = _krfactory.begin(); it != _krfactory.end(); ++it) {
			rlist.push_back(it->first);
		}

		return rlist;
	}

	bool KResourceManager::isRegiteredType(const std::string &Type) {
		if (_krfactory.find(Type) != _krfactory.end()) {
			return true;
		}
		return false;
	}

	void KResourceManager::clear() {
		for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
			// post a message about unloaded resource
			KMessage msg;
			msg.setType("RES_UNLOAD");
			msg.setData((void *)it->second.first, sizeof(it->second.first));
			postMessage(&msg, MessageScope::ALL);

			// free resource
			delete it->second.first;

			// free associated input stream 
			if (it->second.second != nullptr) {
				it->second.second->close();
				delete it->second.second;
			}
		}

		// clear map
		_kmap.clear();
		_kdict.clear();
	}

	bool KResourceManager::loadCompositeList(KResource *Res, KIStream *Stream, const std::string &Address, U32 Flag) {
		std::vector<std::pair<std::string, std::string>> plist;
		KBinarySerial serializer;
		if (!serializer.loadStream(Stream, Address + ".dep")) {
			KD_FPRINT("cant load resource composite list. rname: %s", Res->getName().c_str());
			return false;
		}
		serializer >> plist;
		Res->_kclist.clear();

		// retrieving file name and path
		KFileInfo finfo;
		Stream->getFileInfoStr(Address, finfo);

		// loading coposite resources
		for (auto it = plist.begin(); it != plist.end(); ++it) {

			// case1: first we try load composite resources based our name\address dictionary
			auto comp = load(Stream->getType(), it->second, it->first);
			if (comp == nullptr) {

				// case 2: then we try load it based parrent address
				comp = load(Stream->getType(), it->second, finfo.path + "\\" + it->first);
				if (comp == nullptr) {
					KD_FPRINT("cant load composite resource. composite name: %s", it->first.c_str());
					return false;
				}
			}
			Res->_kclist.push_back(comp);
		}

		return true;
	}

	KMETA_KRESOURCEMANAGER_SOURCE();
}