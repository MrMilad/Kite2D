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
		void create(U32 Width, U32 Height, U16 TileWidth, U16 TileHeight);

		KM_FUN()
		inline U32 getMapWidth() const { return _kwidth; }

		KM_FUN()
		inline U32 getMapHeight() const { return _kheight; }

		KM_FUN()
		inline U16 getTileWidth() const { return _ktilewidth; }

		KM_FUN()
		inline U16 getTileHeight() const { return _ktileheight; }

		KM_FUN()
		void pushTile(U32 TileID, const KAtlasItem &Texture, const KColor &Tint);

		KM_FUN()
		void popTile(U32 TileID);

		KM_FUN()
		void clearTile(U32 TileID);

		KM_FUN()
		bool getTileID(const KVector2F32 &Position, U32 &Output);

		KM_FUN()
		KRectU32 getTileDimension(U32 TileID);

		KM_FUN()
		SIZE getTileSize(U32 TileID);

		void queryTiles(const KRectF32 &Area, std::vector<KOrthoTile> &Output);

		void queryTilesVertex(const KRectF32 &Area, std::vector<KVertex> &Output);
	private:
		bool _saveStream(KOStream &Stream, const std::string &Address) override;
		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		void initeNode(KOrthoTile &Node, U32 ID, const KAtlasItem &UV, const KColor &Col);

		U32 _kwidth;
		U32 _kheight;
		U16 _ktilewidth;
		U16 _ktileheight;
		std::vector<KRootTileMap> _krootList;
		std::vector<KOrthoTile> _knodeList;
		std::vector<SIZE> _kfreeNodeList;
	};
}

#endif // KORTHOGONALMAP_H
