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

#include "Kite/Assist/map/ktilemap.h"

namespace Kite{
	KTileMap::KTileMap(const KTileMapInfo &MapInfo):
		_kmapInfo(MapInfo),
		_ktiles(MapInfo.mapSize.x * MapInfo.mapSize.y, 0)
	{}

	bool KTileMap::setObject(KTileMapObject *Objects) {
		bool ret = true;
		if (Objects) {
			U32 row = Objects->getTilePosition()->y / _kmapInfo.tileSize.y;
			U32 col = Objects->getTilePosition()->x / _kmapInfo.tileSize.x;
			U32 index = (row * _kmapInfo.mapSize.x) + col;

			// set tile
			if (index < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
				_ktiles[index] = Objects;
				Objects->_ktID = index;
			} else {
				ret = false;
			}
		}

		return ret;
	}

	void KTileMap::setTileHitBitmap(const std::vector<KTileBitmapTypes> &TileBitmap) {

	}

	F32 KTileMap::getMapWidth() const {
		return _kmapInfo.tileSize.x * _kmapInfo.mapSize.x;
	}

	F32 KTileMap::getMapHeight() const {
		return _kmapInfo.tileSize.y * _kmapInfo.mapSize.y;
	}

	I32 KTileMap::getTileID(const KVector2F32 &Position) const{
		// out of range
		if (Position.x >= getMapWidth() || Position.x < 0 || Position.y >= getMapHeight() || Position.y < 0)
			return -1;

		U32 row = Position.y / _kmapInfo.tileSize.y;
		U32 col = Position.x / _kmapInfo.tileSize.x;
		U32 index = (row * _kmapInfo.mapSize.x) + col;

		if (index >= 0 && index < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
			return index;
		}
		return -1;
	}

	bool KTileMap::getTilePosition(U32 TileID, KVector2F32 &Output) const {
		if (TileID < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
			Output.x = (TileID % _kmapInfo.mapSize.x) * _kmapInfo.tileSize.x;
			Output.y = (TileID / _kmapInfo.mapSize.x) * _kmapInfo.tileSize.y;
			return true;
		} else {
			return false;
		}
	}

	bool KTileMap::getTileDimension(U32 TileID, KRect2F32 &Output) const {
		if (TileID < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
			U32 x = (TileID % _kmapInfo.mapSize.x) * _kmapInfo.tileSize.x;
			U32 y = (TileID / _kmapInfo.mapSize.x) * _kmapInfo.tileSize.y;

			Output.leftBottom.x = x;
			Output.leftBottom.y = y;

			Output.leftTop.x = x;
			Output.leftTop.y = y + _kmapInfo.tileSize.y;

			Output.rightBottom.x = x + _kmapInfo.tileSize.x;
			Output.rightBottom.y = y;

			Output.rightTop.x = x + _kmapInfo.tileSize.x;
			Output.rightTop.y = y + _kmapInfo.tileSize.y;

			return true;
		} else {
			return false;
		}
	}

	bool KTileMap::getTileDimension(const KVector2F32 &Position, KRect2F32 &Output) const {
		I32 id = getTileID(Position);
		if (id >= 0) {
			return getTileDimension(id, Output);
		}
		
		return false;
	}

	KTileMapObject *KTileMap::getTileObject(U32 TileID) const{
		if (TileID < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
			return _ktiles[TileID];
		}
		return 0;
	}

	KTileMapObject *KTileMap::getTileObject(const KVector2F32 &Position) const {
		I32 id = getTileID(Position);
		if (id >= 0) {
			return getTileObject(getTileID(Position));
		}

		return 0;
	}

	const std::vector<KTileMapObject *> *KTileMap::queryTiles(KRectF32 &Area) const {
		static std::vector<KTileMapObject *> result;
		result.clear();

		// out of range
		if (Area.left >= getMapWidth()) return &result;
		if (Area.bottom >= getMapHeight()) return &result;
		if (Area.top <= 0) return &result;
		if (Area.right <= 0) return &result;

		// calibrate area
		if (Area.left < 0) Area.left = 0;
		if (Area.right < 0) Area.right = 0;
		if (Area.bottom < 0) Area.bottom = 0;
		if (Area.top < 0) Area.top = 0;
		if (Area.right > getMapWidth()) Area.right = getMapWidth();
		if (Area.top > getMapHeight()) Area.top = getMapHeight();

		Area.left -= fmod(Area.left, _kmapInfo.tileSize.x);
		Area.bottom -= fmod(Area.bottom, _kmapInfo.tileSize.y);

		// find lenght of row and column of given area
		// then fill every row in between
		U32 rowLen = (Area.right - Area.left) / _kmapInfo.tileSize.x;
		if (fmod(Area.right, _kmapInfo.tileSize.x) > 0) ++rowLen;
		U32 colLen = (Area.top - Area.bottom) / _kmapInfo.tileSize.y;
		if (fmod(Area.top, _kmapInfo.tileSize.y) > 0) ++colLen;
		I32 startPoint = getTileID(KVector2F32(Area.left, Area.bottom));
		U32 row = startPoint / _kmapInfo.mapSize.x;
		U32 tile = startPoint % _kmapInfo.mapSize.x;

		// calculate enough size
		result.reserve(rowLen * colLen);

		// fill vector
		if (startPoint >= 0) {
			for (auto i = 0; i < colLen; i++) {
				result.insert(result.end(), &_ktiles[(_kmapInfo.mapSize.x * row) + tile], (&_ktiles[(_kmapInfo.mapSize.x * row) + tile]) + rowLen);
				++row;
			}
		}

		return &result;
	}

}
