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
#ifndef KRESOURCES_H
#define KRESOURCES_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/system/knoncopyable.h"
#include <unordered_map>

namespace Kite{
	class KITE_FUNC_EXPORT KResources : KNonCopyable{
	public:
		KResources(KResourcesTypes Type);
		virtual ~KResources();

		// a unique id per resource
		inline U64 resGetUniqueID() const { return _kid; }

		// type of resources
		inline KResourcesTypes resGetType() const { return _ktype; }

		// count instances
		static U32 resGetCount(KResourcesTypes Type);

		// get resource pointer by id
		static const KResources *resGetPointer(U64 UniqueID);

		// size in byte
		// size of all types
		static U64 resGetTotalSize();

		// by type (total size of a type)
		static U64 resGetSize(KResourcesTypes Type);

		// by instance
		virtual U64 resGetSize() const = 0;

	private:
		U64 _kid;
		static U32 _kinstances[7];
		KResourcesTypes _ktype;
		static std::unordered_map<U64, const KResources *> _kinsptr;
	};
}

#endif // KRESOURCES_H