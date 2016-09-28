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
		KResource(Name, false, true),
		_kwidth(0),
		_kheight(0),
		_ktilewidth(1),
		_ktileheight(1),
		_ktotalWidthPixels(0),
		_ktotalHeightPixels(0),
		_ktotalStack(0),
		_ktileSet(nullptr)
	{
		_krootList.reserve(KORTHO_MAP_SIZE);
		_knodeList.reserve(KORTHO_STACK_SIZE);
	}

	bool KOrthogonalMap::inite() {
		setInite(true);
		return true;
	}

	void KOrthogonalMap::create(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight) {
		_krootList.clear();
		_knodeList.clear();
		_kfreeNodeList.clear();

		_kwidth = Width;
		_kheight = Height;
		_ktilewidth = TileWidth;
		_ktileheight = TileHeight;
		_ktotalWidthPixels = _kwidth * _ktilewidth;
		_ktotalHeightPixels = _kheight * _ktileheight;
		_ktotalStack = 0;

		_krootList.resize(Width * Height);
	}

	bool KOrthogonalMap::setTile(U32 TileID, const KAtlasItem &Texture, U16 LayerIndex, U16 TextureIndex, const KColor &Tint) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return false;
		}

		// check layer
		if (_krootList[TileID].size <= LayerIndex) return false;

		// iterate over nodes for findig given layer index
		SIZE nodeIndex = _krootList[TileID].firstIndex;
		for (SIZE i = 0; i < LayerIndex; ++i) {
			nodeIndex = _knodeList[nodeIndex].nextIndex;
		}

		// set node at the given layer index
		initeNode(_knodeList[nodeIndex], TileID, Texture, TextureIndex, Tint);
		return true;
	}

	void KOrthogonalMap::pushTile(U32 TileID, const KAtlasItem &Texture, U16 TextureIndex, const KColor &Tint) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// create a new node
		// there is enough capacity in node list so we use it
		SIZE nodeIndex = 0;
		if (_knodeList.size() < _knodeList.capacity()) {
			KOrthoTileNode node;
			initeNode(node, TileID, Texture, TextureIndex, Tint);
			_knodeList.push_back(node);
			nodeIndex = _knodeList.size() - 1;


			// recycle node from free list if any
		} else if (!_kfreeNodeList.empty()) {
			nodeIndex = _kfreeNodeList.back();
			_kfreeNodeList.pop_back();
			initeNode(_knodeList[nodeIndex], TileID, Texture, TextureIndex, Tint);

			// node list is full and we have to push new node to it (new allocation)
		} else {
			KOrthoTileNode node;
			initeNode(node, TileID, Texture, TextureIndex, Tint);
			_knodeList.push_back(node);
			nodeIndex = _knodeList.size() - 1;
		}

		// add new node to map
		// empty root
		if (_krootList[TileID].size == 0) {
			_krootList[TileID].firstIndex = nodeIndex;
			_krootList[TileID].lastIndex = nodeIndex;

			// link to last node
		} else {
			_knodeList[_krootList[TileID].lastIndex].nextIndex = nodeIndex;
			_krootList[TileID].lastIndex = nodeIndex;
		}

		++_krootList[TileID].size;
		++_ktotalStack;
	}

	void KOrthogonalMap::pushTileStamp(U32 TileID, const std::vector<KTileStamp> &Stamp, U16 TextureIndex, const KColor &Tint) {
		auto anchRow = getTileRow(TileID);
		auto anchCol = getTileColumn(TileID);
		for (auto it = Stamp.begin(); it != Stamp.end(); ++it) {
			if ((anchRow + it->row) < _kheight && (anchCol + it->col) < _kwidth){
				// calculate id and push 
				auto id = ((anchRow + it->row) * _kwidth) + (anchCol + it->col);
				pushTile(id, it->atlas, TextureIndex, Tint);
			}
		}
	}

	void KOrthogonalMap::pushLayer(const KAtlasItem &Texture, U16 TextureIndex, const KColor &Tint) {
		for (SIZE i = 0; i < _krootList.size(); ++i) {
			pushTile(i, Texture, TextureIndex, Tint);
		}
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
			--_ktotalStack;
			return;
		}

		KD_FPRINT("this tile is empty. tid: %u", TileID);
	}

	void KOrthogonalMap::popTileStamp(U32 TileID, const std::vector<KTileStamp> &Stamp) {
		auto anchRow = getTileRow(TileID);
		auto anchCol = getTileColumn(TileID);
		for (auto it = Stamp.begin(); it != Stamp.end(); ++it) {
			if ((anchRow + it->row) < _kheight && (anchCol + it->col) < _kwidth) {
				// calculate id and push 
				auto id = ((anchRow + it->row) * _kwidth) + (anchCol + it->col);
				popTile(id);
			}
		}
	}

	void KOrthogonalMap::clearTile(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// iterate over nodes and move them to free list
		_ktotalStack -= _krootList[TileID].size;
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
		if (_krootList.empty() || Position.x < 0 || Position.y < 0 ||
			Position.x > _ktotalWidthPixels || Position.y > _ktotalHeightPixels) {
			return false;
		}

		// calculate column\row
		U32 col = (U32)Position.x / _ktilewidth;
		if (col >= 1 && ((U32)Position.x % _ktilewidth) == 0) --col;

		U32 row = (U32)Position.y / _ktileheight;
		if (row >= 1 && ((U32)Position.y % _ktileheight) == 0) --row;

		// calculate id
		Output = (row * _kwidth) + col;

		return true;
	}

