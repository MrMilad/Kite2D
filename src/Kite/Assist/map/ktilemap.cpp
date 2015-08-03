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
	KTileMap::KTileMap(const KTileMapInfo &MapInfo) :
		_kmapInfo(MapInfo),
		_ktiles(MapInfo.mapSize.x * MapInfo.mapSize.y, std::pair<KTileMapObject *, KTileMapObject *>(0, 0))
	{}

	bool KTileMap::addObject(KTileMapObject *Object) {
		bool ret = true;
		if (Object) {

			// duplicate is not allowed
			if (Object->getTileID() >= 0)
				return false;

			U32 row = (U32)Object->getTilePosition()->y / (U32)_kmapInfo.tileSize.y;
			U32 col = (U32)Object->getTilePosition()->x / (U32)_kmapInfo.tileSize.x;
			U32 index = (row * _kmapInfo.mapSize.x) + col;

			// set tile
			if (index < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {

				// store position
				Object->_klink.point = *Object->getTilePosition();

				// set id
				Object->_ktID = index;
				
				// list is empty
				if (_ktiles[index].first == 0) {

					// inite object
					Object->_klink.next = 0;
					Object->_klink.prev = 0;

					// add object as first item in the list
					_ktiles[index].first = _ktiles[index].second = Object;
				
				// list is not empty
				} else {

					// inite object
					Object->_klink.next = 0;
					Object->_klink.prev = _ktiles[index].second;

					// attach object to last node in the list
					_ktiles[index].second->_klink.next = Object;

					// set last pointer
					_ktiles[index].second = Object;
				}
			} else {
				ret = false;
			}
		}

		return ret;
	}

	void KTileMap::removeObject(KTileMapObject *Object) {
		if (Object) {
			if (Object->getTileID() < 0)
				return;

			U32 index = Object->getTileID();

			if (index < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {

				// object is at begin node
				if (Object == _ktiles[index].first) {
					// remove object from list
					_ktiles[index].first = (KTileMapObject *)Object->_klink.next;

					// single item list
					if (Object == _ktiles[index].second) {
						_ktiles[index].second = 0;
					} else {
						_ktiles[index].first->_klink.prev = 0;
					}

					// reset object node info
					Object->_ktID = -1;
					Object->_klink.next = Object->_klink.prev = 0;

				// object is at end node
				} else if (Object == _ktiles[index].second){
					_ktiles[index].second = (KTileMapObject *)Object->_klink.prev;

					// just in case
					if (_ktiles[index].second) {
						_ktiles[index].second->_klink.next = 0;
					}

					// reset object node info
					Object->_ktID = -1;
					Object->_klink.next = Object->_klink.prev = 0;

				// object is in middle of the list
				} else {
					// retrieve previous object
					KTileMapObject *temp = (KTileMapObject *)Object->_klink.prev;

					// just in case
					if (temp) {
						temp->_klink.next = (KTileMapObject *)Object->_klink.next;

						// reset object node info
						Object->_ktID = -1;
						Object->_klink.next = Object->_klink.prev = 0;
					}
				}
			}
		}
	}

	void KTileMap::setTileHitBitmap(const std::vector<KTileBitmapTypes> *TileBitmap) {

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

		U32 row = (U32)Position.y / (U32)_kmapInfo.tileSize.y;
		U32 col = (U32)Position.x / (U32)_kmapInfo.tileSize.x;
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
			F32 x = (TileID % _kmapInfo.mapSize.x) * _kmapInfo.tileSize.x;
			F32 y = (TileID / _kmapInfo.mapSize.x) * _kmapInfo.tileSize.y;

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

	KTileMapObject *KTileMap::getTileObjects(U32 TileID) const{
		if (TileID < (_kmapInfo.mapSize.x * _kmapInfo.mapSize.y)) {
			return _ktiles[TileID].first;
		}
		return 0;
	}

	KTileMapObject *KTileMap::getTileObjects(const KVector2F32 &Position) const {
		I32 id = getTileID(Position);
		if (id >= 0) {
			return getTileObjects(getTileID(Position));
		}

		return 0;
	}

	void KTileMap::queryTiles(KRectF32 &Area) {
		// check callback
		if (!_kcallb) return;

		// out of range
		if (Area.left >= getMapWidth()) return;
		if (Area.bottom >= getMapHeight()) return;
		if (Area.top <= 0) return;
		if (Area.right <= 0) return;

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
		U32 rowLen = (U32)(Area.right - Area.left) / (U32)_kmapInfo.tileSize.x;
		//if (fmod(Area.right, _kmapInfo.tileSize.x) > 0) ++rowLen;
		U32 colLen = (U32)(Area.top - Area.bottom) / (U32)_kmapInfo.tileSize.y;
		//if (fmod(Area.top, _kmapInfo.tileSize.y) > 0) ++colLen;
		I32 startPoint = getTileID(KVector2F32(Area.left, Area.bottom));
		U32 row = startPoint / _kmapInfo.mapSize.x;
		U32 tile = startPoint % _kmapInfo.mapSize.x;


		// fill vector
		if (startPoint >= 0) {
			for (U32 cc = 0; cc < colLen; cc++) {
				for (U32 rc = 0; rc < rowLen; rc++) {
					if (_ktiles[row * cc + tile].first) {
						_kcallb(_ktiles[(row * _kmapInfo.mapSize.x) + tile + rc].first, _ksender);
					}
				}
				++row;
			}
		}
	}

}
