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
#include "Kite/Core/utility/kmemorystream.h"
#include <algorithm>

namespace Kite{
	void KMemoryStream::addSection(const std::string &SectionName, void *Data, size_t DataSize){
		if (SectionName.empty() || !Data || DataSize <= 0){
			KDEBUG_PRINT("invalid section");
			return;
		}

		// convert section name to lower case to using for key
		std::string tempKey = SectionName;
		std::transform(SectionName.begin(), SectionName.end(), tempKey.begin(), ::tolower);

		std::pair<void *, size_t> mem(Data, DataSize);
		_kmap.insert({ tempKey, mem });
	}

	void KMemoryStream::removeSection(const std::string &SectionName){
		if (SectionName.empty()){
			KDEBUG_PRINT("invalid section");
			return;
		}

		// convert section name to lower case to using for key
		std::string tempKey = SectionName;
		std::transform(SectionName.begin(), SectionName.end(), tempKey.begin(), ::tolower);

		_kmap.erase(tempKey);
	}

	void KMemoryStream::clear(){
		_kmap.clear();
	}

	KInputStream *KMemoryStream::openRead(const std::string &Name){

		// convert section name to lower case to using for key
		std::string tempKey = Name;
		std::transform(Name.begin(), Name.end(), tempKey.begin(), ::tolower);

		std::unordered_map<std::string, std::pair<void *, size_t>>::iterator found = _kmap.find(tempKey);
		if (found != _kmap.end())
			return new KMemInputStream(found->second.first, found->second.second);

		return 0;
	}
}