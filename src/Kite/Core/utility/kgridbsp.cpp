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

#include "Kite/core/utility/kgridbsp.h"

namespace Kite{
	KGridBSP::KGridBSP(const KGridBSPInfo &GridInfo) :
		_kgridInfo(GridInfo),
		_kcells(GridInfo.gridSize.x * GridInfo.gridSize.y, Internal::KCell()),
		_kborder(0)
	{}

	bool KGridBSP::addObject(KGridBSPObject *Object) {
		bool ret = true;
		if (Object) {

			// duplicate is not allowed
			if (Object->getCellID() >= 0)
				return false;

			U32 row = (U32)Object->getCellPosition()->y / (U32)_kgridInfo.cellSize.y;
			U32 col = (U32)Object->getCellPosition()->x / (U32)_kgridInfo.cellSize.x;
			U32 index = (row * _kgridInfo.gridSize.x) + col;

			// set tile
			if (index < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {

				// store position
				Object->_klink.point = *Object->getCellPosition();

				// set id
				Object->_kcID = index;
				
				// list is empty
				if (_kcells[index].first == 0) {

					// inite object
					Object->_klink.next = 0;
					Object->_klink.prev = 0;

					// add object as first item in the list
					_kcells[index].first = _kcells[index].second = Object;
				
				// list is not empty
				} else {

					// inite object
					Object->_klink.next = 0;
					Object->_klink.prev = _kcells[index].second;

					// attach object to last node in the list
					((KGridBSPObject *)_kcells[index].second)->_klink.next = Object;

					// set last pointer
					_kcells[index].second = Object;
				}
			} else {
				ret = false;
			}
		}

		return ret;
	}

	void KGridBSP::removeObject(KGridBSPObject *Object) {
		if (Object) {
			if (Object->getCellID() < 0)
				return;

			U32 index = Object->getCellID();

			if (index < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {

				// object is at begin node
				if (Object == _kcells[index].first) {
					// remove object from list
					_kcells[index].first = (KGridBSPObject *)Object->_klink.next;

					// single item list
					if (Object == _kcells[index].second) {
						_kcells[index].second = 0;
					} else {
						((KGridBSPObject *)_kcells[index].first)->_klink.prev = 0;
					}

					// reset object node info
					Object->_kcID = -1;
					Object->_klink.next = Object->_klink.prev = 0;

				// object is at end node
				} else if (Object == _kcells[index].second){
					_kcells[index].second = (KGridBSPObject *)Object->_klink.prev;

					// just in case
					if (_kcells[index].second) {
						((KGridBSPObject *)_kcells[index].second)->_klink.next = 0;
					}

					// reset object node info
					Object->_kcID = -1;
					Object->_klink.next = Object->_klink.prev = 0;

				// object is in middle of the list
				} else {
					// retrieve previous object
					KGridBSPObject *temp = (KGridBSPObject *)Object->_klink.prev;

					// just in case
					if (temp) {
						temp->_klink.next = (KGridBSPObject *)Object->_klink.next;

						// reset object node info
						Object->_kcID = -1;
						Object->_klink.next = Object->_klink.prev = 0;
					}
				}
			}
		}
	}

	void KGridBSP::setCellHitBitmap(const std::vector<KCellBitmapTypes> *CellBitmap) {

	}

	F32 KGridBSP::getGridWidth() const {
		return _kgridInfo.cellSize.x * _kgridInfo.gridSize.x;
	}

	F32 KGridBSP::getGridHeight() const {
		return _kgridInfo.cellSize.y * _kgridInfo.gridSize.y;
	}

	I32 KGridBSP::getCellID(const KVector2F32 &Position) const {
		// out of range
		if (Position.x >= getGridWidth() || Position.x < 0 || Position.y >= getGridHeight() || Position.y < 0)
			return -1;

		U32 row = (U32)Position.y / (U32)_kgridInfo.cellSize.y;
		U32 col = (U32)Position.x / (U32)_kgridInfo.cellSize.x;
		U32 index = (row * _kgridInfo.gridSize.x) + col;

		if (index >= 0 && index < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {
			return index;
		}
		return -1;
	}

	bool KGridBSP::getCellPosition(U32 CellID, KVector2F32 &Output) const {
		if (CellID < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {
			Output.x = (CellID % _kgridInfo.gridSize.x) * _kgridInfo.cellSize.x;
			Output.y = (CellID / _kgridInfo.gridSize.x) * _kgridInfo.cellSize.y;
			return true;
		} else {
			return false;
		}
	}

	bool KGridBSP::getCellDimension(U32 CellID, KRect2F32 &Output) const {
		if (CellID < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {
			F32 x = (CellID % _kgridInfo.gridSize.x) * _kgridInfo.cellSize.x;
			F32 y = (CellID / _kgridInfo.gridSize.x) * _kgridInfo.cellSize.y;

			Output.leftBottom.x = x;
			Output.leftBottom.y = y;

			Output.leftTop.x = x;
			Output.leftTop.y = y + _kgridInfo.cellSize.y;

			Output.rightBottom.x = x + _kgridInfo.cellSize.x;
			Output.rightBottom.y = y;

			Output.rightTop.x = x + _kgridInfo.cellSize.x;
			Output.rightTop.y = y + _kgridInfo.cellSize.y;

			return true;
		} else {
			return false;
		}
	}

	bool KGridBSP::getCellDimension(const KVector2F32 &Position, KRect2F32 &Output) const {
		I32 id = getCellID(Position);
		if (id >= 0) {
			return getCellDimension(id, Output);
		}
		
		return false;
	}

	KGridBSPObject *KGridBSP::getCellObjects(U32 CellID) const {
		if (CellID < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {
			return (KGridBSPObject *)_kcells[CellID].first;
		}
		return 0;
	}

	KGridBSPObject *KGridBSP::getCellObjects(const KVector2F32 &Position) const {
		I32 id = getCellID(Position);
		if (id >= 0) {
			return getCellObjects(getCellID(Position));
		}

		return 0;
	}

	void KGridBSP::setCellTrigger(U32 CellID, KCallCellTrigger Functor, void *Sender) {
		if (CellID < (_kgridInfo.gridSize.x * _kgridInfo.gridSize.y)) {
			_kcells[CellID].sender = Sender;
			_kcells[CellID].trigger = Functor;
		}
	}

	void KGridBSP::setCellTrigger(const KVector2F32 &Position, KCallCellTrigger Functor, void *Sender) {
		I32 tid = getCellID(Position);
		if (tid >= 0) {
			setCellTrigger(tid, Functor, Sender);
		}
	}

	void KGridBSP::queryCells(KRectF32 &Area) {
		// check callback
		if (!_kcallb) return;

		// bad area
		if (Area.right < Area.left) return;
		if (Area.top < Area.bottom) return;

		// added border
		Area.left -= (_kborder * _kgridInfo.cellSize.x);
		Area.bottom -= (_kborder * _kgridInfo.cellSize.y);
		Area.right += (_kborder * _kgridInfo.cellSize.x);
		Area.top += (_kborder * _kgridInfo.cellSize.y);

		// out of range
		if (Area.left >= getGridWidth()) return;
		if (Area.bottom >= getGridHeight()) return;
		if (Area.top <= 0) return;
		if (Area.right <= 0) return;

		// calibrate area
		if (Area.left < 0) Area.left = 0;
		if (Area.right < 0) Area.right = 0;
		if (Area.bottom < 0) Area.bottom = 0;
		if (Area.top < 0) Area.top = 0;
		if (Area.right > getGridWidth()) Area.right = getGridWidth();
		if (Area.top > getGridHeight()) Area.top = getGridHeight();

		Area.left -= fmod(Area.left, _kgridInfo.cellSize.x);
		Area.bottom -= fmod(Area.bottom, _kgridInfo.cellSize.y);

		// find lenght of row and column of given area
		// then fill every row in between
		U32 rowLen = (U32)(Area.right - Area.left) / (U32)_kgridInfo.cellSize.x;
		if (fmod(Area.right, _kgridInfo.cellSize.x) > 0) ++rowLen;
		U32 colLen = (U32)(Area.top - Area.bottom) / (U32)_kgridInfo.cellSize.y;
		if (fmod(Area.top, _kgridInfo.cellSize.y) > 0) ++colLen;
		I32 startPoint = getCellID(KVector2F32(Area.left, Area.bottom));
		U32 row = startPoint / _kgridInfo.gridSize.x;
		U32 tile = startPoint % _kgridInfo.gridSize.x;


		// fill vector
		if (startPoint >= 0) {
			for (U32 cc = 0; cc < colLen; cc++) {
				for (U32 rc = 0; rc < rowLen; rc++) {
					// query callback
					if (_kcells[row * cc + tile].first) {
						_kcallb(_kcells[(row * _kgridInfo.gridSize.x) + tile + rc].first, _ksender);
					}

					// cell trigger
					if (_kcells[(row * _kgridInfo.gridSize.x) + tile + rc].trigger) {
						_kcells[(row * _kgridInfo.gridSize.x) + tile + rc].trigger(((row * _kgridInfo.gridSize.x) + tile + rc),
							_kcells[(row * _kgridInfo.gridSize.x) + tile + rc].sender);
					}
				}
				++row;
			}
		}
	}

}
