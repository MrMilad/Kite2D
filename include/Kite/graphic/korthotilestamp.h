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
#ifndef KORTHOTILESTAMP_H
#define KORTHOTILESTAMP_H

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "korthotilestamp.khgen.h"
#include <vector>
#include <bitset>

KMETA
namespace Kite {
	KM_CLASS(POD)
	class KITE_FUNC_EXPORT KOrthoTileStamp {
		KMETA_KORTHOTILESTAMP_BODY();
	public:
		// width and height mean the real width and height of image not number of tiles.
		KM_CON(U32, U32, U32, U32)
		KOrthoTileStamp(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight);

		KM_PRO_GET(KP_NAME = "rows", KP_TYPE = U32, KP_CM = "number of rows")
		inline U32 getRowCount() const { return _krowCount; }

		KM_PRO_GET(KP_NAME = "columns", KP_TYPE = U32, KP_CM = "number of columns")
		inline U32 getColumnCount() const { return _kcolCount; }

		KM_PRO_GET(KP_NAME = "isValid", KP_TYPE = bool, KP_CM = "is stamp valid")
		inline bool isValid() const { return _kisValid; }

		KM_PRO_GET(KP_NAME = "tileWidth", KP_TYPE = U32, KP_CM = "width of tiles")
		inline U32 getTileWidth() const { return _ktileWidth; }

		KM_PRO_GET(KP_NAME = "tileHeight", KP_TYPE = U32, KP_CM = "height of tiles")
		inline U32 getTileHeight() const { return _ktileHeight; }

		KM_PRO_SET(KP_NAME = "anchorPoint")
		void setAnchorID(U32 TileID);

		KM_PRO_GET(KP_NAME = "anchorPoint", KP_TYPE = U32, KP_CM = "anchor point of selcted tiles")
		U32 getAnchorID() const { return _kanchor; }

		KM_FUN()
		void resetSize(U32 Width, U32 Height, U32 TileWidth, U32 TileHeight);

		KM_FUN()
		bool getTileID(const KVector2F32 &Position, U32 &Output) const ;

		KM_FUN()
		KRectF32 getTileDimension(U32 TileID) const;

		KM_FUN()
		U32 getTileRow(U32 TileID) const;

		KM_FUN()
		U32 getTileColumn(U32 TileID) const;

		KM_FUN()
		void select(U32 TileID);

		KM_FUN()
		void querySelect(const KRectF32 &Area);

		KM_FUN()
		void deselect(U32 TileID);

		KM_FUN()
		void queryDeselect(const KRectF32 &Area);

		KM_FUN()
		bool isSelected(U32 TileID);

		KM_FUN()
		void resetSelection();

		KM_FUN()
		const auto getSelectedItems() const { return &_kselected; }

		/// query only (will not select any tile, just temporary selection)
		/// override: if false passed, query will ignore selected tiles and will not pick them
		KM_FUN()
		void query(const KRectF32 &Area, bool Override);

		/// quey only output
		KM_FUN()
		const auto getQueriedItems() const { return &_kqueried; }

	private:
		void queryExec(const KRectF32 &Area, void(KOrthoTileStamp::*Func)(U32));
		void initeTile(U32 ID, KTileStamp &Item);
		void queryOnly(U32 TileID);
		KM_VAR(UNBIND) U32 _kwidth;
		KM_VAR(UNBIND) U32 _kheight;
		KM_VAR(UNBIND) U32 _ktileWidth;
		KM_VAR(UNBIND) U32 _ktileHeight;
		KM_VAR(UNBIND) U32 _krowCount;
		KM_VAR(UNBIND) U32 _kcolCount;
		KM_VAR(UNBIND) bool _kisValid;
		KM_VAR(UNBIND) U32 _kanchor;
		KM_VAR(UNBIND) std::vector<KTileStamp> _kselected;
		KM_VAR(UNBIND) std::vector<std::bitset<8>> _kitems; // 0 = selection 
		std::vector<KTileStamp> _kqueried;
		bool _koverride;
	};
}

#endif // KORTHOTILESTAMP_H