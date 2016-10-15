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
#include "Kite/graphic/korthotilestamp.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/serialization/types/kstdbitset.h"
#include <algorithm>
#include <luaintf/LuaIntf.h>

namespace Kite {
	KOrthoTileStamp::KOrthoTileStamp(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight){
		resetSize(Width, Height, TileWidth, TileHeight);
	}

	void KOrthoTileStamp::resetSize(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight) {
		_kwidth = Width;
		_kheight = Height;
		_ktileWidth = TileWidth;
		_ktileHeight = TileHeight;
		_kisValid = false;
		_kanchor = 0;
		resetSelection();

		if (TileWidth == 0 || TileHeight == 0) {
			return;
		}

		_krowCount = Height / TileHeight;
		_kcolCount = Width / TileWidth;

		_kitems.resize(_krowCount * _kcolCount);

		if (_krowCount > 0 && _kcolCount > 0) {
			_kisValid = true;
		}
	}

	bool KOrthoTileStamp::getTileID(const KVector2F32 &Position, U32 &Output) const {
		if (!_kisValid || Position.x < 0 || Position.y < 0 ||
			Position.x > (_kcolCount * _ktileWidth) || Position.y > (_krowCount * _ktileHeight)) {
			return false;
		}

		// calculate column\row
		U32 col = (U32)Position.x / _ktileWidth;
		if (col >= 1 && ((U32)Position.x % _ktileWidth) == 0) --col;

		U32 row = (U32)Position.y / _ktileHeight;
		if (row >= 1 && ((U32)Position.y % _ktileHeight) == 0) --row;

		// calculate id
		Output = (row * _kcolCount) + col;

		return true;
	}

	KRectF32 KOrthoTileStamp::getTileDimension(U32 TileID) const {
		KRectF32 Output;
		Output.bottom = (TileID / _kcolCount) * _ktileHeight;
		Output.top = Output.bottom + _ktileHeight;

		Output.left = (TileID % _kcolCount) * _ktileWidth;
		Output.right = Output.left + _ktileWidth;
		return Output;
	}

	U32 KOrthoTileStamp::getTileRow(U32 TileID) const {
		return TileID / _kcolCount;
	}

	U32 KOrthoTileStamp::getTileColumn(U32 TileID) const {
		return TileID % _kcolCount;
	}

	void KOrthoTileStamp::queryOnly(U32 TileID) {
		// add (selecet) id to selection list
		if (_koverride) {
			KTileStamp item;
			initeTile(TileID, item);
			_kqueried.push_back(item);
		}else{
			if (!isSelected(TileID)) {
				KTileStamp item;
				initeTile(TileID, item);
				_kqueried.push_back(item);
			}
		}
	}

	KTileStamp KOrthoTileStamp::getStamp(U32 TileID) const {
		if (_kitems.size() <= TileID) {
			KD_PRINT("tile id is out of range");
			return KTileStamp();
		}
		KTileStamp item;
		initeTile(TileID, item);
		return item;
	}

	void KOrthoTileStamp::select(U32 TileID) {
		if (_kitems.size() <= TileID) {
			KD_PRINT("tile id is out of range");
			return;
		}

		// add (selecet) id to selection list
		if (!isSelected(TileID)) {
			_kitems[TileID].set(0, true);
			KTileStamp item;
			initeTile(TileID, item);
			_kselected.push_back(item);
			return;
		}
	}

	void KOrthoTileStamp::querySelect(const KRectF32 &Area) {
		queryExec(Area, &KOrthoTileStamp::select);
	}

	void KOrthoTileStamp::query(const KRectF32 &Area, bool Override) {
		_koverride = Override;
		_kqueried.clear();
		queryExec(Area, &KOrthoTileStamp::queryOnly);
	}

