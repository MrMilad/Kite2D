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
#ifndef KTILEMAP_H
#define KTILEMAP_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/graphic/kgraphicstructs.h"
#include "kite/Core/graphic/kgraphictypes.h"
#include "kite/Assist/map/ktilemapobject.h"
#include <utility>
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KTileMap{
	public:
		KTileMap(const KTileMapInfo &MapInfo);

		// add an object to the appropriate tile
		// stack object in a single tile is allowed
		// add or duplicate same object in two or more tile is not allowed
		bool addObject(KTileMapObject *Object);

		// remove object from associated tile
		// object will remove even with changed position
		void removeObject(KTileMapObject *Object);

		// set bitmap array for isometric mouse hit detection
		// vector size must equal with tile size
		// eg: tile size = 100 x 50 (pixel) , vector size = 5000
		void setTileHitBitmap(const std::vector<KTileBitmapTypes> *TileBitmap);

		// get map width (in pixel)
		F32 getMapWidth() const;

		// get map height (in pixel)
		F32 getMapHeight() const;

		inline const KTileMapInfo *getMapInfo() const { return &_kmapInfo; }

		// retun -1 if out of position
		I32 getTileID(const KVector2F32 &Position) const;

		bool getTilePosition(U32 TileID, KVector2F32 &Output) const;
		bool getTileDimension(U32 TileID, KRect2F32 &Output) const;
		bool getTileDimension(const KVector2F32 &Position, KRect2F32 &Output) const;

		// return pointer to first object in the list
		KTileMapObject *getTileObjects(U32 TileID) const;

		// return pointer to first object in the list
		KTileMapObject *getTileObjects(const KVector2F32 &Position) const;

		// functor triggered when tile is in query result
		void setTileTrigger(U32 TileID, KCallTileTrigger Functor, void *Sender);
		void setTileTrigger(const KVector2F32 &Position, KCallTileTrigger Functor, void *Sender);

		// callback will called for every tile in the given area
		// callback will called once for every tile and send begin item of list
		inline void setQueryCallback(KCallTileQuery Callback, void *Sender) { _kcallb = Callback; _ksender = Sender; }

		void queryTiles(KRectF32 &Area);

	private:
		KTileMapInfo _kmapInfo;
		std::vector<Internal::KTile> _ktiles;
		KCallTileQuery _kcallb;
		void *_ksender;
	};
}

#endif // KTILEMAP_H

