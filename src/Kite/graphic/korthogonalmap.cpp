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
#include "Kite/graphic/korthogonalmap.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/core/kfostream.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdvector.h"
#include <luaintf/LuaIntf.h>

namespace Kite {
	KOrthogonalMap::KOrthogonalMap(const std::string &Name) :
		KResource(Name, false, false) ,
		_kwidth(0),
		_kheight(0),
		_ktilewidth(0),
		_ktileheight(0)
	{
		_krootList.reserve(KORTHO_MAP_SIZE);
		_knodeList.reserve(KORTHO_STACK_SIZE);
	}

	bool KOrthogonalMap::inite() {
		setInite(true);
		return true;
	}

	void KOrthogonalMap::create(U32 Width, U32 Height, U16 TileWidth, U16 TileHeight) {
		_krootList.clear();
		_knodeList.clear();
		_kfreeNodeList.clear();

		_kwidth = Width;
		_ktileheight = Height;
		_ktilewidth = TileWidth;
		_ktileheight = TileHeight;

		_krootList.resize(Width * Height);
	}

	void KOrthogonalMap::pushTile(U32 TileID, const KAtlasItem &Texture, const KColor &Tint) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// create a new node
		// there is enough capacity in node list so we use it
		SIZE nodeIndex = 0;
		if (_knodeList.size() < _knodeList.capacity()) {
			KOrthoTile node;
			initeNode(node, TileID, Texture, Tint);
			_knodeList.push_back(node);
			nodeIndex = _knodeList.size() - 1;


		// recycle node from free list if any
		} else if (!_kfreeNodeList.empty()) {
			nodeIndex = _kfreeNodeList.back();
			_kfreeNodeList.pop_back();
			initeNode(_knodeList[nodeIndex], TileID, Texture, Tint);

		// node list is full and we have to push new node to it (new allocation)
		} else {
			KOrthoTile node;
			initeNode(node, TileID, Texture, Tint);
			_knodeList.push_back(node);
			nodeIndex = _knodeList.size() - 1;
		}

		// add new node to map
		// empty root
		if (_krootList[TileID].size > 0) {
			_krootList[TileID].firstIndex = nodeIndex;
			_krootList[TileID].lastIndex = nodeIndex;

		// link to last node
		} else {
			_knodeList[_krootList[TileID].lastIndex].nextIndex = nodeIndex;
			_krootList[TileID].lastIndex = nodeIndex;
		}

