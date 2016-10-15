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
#include "Kite/serialization/types/kstdpair.h"
#include <luaintf/LuaIntf.h>
#include <assert.h>

namespace Kite {
	KOrthogonalMap::KOrthogonalMap(const std::string &Name) :
		KResource(Name, false, true),
		_kwidth(0),
		_kheight(0),
		_ktilewidth(1),
		_ktileheight(1),
		_ktotalWidthPixels(0),
		_ktotalHeightPixels(0),
		_ktileSet(nullptr),
		_ktilesetStamp(0, 0, 1, 1)
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
		_klayerNames.clear();
		
		if (_ktileSet) {
			_ktilesetStamp.resetSize(_ktileSet->getTextureWidth(), _ktileSet->getTextureHeight(), TileWidth, TileHeight);
		}

		_kwidth = Width;
		_kheight = Height;
		_ktilewidth = TileWidth;
		_ktileheight = TileHeight;
		_ktotalWidthPixels = _kwidth * _ktilewidth;
		_ktotalHeightPixels = _kheight * _ktileheight;

		_krootList.resize(Width * Height);
	}

	void KOrthogonalMap::setTileset(KAtlasTextureArray *Tileset) {
		_ktileSet = Tileset;

		if (_ktileSet) {
			_ktilesetStamp.resetSize(_ktileSet->getTextureWidth(), _ktileSet->getTextureHeight(), _ktilewidth, _ktileheight);
		} else {
			_ktilesetStamp.resetSize(0, 0, 1, 1);
		}
	}

	void KOrthogonalMap::setTileLayer(U32 TileID, U16 LayerIndex, const KOrthoLayer &Layer) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// find layer if it is already exist otherwise create it
		KOrthoNode &nodeRef = insertNode(TileID, LayerIndex);

		// reset node with new values
		initeNode(nodeRef, TileID, Layer);
	}

	void KOrthogonalMap::setTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp, const KOrthoLayer &Layer) {
		auto anchRow = getTileRow(TileID);
		auto anchCol = getTileColumn(TileID);
		for (auto it = Stamp.begin(); it != Stamp.end(); ++it) {
			if ((anchRow + it->row) < _kheight && (anchCol + it->col) < _kwidth) {
				// calculate id
				auto id = ((anchRow + it->row) * _kwidth) + (anchCol + it->col);
				setTileLayer(id, LayerIndex, Layer);
			}
		}
	}

	void KOrthogonalMap::swapTileLayer(U32 TileID, U16 Layer1, U16 Layer2) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		if (Layer1 == Layer2) return;

		// layer 1
		KOrthoLayer layer1;
		bool l1 = getTileLayer(TileID, Layer1, layer1);
		removeTileLayer(TileID, Layer1);

		// layer 2
		KOrthoLayer layer2;
		bool l2 = getTileLayer(TileID, Layer2, layer2);
		removeTileLayer(TileID, Layer2);

		if (l1) {
			setTileLayer(TileID, Layer2, layer1);
		}

		if (l2) {
			setTileLayer(TileID, Layer1, layer2);
		}

#if defined(KITE_EDITOR)
		setScenePtr(TileID, Layer1, layer2.sceneItem);
		setScenePtr(TileID, Layer2, layer1.sceneItem);
