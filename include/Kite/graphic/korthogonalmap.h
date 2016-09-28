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
#include "Kite/core/kresource.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kgraphicstructs.h"
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
		KMETA_KORTHOGONALMAP_BODY();
	public:
		KOrthogonalMap(const std::string &Name);

		bool inite() override;

		KM_FUN()
		void create(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight);

		KM_FUN()
		inline U32 getMapWidth() const { return _kwidth; }

		KM_FUN()
		inline void setTileset(KAtlasTextureArray *Tileset) { _ktileSet = Tileset; }

		KM_FUN()
		inline KAtlasTextureArray *getTileset() const { return _ktileSet; }

		KM_FUN()
		inline U32 getMapHeight() const { return _kheight; }

		KM_FUN()
		inline U32 getTileWidth() const { return _ktilewidth; }

		KM_FUN()
		inline U32 getTileHeight() const { return _ktileheight; }

		KM_FUN()
		inline U32 getTotalWidthPixelSize() const { return _ktotalWidthPixels; }

		KM_FUN()
		inline U32 getTotalHeightPixelSize() const { return _ktotalHeightPixels; }

		KM_FUN()
		inline U32 getTotalTiles() const { return _krootList.size(); }

		KM_FUN()
		inline U32 getTotalStackedTiles() const { return _ktotalStack; }

		KM_FUN()
		void pushTile(U32 TileID, const KAtlasItem &Texture, U16 TextureIndex, const KColor &Tint);

		KM_FUN()
		void pushTileStamp(U32 TileID, const std::vector<KTileStamp> &Stamp, U16 TextureIndex, const KColor &Tint);

		/// push a tile over entire map
		/// common usage: inite map (with background tile) or add a new layer to tiles
		KM_FUN()
		void pushLayer(const KAtlasItem &Texture, U16 TextureIndex, const KColor &Tint);

		/// return true if LayerIndex was exist in the TileID
		KM_FUN()
		bool setTile(U32 TileID, const KAtlasItem &Texture, U16 LayerIndex, U16 TextureIndex, const KColor &Tint);

		KM_FUN()
		void popTile(U32 TileID);

		KM_FUN()
		void popTileStamp(U32 TileID, const std::vector<KTileStamp> &Stamp);

		KM_FUN()
		void clearTile(U32 TileID);

		KM_FUN()
		bool getTileID(const KVector2F32 &Position, U32 &Output);

		KM_FUN()
		KRectF32 getTileDimension(U32 TileID);

		KM_FUN()
		U32 getTileRow(U32 TileID) const;

		KM_FUN()
		U32 getTileColumn(U32 TileID) const;

		// number of attached tiles in the root
		KM_FUN()
		SIZE getTileSize(U32 TileID);

		/// normal version
		KM_FUN()
		void queryTiles(U32 TileID, std::vector<KOrthoTile> &Output);

		/// fast and optimized version for rendering purpose
		void queryTilesVertex(const KRectF32 &Area, std::vector<KGLVertex> &Output);

#if defined(KITE_EDITOR)
		// convert top-left coordinate system to bottom-left
		U32 convertID(U32 TileID);
#endif
	private:
		bool _saveStream(KOStream &Stream, const std::string &Address) override;
		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		void initeNode(KOrthoTileNode &Node, U32 ID, const KAtlasItem &UV, U16 TIndex, const KColor &Col);

		U32 _kwidth;
		U32 _kheight;
		U32 _ktilewidth;
		U32 _ktileheight;
		U32 _ktotalWidthPixels;
		U32 _ktotalHeightPixels;
		U32 _ktotalStack;
		std::vector<KRootTileMap> _krootList;
		std::vector<KOrthoTileNode> _knodeList;
		std::vector<SIZE> _kfreeNodeList;

		KAtlasTextureArray *_ktileSet;
	};
}

#endif // KORTHOGONALMAP_H
