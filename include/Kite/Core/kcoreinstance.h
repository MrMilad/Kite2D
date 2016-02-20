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
#ifndef KCOREINSTANCE_H
#define KCOREINSTANCE_H

#include "Kite/Core/kcoredef.h"
#include "Kite/Core/kcoretypes.h"
#include "Kite/Core/knoncopyable.h"
#include <unordered_map>

namespace Kite{
	
	class KITE_FUNC_EXPORT KCoreInstance : KNonCopyable{
	public:
		KCoreInstance(KCoreInstanceTypes Type);
		virtual ~KCoreInstance();

		// a unique id per instance
		inline U64 getInstanceUniqueID() const { return _kid; }

		// type of resources
		inline KCoreInstanceTypes getInstanceType() const { return _ktype; }

		// count instances
		static U32 getInstanceCount(KCoreInstanceTypes Type);

		// get resource pointer by id
		static const KCoreInstance *getInstancePointer(U64 UniqueID);

		// size in byte
		// size of all types
		static U64 getInstanceTotalSize();

		// by type (total size of a type)
		static U64 getInstanceSize(KCoreInstanceTypes Type);

		// by instance
		virtual U64 getInstanceSize() const = 0;

	private:
		static std::unordered_map<U64, const KCoreInstance *> _kinsptr;
		static U32 _kinstances[12];
		U64 _kid;
		KCoreInstanceTypes _ktype;
	};
}

#endif // KCOREINSTANCE_H