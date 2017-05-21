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
#ifndef KORTHOGONALMAP_H
#define KORTHOGONALMAP_H

#include "Kite/core/kcoredef.h"
#include "Kite/ecs/kresource.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/korthotilestamp.h"
#include "Kite/graphic/katlastexturearray.h"
#include <vector>
#include "korthogonalmap.khgen.h"

#define KORTHO_MAP_SIZE 400		// item (KRootTileMap)
#define KORTHO_STACK_SIZE 800	// item (KOrthoTile)

/*! \namespace Kite
\brief Public namespace.
*/
KMETA
namespace Kite {

	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KOrthogonalMap : public KResource {
		KM_INFO(KI_NAME = "OrthogonalMap");
		KORTHOGONALMAP_BODY();
	public:
		KOrthogonalMap(const std::string &Name);

		KM_FUN()
		void create(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight);

		KM_FUN()
		inline U32 getMapWidth() const { return _kwidth; }

		KM_FUN()
		void setTileset(KAtlasTextureArray *Tileset);

		KM_FUN()
		inline KAtlasTextureArray *getTileset() const { return _ktileSet; }

		KM_FUN()
		inline U32 getMapHeight() const { return _kheight; }

		KM_FUN()
		inline U32 getTileWidth() const { return _ktilewidth; }

		KM_FUN()
		inline U32 getTileHeight() const { return _ktileheight; }

		KM_FUN()
		inline U32 getMapWidthPixel() const { return _ktotalWidthPixels; }

		KM_FUN()
		inline U32 getMapHeightPixel() const { return _ktotalHeightPixels; }

		KM_FUN()
		inline U32 getTilesCount() const { return _krootList.size(); }

		KM_FUN()
		inline U32 getUsedLayers() const { return _knodeList.size(); }

		/// if the layer was exist, the tile of available layer will change. Otherwise, a new layer will be created.
		/// LayerIndex is same as stack index so 0 mean bottom layer
		KM_FUN()
		void setTileLayer(U32 TileID, U16 LayerIndex, const KOrthoLayer &Layer);

		KM_FUN()
		void setTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp, const KOrthoLayer &Layer);

		KM_FUN()
		void swapTileLayer(U32 TileID, U16 Layer1, U16 Layer2);

		KM_FUN()
		void removeTileLayer(U32 TileID, U16 LayerIndex);

		KM_FUN()
		void removeTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp);

		KM_FUN()
		void removeTileLayers(U32 TileID);

		KM_FUN()
		void swapMapLayer(U16 Layer1, U16 Layer2);

		KM_FUN()
		void removeMapLayer(U16 LayerIndex);

		KM_FUN()
		void setMapLayerName(U16 LayerIndex, const std::string &Name);

		KM_FUN()
		const std::string &getMapLayerName(U16 LayerIndex);

		KM_FUN()
		bool getTileLayer(U32 TileID, U16 LayerIndex, KOrthoLayer &Output);

		KM_FUN()
		void getTileLayers(U32 TileID, std::vector<KOrthoLayer> &Output);

		KM_FUN()
		void getTilesLayer(U32 TileID, U16 LayerIndex, const std::vector<KTileStamp> &Stamp, std::vector<KOrthoLayer> &Output);

		// number of attached tiles in the root
		KM_FUN()
		SIZE getTileLayerSize(U32 TileID);

		KM_FUN()
		bool getTileID(const KVector2F32 &Position, U32 &Output);

		KM_FUN()
		KRectF32 getTileDimension(U32 TileID);

		KM_FUN()
		U32 getTileRow(U32 TileID) const;

		KM_FUN()
		U32 getTileColumn(U32 TileID) const;

		/// fast and optimized for rendering purpose
		void queryTilesVertex(const KRectF32 &Area, std::vector<KGLVertex> &Output);

#if defined(KITE_EDITOR)
		// convert top-left coordinate system to bottom-left
		U32 convertID(U32 TileID);

		void setScenePtr(U32 TileID, U16 Layer, void *Pointer);
#endif
	private:
		bool _saveStream(KOStream &Stream, const std::string &Address) override;
		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		void moveNode(KOrthoNode &Node, SIZE NewIndex);
		void initeNode(KOrthoNode &Node, U32 TileID, const KOrthoLayer &Tile);
		void initeLayer(const KOrthoNode &Node, KOrthoLayer &Layer);
		KOrthoNode &insertNode(U32 TileID, U16 LayerIndex);

		U32 _kwidth;
		U32 _kheight;
		U32 _ktilewidth;
		U32 _ktileheight;
		U32 _ktotalWidthPixels;
		U32 _ktotalHeightPixels;
		std::vector<KRootTileMap> _krootList;
		std::vector<KOrthoNode> _knodeList;
		KOrthoTileStamp _ktilesetStamp;
		std::vector<std::pair<U16, std::string>> _klayerNames; //<layer index, name>

		KAtlasTextureArray *_ktileSet;
	};
}

#endif // KORTHOGONALMAP_H
