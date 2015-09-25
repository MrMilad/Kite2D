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
#ifndef KGRIDBSP_H
#define KGRIDBSP_H

#include "Kite/Core/system/ksystemdef.h"
#include "kite/Core/utility/kutilitystructs.h"
#include "kite/Core/utility/kutilitytypes.h"
#include "kite/Core/utility/kgridbspobject.h"
#include <utility>
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KGridBSP{
	public:
		KGridBSP(const KGridBSPInfo &GridInfo);

		// add an object to the appropriate cell
		// stack object in a single cell is allowed
		// add or duplicate same object in two or more cell is not allowed
		bool addObject(KGridBSPObject *Object);

		// remove object from associated cell
		// object will remove even with changed position
		void removeObject(KGridBSPObject *Object);

		// set bitmap array for isometric-cell mouse hit detection
		// vector size must equal with cell size
		// eg: cell size = 100 x 50 (pixel) , vector size = 5000
		void setCellHitBitmap(const std::vector<KCellBitmapTypes> *TileBitmap);

		// get grid width (in pixel)
		F32 getGridWidth() const;

		// get grid height (in pixel)
		F32 getGridHeight() const;

		inline const KGridBSPInfo *getGridInfo() const { return &_kgridInfo; }

		// retun -1 if out of position
		I32 getCellID(const KVector2F32 &Position) const;

		bool getCellPosition(U32 CellID, KVector2F32 &Output) const;
		bool getCellDimension(U32 CellID, KRect2F32 &Output) const;
		bool getCellDimension(const KVector2F32 &Position, KRect2F32 &Output) const;

		// return pointer to first object in the list
		KGridBSPObject *getCellObjects(U32 CellID) const;

		// return pointer to first object in the list
		KGridBSPObject *getCellObjects(const KVector2F32 &Position) const;

		// border tickness
		// 0 by default
		// Width count as cell (not pixels or ...)
		// ex: width = 1, result= 1 * tile size
		inline void setQueryBorderWidth(U32 Width) { _kborder = Width; }
		inline U32 getQueryBorderWidth() const { return _kborder; }

		// functor triggered when tile is in query result
		void setCellTrigger(U32 CellID, KCallCellTrigger Functor, void *Sender);
		void setCellTrigger(const KVector2F32 &Position, KCallCellTrigger Functor, void *Sender);

		// callback will called for every cell in the given area
		// callback will called once for every cell and send begin item of list
		inline void setQueryCallback(KCallCellQuery Callback, void *Sender) { _kcallb = Callback; _ksender = Sender; }

		void queryCells(KRectF32 &Area);

	private:
		KGridBSPInfo _kgridInfo;
		std::vector<Internal::KCell> _kcells;
		KCallCellQuery _kcallb;
		void *_ksender;
		U32 _kborder;
	};
}

#endif // KGRIDBSP_H

