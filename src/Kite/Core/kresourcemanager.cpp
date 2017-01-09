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
#include "kmeta.khgen.h"
#include <luaintf\LuaIntf.h>

namespace Kite {
	KResourceManager::KResourceManager(){
		// initialize factory array
		initeRFactory();

		// register resource types and stream types
		registerRTypes(this);
	}

	KResourceManager::~KResourceManager(){
		clear();
	}

	void KResourceManager::initeRFactory() {
		for (SIZE i = 0; i < (SIZE)RTypes::maxSize; ++i) {
			_krfactory[i] = nullptr;
		}
	}

	void KResourceManager::registerIStream(IStreamTypes SType, KIStream *(*Func)()) {
		auto index = (SIZE)SType;
		_ksfactory[index] = Func;
	}

	void KResourceManager::registerResource(RTypes RType, KResource *(*Func)(const std::string &)) {
		auto index = (SIZE)RType;
		_krfactory[index] = Func;
	}

	KResource *KResourceManager::create(RTypes Type, const std::string &Name, bool OnFly) {
		auto index = (SIZE)Type;

		if (get(Name) && !OnFly) {
			KD_FPRINT("this name is already exixst. rname: %s", Name.c_str());
			return nullptr;
		}

		// create new resource and return it
		auto newRes = _krfactory[index](Name);

		// return on fly
		if (OnFly) { return newRes; }

		// increment refrence count
		newRes->incRef();

		// create key 
		std::string tempKey(Name);
		std::transform(Name.begin(), Name.end(), tempKey.begin(), ::tolower);

		// insert resource to address/map
		_kmap.insert({ tempKey, newRes });

		// post a message about new resource
		KMessage msg;
		msg.setType("RES_CREATED");
		msg.setData((void *)newRes, sizeof(newRes));
		postMessage(&msg, MessageScope::ALL);

		return newRes;
	}

