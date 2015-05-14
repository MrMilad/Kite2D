/*
Kite2D Game Framework.
Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Kite/core/system/kresources.h"
#include "Kite/Core/system/ksystemutil.h"

namespace Kite{
	U32 KResources::_kinstances[7];
	std::unordered_map<U64, const KResources *> KResources::_kinsptr;
	KResources::KResources(KResourcesTypes Type):
		_ktype(Type),
		_kid(getUniqueNumber())
	{
		++_kinstances[Type];
		_kinsptr.insert({ _kid, this });
	}

	KResources::~KResources(){
		--_kinstances[_ktype];
		_kinsptr.erase(_kid);
	}

	const KResources *KResources::resourcesGetPointer(U64 ID){
		std::unordered_map<U64, const KResources *>::const_iterator got = _kinsptr.find(ID);

		if (got != _kinsptr.end())
			return got->second;

		return 0;
	}

	U64 KResources::resourcesGetTotalSize(){
		U64 total = 0;
		for (auto it = _kinsptr.begin(); it != _kinsptr.end(); ++it){
			total += it->second->resourcesGetSize();
		}
		return total;
	}

	U64 KResources::resourcesGetSize(KResourcesTypes Type){
		U64 total = 0;
		for (auto it = _kinsptr.begin(); it != _kinsptr.end(); ++it){
			if (it->second->_ktype == Type)
				total += it->second->resourcesGetSize();
		}

		return total;
	}
}