	void KOrthoTileStamp::deselect(U32 TileID) {
		if (_kitems.size() <= TileID) {
			KD_PRINT("tile id is out of range");
			return;
		}

		// add (selecet) id to selection list
		if (isSelected(TileID)) {
			_kitems[TileID].set(0, false);
			
			// remove (deselect) item from list
			// swap and remove methode
			auto found = std::find_if(_kselected.begin(), _kselected.end(),
										  [TileID](KTileStamp const& item) {
				return item.atlas.id == TileID;
			});

			(*found) = _kselected.back();
			_kselected.pop_back();
			return;
		}
	}

	void KOrthoTileStamp::queryDeselect(const KRectF32 &Area) {
		queryExec(Area, &KOrthoTileStamp::deselect);
	}

	bool KOrthoTileStamp::isSelected(U32 TileID) {
		if (_kitems.size() <= TileID) {
			KD_PRINT("tile id is out of range");
			return false;
		}

		return _kitems[TileID].test(0);
	}

	void KOrthoTileStamp::resetSelection() {
		for (auto it = _kselected.begin(); it != _kselected.end(); ++it) {
			_kitems[it->atlas.id].set(0, false);
		}
		_kselected.clear();
	}

	void KOrthoTileStamp::setAnchorID(U32 TileID) {
		_kanchor = TileID;
		for (auto it = _kselected.begin(); it != _kselected.end(); ++it) {
			// reset position
			it->row = getTileRow(_kanchor) - getTileRow(it->atlas.id);
			it->col = getTileColumn(it->atlas.id) - getTileColumn(_kanchor);
		}
	}

	void KOrthoTileStamp::queryExec(const KRectF32 &Area, void(KOrthoTileStamp::*Func)(U32)) {
		KRectF32 cullArea = Area;

		// check area (is inside?)
		if (cullArea.left > cullArea.right) return;
		if (cullArea.bottom > cullArea.top) return;
		if (cullArea.left > _kwidth) return;
		if (cullArea.bottom > _kheight) return;
		if (cullArea.top < 0) return;
		if (cullArea.right < 0) return;

		// caliboration area
		if (cullArea.bottom < 0) cullArea.bottom = 0;
		if (cullArea.top > _kheight) cullArea.top = _kheight;

		if (cullArea.left < 0) cullArea.left = 0;
		if (cullArea.right > _kwidth) cullArea.right = _kwidth;

		// find bottom-left and top-right tile id's
		U32 blID = 0, brID = 0, trID = 0;
		getTileID(KVector2F32(cullArea.left, cullArea.bottom), blID);
		getTileID(KVector2F32(cullArea.right, cullArea.bottom), brID);
		getTileID(KVector2F32(cullArea.right, cullArea.top), trID);

		const U32 lenght = brID - blID;
		U32 lcounter = 0;

		// iterate over tiles and extract their vertex data
		for (U32 i = blID; i <= trID; i) {
			(*this.*Func)(i);

			// check lenght
			if (lcounter == lenght) {
				i += (_kcolCount - lenght); // skip extra tiles and move to next row
				lcounter = 0;
			} else {
				++lcounter;
				++i;
			}
		}
	}

	void KOrthoTileStamp::initeTile(U32 ID, KTileStamp &Item) const{
		// tile dimension
		auto dim = getTileDimension(ID);
		Item.atlas.id = ID;
		Item.atlas.width = _ktileWidth;
		Item.atlas.height = _ktileHeight;
		Item.atlas.xpos = dim.left;
		Item.atlas.ypos = dim.bottom;
		Item.atlas.blu = dim.left / (F32)_kwidth;
		Item.atlas.blv = dim.bottom / (F32)_kheight;
		Item.atlas.tru = dim.right / (F32)_kwidth;
		Item.atlas.trv = dim.top / (F32)_kheight;

		// anchor position
		Item.row = getTileRow(_kanchor) - getTileRow(ID);
		Item.col = getTileColumn(ID) - getTileColumn(_kanchor);
	}

	KMETA_KORTHOTILESTAMP_SOURCE();
}