#if defined(KITE_EDITOR)
	U32 KOrthogonalMap::convertID(U32 TileID) {
		auto row = (TileID / _kwidth);
		auto col = TileID - (row * _kwidth);
		auto crow = (_krootList.size() / _kwidth) - ++row;
		
		return (crow * _kwidth) + col;
	}
#endif

	KRectF32 KOrthogonalMap::getTileDimension(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return KRectF32();
		}

		KRectF32 Output;
		Output.bottom = (TileID / _kwidth) * _ktileheight;
		Output.top = Output.bottom + _ktileheight;

		Output.left = (TileID % _kwidth) * _ktilewidth;
		Output.right = Output.left + _ktilewidth;
		return Output;
	}

	U32 KOrthogonalMap::getTileRow(U32 TileID) const {
		return TileID / _kwidth;
	}

	U32 KOrthogonalMap::getTileColumn(U32 TileID) const {
		return TileID % _kwidth;
	}

	SIZE KOrthogonalMap::getTileSize(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return 0;
		}

		return _krootList[TileID].size;
	}

	void KOrthogonalMap::queryTiles(U32 TileID, std::vector<KOrthoTile> &Output) {
		Output.clear();
		Output.reserve(_krootList[TileID].size);

		U32 tindex = _krootList[TileID].firstIndex;
		auto dim = getTileDimension(TileID);
		for (SIZE ncounter = 0; ncounter < _krootList[TileID].size; ++ncounter) {
			Output.push_back(KOrthoTile());
			Output.back().tint = KColor(_knodeList[tindex].verts[0].r, _knodeList[tindex].verts[0].g,
							   _knodeList[tindex].verts[0].b, _knodeList[tindex].verts[0].a);

			Output.back().atlas = *_ktileSet->getItem(_knodeList[tindex].verts[0].tindex)->getItem(_knodeList[tindex].atlasID);
			Output.back().textureIndex = _knodeList[tindex].verts[0].tindex;

			tindex = _knodeList[tindex].nextIndex;
		}
	}


	void KOrthogonalMap::queryTilesVertex(const KRectF32 &Area, std::vector<KGLVertex> &Output) {
		Output.clear();
		KRectF32 cullArea = Area;

		// check area (is inside?)
		if (cullArea.left > Area.right) return;
		if (cullArea.bottom > cullArea.top) return;
		if (cullArea.left > _ktotalWidthPixels) return;
		if (cullArea.bottom > _ktotalHeightPixels) return;

		// caliboration area
		if (cullArea.bottom < 0) cullArea.bottom = 0;
		if (cullArea.top > _ktotalHeightPixels) cullArea.top = _ktotalHeightPixels;

		if (cullArea.left < 0) cullArea.left = 0;
		if (cullArea.right > _ktotalWidthPixels) cullArea.right = _ktotalWidthPixels;

		// find bottom-left and top-right tile id's
		U32 blID = 0, brID = 0, trID = 0;
		getTileID(KVector2F32(cullArea.left, cullArea.bottom), blID);
		getTileID(KVector2F32(cullArea.right, cullArea.bottom), brID);
		getTileID(KVector2F32(cullArea.right, cullArea.top), trID);

		const U32 lenght = brID - blID;
		U32 lcounter = 0;

		Output.reserve((trID - blID));

		// iterate over tiles and extract their vertex data
		for (U32 i = blID; i <= trID; i) {
			U32 tindex = _krootList[i].firstIndex;
			for (SIZE ncounter = 0; ncounter < _krootList[i].size; ++ncounter) {
				Output.insert(Output.end(), _knodeList[tindex].verts, _knodeList[tindex].verts + 4);
				tindex = _knodeList[tindex].nextIndex;
			}

			// check lenght
			if (lcounter == lenght) {
				i += (_kwidth - lenght); // skip extra tiles and move to next row
				lcounter = 0;
			} else {
				++lcounter;
				++i;
			}
		}
	}

	void KOrthogonalMap::initeNode(KOrthoTileNode &Node, U32 ID, const KAtlasItem &UV, U16 TIndex, const KColor &Col) {
		KRectF32 dim = getTileDimension(ID);

		Node.atlasID = UV.id;
		
		Node.verts[0].pos.x = dim.left;
		Node.verts[0].pos.y = dim.bottom;
		Node.verts[1].pos.x = dim.left;
		Node.verts[1].pos.y = dim.top;
		Node.verts[2].pos.x = dim.right;
		Node.verts[2].pos.y = dim.bottom;
		Node.verts[3].pos.x = dim.right;
		Node.verts[3].pos.y = dim.top;

		Node.verts[0].tindex = Node.verts[1].tindex = Node.verts[2].tindex = Node.verts[3].tindex = TIndex;

		Node.verts[0].r = Node.verts[1].r = Node.verts[2].r = Node.verts[3].r = Col.getGLR();
		Node.verts[0].g = Node.verts[1].g = Node.verts[2].g = Node.verts[3].g = Col.getGLG();
		Node.verts[0].b = Node.verts[1].b = Node.verts[2].b = Node.verts[3].b = Col.getGLB();
		Node.verts[0].a = Node.verts[1].a = Node.verts[2].a = Node.verts[3].a = Col.getGLA();

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
		bserial << _ktotalWidthPixels;
		bserial << _ktotalHeightPixels;
		bserial << _ktotalStack;
		bserial << _krootList;
		bserial << _knodeList;
		bserial << _kfreeNodeList;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}
		Stream.close();

		// save composite list (texture)
		std::vector<KResource *> composite;
		composite.push_back((KResource *)_ktileSet);
		setCompositeList(composite);

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
		bserial >> _ktotalWidthPixels;
		bserial >> _ktotalHeightPixels;
		bserial >> _ktotalStack;
		bserial >> _krootList;
		bserial >> _knodeList;
		bserial >> _kfreeNodeList;

		// load composite resources (texture)
		auto slist = getCompositeList();
		if (!slist.empty()) {
			_ktileSet = (KAtlasTextureArray *)(*slist.begin());
		} else {
			_ktileSet = nullptr;
		}

		Stream.close();

		return true;
	}

	KMETA_KORTHOGONALMAP_SOURCE();
}