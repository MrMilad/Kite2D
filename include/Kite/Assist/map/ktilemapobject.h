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
#ifndef KTILE_H
#define KTILE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"

namespace Kite {
	class KITE_FUNC_EXPORT KTileMapObject {
		friend class KTileMap;
	public:
		KTileMapObject() :
			_ktID(0)
		{}

		virtual ~KTileMapObject() {}

		// ID of tile in the map
		inline U32 getTileID() const { return _ktID; }

		virtual const KVector2F32 *getTilePosition() const = 0;

	private:
		mutable U32 _ktID;
	};
}


#endif // KTILE_H