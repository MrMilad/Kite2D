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
#ifndef KTILESHEET_H
#define KTILESHEET_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/graphic/kgraphicstructs.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KTileSheet{
	public:
		KTileSheet(const KTileMapInfo &MapInfo);

		// set bitmap array for isometric mouse coordinate detection
		// vector size must equal with tile size
		// eg: tile size = 100 x 50 (pixel) , vector size = 5000
		void setTileBitmap(const std::vector<KTileBitmapTypes> &TileBitmap);

		// get map width (in pixel)
		U32 getWidth() const;

		// get map height (in pixel)
		U32 getHeight() const;

		inline KTileMapInfo getMapInfo() const { return _kmapInfo; }

		U32 getTileID(const KVector2F32 &Position);

		KVector2F32 getTilePosition(U32 TileID);

		KRect2F32 getTileDimension(U32 TileID);
		KRect2F32 getTileDimension(const KVector2F32 &Position);

	private:
		KTileMapInfo _kmapInfo;
	};
}

#endif // KTILESHEET_H