#endif

	}

	void KOrthogonalMap::removeTileLayer(U32 TileID, U16 LayerIndex) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// empty tile (there is no layer)
		if (_krootList[TileID].layerSize == 0) return;

		// iterate over layers
		SIZE nodeIndex = _krootList[TileID].firstNode;
		SIZE prevNode = nodeIndex;
		for (U16 i = 0; i < _krootList[TileID].layerSize; ++i) {
			// layer not found
			if (_knodeList[nodeIndex].layerIndex > LayerIndex) break;

			// layer found
			if (_knodeList[nodeIndex].layerIndex == LayerIndex) {

				// first node 
				if (i == 0) {
					_krootList[TileID].firstNode = _knodeList[nodeIndex].nextNode;

				} else {
					_knodeList[prevNode].nextNode = _knodeList[nodeIndex].nextNode;
				}

				--_krootList[TileID].layerSize;

				// swap and remove methode
				moveNode(_knodeList.back(), nodeIndex);
				_knodeList.pop_back();
				break;
			}

			// store current node
			prevNode = nodeIndex;

			// move to next node
			nodeIndex = _knodeList[nodeIndex].nextNode;
		}
	}

	void KOrthogonalMap::removeTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp) {
		auto anchRow = getTileRow(TileID);
		auto anchCol = getTileColumn(TileID);
		for (auto it = Stamp.begin(); it != Stamp.end(); ++it) {
			if ((anchRow + it->row) < _kheight && (anchCol + it->col) < _kwidth) {
				// calculate id
				auto id = ((anchRow + it->row) * _kwidth) + (anchCol + it->col);
				removeTileLayer(id, LayerIndex);
			}
		}
	}

	void KOrthogonalMap::removeTileLayers(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// iterate over nodes and remove them
		auto tempSize = _krootList[TileID].layerSize;
		for (SIZE i = 0; i < tempSize; ++i) {
			--_krootList[TileID].layerSize;
			// swap and remove methode
			// swap removed node with last node in the list
			moveNode(_knodeList.back(), _krootList[TileID].firstNode);
			_knodeList.pop_back();

			// move first pointer to next node
			_krootList[TileID].firstNode = _knodeList[_krootList[TileID].firstNode].nextNode;
		}

		// cleanup root
		_krootList[TileID].firstNode = 0;
	}

	void KOrthogonalMap::swapMapLayer(U16 Layer1, U16 Layer2) {
		for (SIZE i = 0; i < _krootList.size(); ++i) {
			swapTileLayer(i, Layer1, Layer2);
		}

		auto name1 = getMapLayerName(Layer1);
		auto name2 = getMapLayerName(Layer2);

		setMapLayerName(Layer1, name2);
		setMapLayerName(Layer2, name1);
	}

	void KOrthogonalMap::removeMapLayer(U16 LayerIndex) {
		for (SIZE i = 0; i < _krootList.size(); ++i) {
			// remove layer
			removeTileLayer(i, LayerIndex);
		}
	}

	void KOrthogonalMap::setMapLayerName(U16 LayerIndex, const std::string &Name) {
		// linear search
		for (auto it = _klayerNames.begin(); it != _klayerNames.end(); ++it) {
			if (it->first == LayerIndex) {
				it->second = Name;
				return;
			}
		}

		// add to vector
		_klayerNames.push_back({ LayerIndex, Name });
	}

	const std::string &KOrthogonalMap::getMapLayerName(U16 LayerIndex) {
		// linear search
		for (auto it = _klayerNames.begin(); it != _klayerNames.end(); ++it) {
			if (it->first == LayerIndex) {
				return it->second;
			}
		}

		// add to vector
		_klayerNames.push_back({ LayerIndex, "" });
		return _klayerNames.back().second;
	}

	bool KOrthogonalMap::getTileLayer(U32 TileID, U16 LayerIndex, KOrthoLayer &Output) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return false;
		}

		// iterate over nodes
		SIZE nodeIndex = _krootList[TileID].firstNode;
		for (SIZE i = 0; i < _krootList[TileID].layerSize; ++i) {
			// layer found
			if (_knodeList[nodeIndex].layerIndex == LayerIndex) {
				initeLayer(_knodeList[nodeIndex], Output);
				return true;
			}

			// layer not found
			if (_knodeList[nodeIndex].layerIndex > LayerIndex) break;

			// move to next node
			nodeIndex = _knodeList[nodeIndex].nextNode;
		}
		return false;
	}

	void KOrthogonalMap::getTileLayers(U32 TileID, std::vector<KOrthoLayer> &Output) {
		Output.clear();
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}
		Output.reserve(_krootList[TileID].layerSize);

		U32 tindex = _krootList[TileID].firstNode;
		auto dim = getTileDimension(TileID);
		for (SIZE ncounter = 0; ncounter < _krootList[TileID].layerSize; ++ncounter) {
			Output.push_back(KOrthoLayer());
			initeLayer(_knodeList[tindex], Output.back());
			tindex = _knodeList[tindex].nextNode;
		}
	}

	void KOrthogonalMap::getTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp, std::vector<KOrthoLayer> &Output) {
		Output.clear();
		auto anchRow = getTileRow(TileID);
		auto anchCol = getTileColumn(TileID);
		for (auto it = Stamp.begin(); it != Stamp.end(); ++it) {
			if ((anchRow + it->row) < _kheight && (anchCol + it->col) < _kwidth) {
				// calculate id
				auto id = ((anchRow + it->row) * _kwidth) + (anchCol + it->col);
				KOrthoLayer layer;
				if (getTileLayer(id, LayerIndex, layer)) {
					Output.push_back(layer);
				}
			}
		}
	}

	SIZE KOrthogonalMap::getTileLayerSize(U32 TileID) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return 0;
		}

		return _krootList[TileID].layerSize;
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

	void KOrthogonalMap::setScenePtr(U32 TileID, U16 LayerIndex, void *Pointer) {
		// check range
		if (TileID >= _krootList.size()) {
			KD_PRINT("tile id is out of range or tile map is not created yet");
			return;
		}

		// iterate over nodes
		SIZE nodeIndex = _krootList[TileID].firstNode;
		for (SIZE i = 0; i < _krootList[TileID].layerSize; ++i) {
			// layer found
			if (_knodeList[nodeIndex].layerIndex == LayerIndex) {
				_knodeList[nodeIndex].sceneItem = Pointer;
				return;
			}

			// layer not found
			if (_knodeList[nodeIndex].layerIndex > LayerIndex) break;

			// move to next node
			nodeIndex = _knodeList[nodeIndex].nextNode;
		}
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

	void KOrthogonalMap::queryTilesVertex(const KRectF32 &Area, std::vector<KGLVertex> &Output) {
		Output.clear();
		KRectF32 cullArea = Area;

		// check area (is inside?)
		if (cullArea.left > Area.right) return;
		if (cullArea.bottom > cullArea.top) return;
		if (cullArea.left > _ktotalWidthPixels) return;
		if (cullArea.bottom > _ktotalHeightPixels) return;
		if (cullArea.top < 0) return;
		if (cullArea.right < 0) return;

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
			U32 tindex = _krootList[i].firstNode;
			for (SIZE ncounter = 0; ncounter < _krootList[i].layerSize; ++ncounter) {
				Output.insert(Output.end(), _knodeList[tindex].verts, _knodeList[tindex].verts + 4);
				tindex = _knodeList[tindex].nextNode;
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

	void KOrthogonalMap::moveNode(KOrthoNode &Node, SIZE NewIndex) {
		// iterate over nodes and change indexes
		SIZE nodeIndex = _krootList[Node.tileID].firstNode;
		SIZE prevNode = 0;
		for (U16 i = 0; i < _krootList[Node.tileID].layerSize; ++i) {

			if (_knodeList[nodeIndex].layerIndex == Node.layerIndex) {
				// first node
				if (i == 0) {
					_krootList[Node.tileID].firstNode = NewIndex;

				} else {
					_knodeList[prevNode].nextNode = NewIndex;
				}
			}

			prevNode = nodeIndex;
			nodeIndex = _knodeList[nodeIndex].nextNode;
		}

		// mve node values
		_knodeList[NewIndex] = Node;
	}

	void KOrthogonalMap::initeNode(KOrthoNode &Node, U32 TileID, const KOrthoLayer &Layer) {
		Node.tileID = TileID;
		Node.atlasID = Layer.atlas.id;
		Node.fliph = Layer.atlas.getFlipH();
		Node.flipv = Layer.atlas.getFlipV();

		KRectF32 dim = getTileDimension(TileID);
		Node.verts[0].pos.x = dim.left;
		Node.verts[0].pos.y = dim.bottom;
		Node.verts[1].pos.x = dim.left;
		Node.verts[1].pos.y = dim.top;
		Node.verts[2].pos.x = dim.right;
		Node.verts[2].pos.y = dim.bottom;
		Node.verts[3].pos.x = dim.right;
		Node.verts[3].pos.y = dim.top;

		Node.verts[0].r = Node.verts[1].r = Node.verts[2].r = Node.verts[3].r = Layer.blend.getR();
		Node.verts[0].g = Node.verts[1].g = Node.verts[2].g = Node.verts[3].g = Layer.blend.getG();
		Node.verts[0].b = Node.verts[1].b = Node.verts[2].b = Node.verts[3].b = Layer.blend.getB();
		Node.verts[0].a = Node.verts[1].a = Node.verts[2].a = Node.verts[3].a = Layer.blend.getA();

		Node.verts[0].tindex = Node.verts[1].tindex = Node.verts[2].tindex = Node.verts[3].tindex = Layer.textureID;
		Node.verts[1].uv = KVector2F32(Layer.atlas.blu, Layer.atlas.blv);
		Node.verts[0].uv = KVector2F32(Layer.atlas.blu, Layer.atlas.trv);
		Node.verts[3].uv = KVector2F32(Layer.atlas.tru, Layer.atlas.blv);
		Node.verts[2].uv = KVector2F32(Layer.atlas.tru, Layer.atlas.trv);
	}

	void KOrthogonalMap::initeLayer(const KOrthoNode &Node, KOrthoLayer &Layer) {
		Layer.textureID = Node.verts[0].tindex;
		Layer.blend = KColor(Node.verts[0].r, Node.verts[0].g, Node.verts[0].b, Node.verts[0].a);
		Layer.atlas = _ktilesetStamp.getStamp(Node.atlasID).atlas;
		Layer.layerIndex = Node.layerIndex;

		if (Node.fliph) Layer.atlas.flipH();
		if (Node.flipv) Layer.atlas.flipV();

#if defined(KITE_EDITOR)
		Layer.sceneItem = Node.sceneItem;
#endif
	}

	KOrthoNode &KOrthogonalMap::insertNode(U32 TileID, U16 LayerIndex){
		// iterate over nodes for findig given layer index
		SIZE nodeIndex = _krootList[TileID].firstNode;
		SIZE prevNode = nodeIndex;
		bool found = false;
		U16 i = 0;
		for (i; i < _krootList[TileID].layerSize; ++i) {
			// layer found
			if (_knodeList[nodeIndex].layerIndex == LayerIndex) {
				found = true;
				break;
			}

			// layer not found
			if (_knodeList[nodeIndex].layerIndex > LayerIndex) {
				break;
			}

			// store current node
			prevNode = nodeIndex;

			// move to next node
			nodeIndex = _knodeList[nodeIndex].nextNode;
		}

		// layer not found, so we create it
		if (!found) {
			KOrthoNode node;
			node.layerIndex = LayerIndex;
			nodeIndex = _knodeList.size();

			// its first node in the root list
			if (i == 0) {
				node.nextNode = _krootList[TileID].firstNode;
				_krootList[TileID].firstNode = nodeIndex;

			// its not first node in the root list
			} else {
				node.nextNode = _knodeList[prevNode].nextNode;
				_knodeList[prevNode].nextNode = nodeIndex;
			}

			// add node to node list
			_knodeList.push_back(node);

			// update root stats
			++_krootList[TileID].layerSize;
		}

		return _knodeList[nodeIndex];
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
		bserial << _krootList;
		bserial << _knodeList;
		bserial << _ktilesetStamp;
		bserial << _klayerNames;

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
		bserial >> _krootList;
		bserial >> _knodeList;
		bserial >> _ktilesetStamp;
		bserial >> _klayerNames;

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