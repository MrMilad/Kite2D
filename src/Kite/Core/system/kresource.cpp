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

	U32 KResources::resGetCount(KResourcesTypes Type) {
		return _kinstances[Type]; 
	}

	const KResources *KResources::resGetPointer(U64 UniqueID){
		std::unordered_map<U64, const KResources *>::const_iterator got = _kinsptr.find(UniqueID);

		if (got != _kinsptr.end())
			return got->second;

		return 0;
	}

	U64 KResources::resGetTotalSize(){
		U64 total = 0;
		for (auto it = _kinsptr.begin(); it != _kinsptr.end(); ++it){
			total += it->second->resGetSize();
		}
		return total;
	}

	U64 KResources::resGetSize(KResourcesTypes Type){
		U64 total = 0;
		for (auto it = _kinsptr.begin(); it != _kinsptr.end(); ++it){
			if (it->second->_ktype == Type)
				total += it->second->resGetSize();
		}

		return total;
	}
}