	KResource *KResourceManager::load(IStreamTypes SType, RTypes RType,
									  const std::string &Address, bool OnFly){
		
		auto sindex = (SIZE)SType;
		auto rindex = (SIZE)RType;

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
		if (!OnFly) {
			std::transform(ResName.begin(), ResName.end(), tempKey.begin(), ::tolower);

			// first, check our resource catch
			auto found = _kmap.find(tempKey);
			if (found != _kmap.end()) {
				found->second->incRef();
				return found->second;
			}
		}

		// create stream
		auto stream = _ksfactory[sindex]();
		if (stream == nullptr) {
			KD_FPRINT("can't create stream. stream type: %s", getIStreamTypesName(SType).c_str());
			return nullptr;
		}

		// retrieving file name and path
		KFileInfo finfo;
		stream->getFileInfoStr(ResName, finfo);

		// create new resource and assocated input stream
		KResource *resource = _krfactory[rindex](finfo.name);

		// loading composite resources (recursive)
		if (resource->isComposite()) {
			if (!loadCompositeList(SType, resource, ResName, OnFly)) {
				KD_FPRINT("can't load composite resource. rname: %s", ResName.c_str());
				delete resource;
				delete stream;
				return nullptr;
			}
		}

		if (!resource->_loadStream(*stream, ResName)) {
			KD_FPRINT("can't load resource. rname: %s", ResName.c_str());
			delete resource;
			delete stream;
			return nullptr;
		}
		resource->setAddress(finfo.path);

		// stream lifetime
		std::pair<KResource *, KIStream *> pair;
		if (resource->isCatchStream()) {
			resource->setCatchStream(stream);
		} else {
			stream->close();
			delete stream;
		}

		// increment refrence count
		resource->incRef();

		if (!OnFly) {
			// insert resource to address/map
			_kmap.insert({ tempKey, resource });

			// insert resource to name dictionary (will override last address with same name)
			_kdict.insert({ finfo.name, finfo.fullPath });


			// post a message about new resource
			KMessage msg;
			msg.setType("RES_LOAD");
			msg.setData((void *)resource, sizeof(resource));
			postMessage(&msg, MessageScope::ALL);
		}

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
			return found->second;
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

			// uload dependency
			/*if (found->second.first->isComposite()) {
				unloadCompositeList(found->second.first);
			}*/

			// dec ref counter
			found->second->decRef();

			// check resource count
			if (found->second->getReferencesCount() == 0 || Immediately) {

				// post a message about unloaded resource
				KMessage msg;
				msg.setType("RES_UNLOAD");
				msg.setData((void *)found->second, sizeof(found->second));
				postMessage(&msg, MessageScope::ALL);

				auto type = found->second->getType();

				// free resource
				// associated stream (catched) will deleted in resource destructure
				delete found->second;

				// erase pair from map
				// we dont need remove it from name\dictionary
				_kmap.erase(found);

				// send msg about this
				msg.setType("RES_UNLOADED");
				msg.setData((void *)&type, sizeof(RTypes));
				postMessage(&msg, MessageScope::ALL);
			}
		} 
	}

	void KResourceManager::free(KResource *Resource) {
		delete Resource;
	}

	const std::vector<KResource *> &KResourceManager::dump() {
		static std::vector<KResource *> res;
		res.clear();
		res.reserve(_kmap.size());

		for (auto it = _kmap.cbegin(); it != _kmap.cend(); ++it) {	
			res.push_back(it->second);
		}

		return res;
	}

	bool KResourceManager::saveDictionary(KOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << _kdict;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_FPRINT("cant save resource dictionary. address: %s", Address.c_str());
			return false;
		}

		return true;
	}

	bool KResourceManager::loadDictionary(KIStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_FPRINT("cant load resource dictionary. address: %s", Address.c_str());
			return false;
		}

		bserial >> _kdict;

		return true;
	}

	void KResourceManager::clear() {
		// for avoiding pointer dangling and circular-iteration we post message about removing all resource(s) at first
		for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
			// post a message about unloaded resource
			KMessage msg;
			msg.setType("RES_UNLOAD");
			msg.setData((void *)it->second, sizeof(it->second));
			postMessage(&msg, MessageScope::ALL);
		}

		for (auto it = _kmap.begin(); it != _kmap.end(); ++it) {
			// free resource
			// associated stream (catched) will deleted in resource destructure
			delete it->second;
		}

		// clear map
		_kmap.clear();
		_kdict.clear();
	}

	bool KResourceManager::loadCompositeList(IStreamTypes SType, KResource *Res, const std::string &Address, bool OnFly) {
		std::vector<std::pair<std::string, RTypes>> plist;
		KBinarySerial serializer;
		auto sindex = (SIZE)SType;
		auto stream = _ksfactory[sindex]();
		if (!serializer.loadStream(*stream, Address + ".dep")) {
			KD_FPRINT("cant load resource composite list. rname: %s", Res->getName().c_str());
			delete stream;
			return false;
		}
		serializer >> plist;
		Res->_kclist.clear();

		// retrieving file name and path
		KFileInfo finfo;
		stream->getFileInfoStr(Address, finfo);
		delete stream;

		// loading composite resources
		for (auto it = plist.begin(); it != plist.end(); ++it) {

			// case1: first we try load composite resources based our name\address dictionary
			auto comp = load(SType, it->second, it->first, OnFly);
			if (comp == nullptr) {

				// case 2: then we try load it based parrent address
				comp = load(SType, it->second, finfo.path + "/" + it->first, OnFly);
				if (comp == nullptr) {
					KD_FPRINT("cant load composite resource. composite name: %s type: %s", it->first.c_str(), getRTypesName(it->second).c_str());
					return false;
				}
			}
			Res->_kclist.push_back(comp);
		}

		return true;
	}

	KMETA_KRESOURCEMANAGER_SOURCE();
}