		++_krootList[TileID].size;
	}

	void KOrthogonalMap::popTile(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		if (_krootList[TileID].size > 0) {
			// move last index to free list and decerase the size
			_kfreeNodeList.push_back(_krootList[TileID].lastIndex);
			--_krootList[TileID].size;
			return;
		}

		KD_FPRINT("this tile is empty. tid: %u", TileID);
	}

	void KOrthogonalMap::clearTile(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// iterate over nodes and move them to free list
		for (SIZE i = 0; i < _krootList[TileID].size; ++i) {

			// push first node to free list
			_kfreeNodeList.push_back(_krootList[TileID].firstIndex);

			// move first pointer to next node
			_krootList[TileID].firstIndex = _knodeList[_krootList[TileID].firstIndex].nextIndex;
		}

		_krootList[TileID].firstIndex = 0;
		_krootList[TileID].size = 0;
	}

	bool KOrthogonalMap::getTileID(const KVector2F32 &Position, U32 &Output) {
		if (_krootList.empty() || Position.x < 0 || Position.y < 0) {
			return false;
		}

		// calculate column\row
		U32 col = (U32)Position.x / _ktilewidth;
		U32 row = (U32)Position.y / _ktileheight;

		// calculate id
		Output = (row * _kwidth) + col;

		//check range
		if (Output >= _krootList.size()) {
			return false;
		}

		return true;
	}

	KRectU32 KOrthogonalMap::getTileDimension(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return KRectU32();
		}

		KRectU32 Output;
		Output.bottom = (TileID / _kwidth) * _ktileheight;
		Output.top = Output.bottom + _ktileheight;

		Output.left = (TileID % _kwidth) * _ktilewidth;
		Output.right = Output.left + _ktilewidth;
		return Output;
	}

	SIZE KOrthogonalMap::getTileSize(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return 0;
		}

		return _krootList[TileID].size;
	}

	void KOrthogonalMap::queryTiles(const KRectF32 &Area, std::vector<KOrthoTile> &Output) {
		Output.clear();
		KRectF32 cullArea = Area;

		// check area (is inside?)
		if (cullArea.left > Area.right) return;
		if (cullArea.bottom > cullArea.top) return;
		if (cullArea.left > (_kwidth * _ktilewidth)) return;
		if (cullArea.bottom > (_kheight * _ktileheight)) return;

		// caliboration area
		if (cullArea.bottom < 0) cullArea.bottom = 0;
		if (cullArea.top > (_kheight * _ktileheight)) cullArea.top = _kheight * _ktileheight;

		if (cullArea.left < 0) cullArea.left = 0;
		if (cullArea.right >(_kwidth * _ktilewidth)) cullArea.right = _kwidth * _ktilewidth;

		// find bottom-left and top-right tile id's
		U32 blID = 0, trID = 0;
		getTileID(KVector2F32(cullArea.left, cullArea.bottom), blID);
		getTileID(KVector2F32(cullArea.right, cullArea.top), trID);

		Output.reserve((trID - blID));

		// iterate over tiles and extract their vertex data
		for (U32 i = blID; i <= trID; ++i) {
			U32 tindex = _krootList[i].firstIndex;
			for (SIZE ncounter = 0; ncounter < _krootList[i].size; ++ncounter) {
				Output.push_back(_knodeList[tindex]);
				tindex = _knodeList[tindex].nextIndex;
			}
		}
	}

	void KOrthogonalMap::queryTilesVertex(const KRectF32 &Area, std::vector<KVertex> &Output) {
		Output.clear();
		KRectF32 cullArea = Area;

		// check area (is inside?)
		if (cullArea.left > Area.right) return;
		if (cullArea.bottom > cullArea.top) return;
		if (cullArea.left > (_kwidth * _ktilewidth)) return;
		if (cullArea.bottom > (_kheight * _ktileheight)) return;

		// caliboration area
		if (cullArea.bottom < 0) cullArea.bottom = 0;
		if (cullArea.top >(_kheight * _ktileheight)) cullArea.top = _kheight * _ktileheight;

		if (cullArea.left < 0) cullArea.left = 0;
		if (cullArea.right >(_kwidth * _ktilewidth)) cullArea.right = _kwidth * _ktilewidth;

		// find bottom-left and top-right tile id's
		U32 blID = 0, trID = 0;
		getTileID(KVector2F32(cullArea.left, cullArea.bottom), blID);
		getTileID(KVector2F32(cullArea.right, cullArea.top), trID);

		Output.reserve((trID - blID));

		// iterate over tiles and extract their vertex data
		for (U32 i = blID; i <= trID; ++i) {
			U32 tindex = _krootList[i].firstIndex;
			for (SIZE ncounter = 0; ncounter < _krootList[i].size; ++ncounter) {
				Output.insert(Output.end(), _knodeList[tindex].verts, _knodeList[tindex].verts + 4);
				tindex = _knodeList[tindex].nextIndex;
			}
		}
	}

	void KOrthogonalMap::initeNode(KOrthoTile &Node, U32 ID, const KAtlasItem &UV, const KColor &Col) {
		KRectU32 dim = getTileDimension(ID);
		
		Node.verts[0].pos.x = (F32)dim.left;
		Node.verts[0].pos.y = (F32)dim.bottom;
		Node.verts[1].pos.x = (F32)dim.left;
		Node.verts[1].pos.y = (F32)dim.top;
		Node.verts[2].pos.x = (F32)dim.right;
		Node.verts[2].pos.y = (F32)dim.bottom;
		Node.verts[3].pos.x = (F32)dim.right;
		Node.verts[3].pos.y = (F32)dim.top;

		Node.verts[0].color = Node.verts[1].color = Node.verts[2].color = Node.verts[3].color = Col;

		Node.verts[1].uv = KVector2F32(UV.blu, UV.blv);
		Node.verts[0].uv = KVector2F32(UV.blu, UV.trv);
		Node.verts[3].uv = KVector2F32(UV.tru, UV.blv);
		Node.verts[2].uv = KVector2F32(UV.tru, UV.trv);
	}

	bool KOrthogonalMap::_saveStream(KOStream &Stream, const std::string &Address) {
		// texture information
		KBinarySerial bserial;
		bserial << std::string("KOMap");

		bserial << _kwidth;
		bserial << _kheight;
		bserial << _ktilewidth;
		bserial << _ktileheight;
		bserial << _krootList;
		bserial << _knodeList;
		bserial << _kfreeNodeList;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}
		Stream.close();

		return true;
	}

	bool KOrthogonalMap::_loadStream(KIStream &Stream, const std::string &Address) {
		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_PRINT("can't load stream");
			Stream.close();
			return false;
		}
		std::string format;

		bserial >> format;

		if (format != "KOMap") {
			KD_PRINT("incorrect file format");
			Stream.close();
			return false;
		}

		bserial >> _kwidth;
		bserial >> _kheight;
		bserial >> _ktilewidth;
		bserial >> _ktileheight;
		bserial >> _krootList;
		bserial >> _knodeList;
		bserial >> _kfreeNodeList;

		Stream.close();

		return true;
	}

	KMETA_KORTHOGONALMAP_SOURCE();
}