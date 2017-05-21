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
#include "Kite/bsp/kspatialhash.h"
#include "Kite/core/kcoreutil.h"

namespace Kite {
	KSpatialHash::KSpatialHash(U32 CellSize, U8 Levels) :
		_klevel(Levels),
		_kcellSize(nullptr),
		_kcellCounts(nullptr),
		_kobjCounts(nullptr) {
		// allocate array for each level
		_kcellSize = new U32[Levels + 1];
		_kcellCounts = new U32[Levels + 1];
		_kobjCounts = new U32[Levels + 1];

		_kcellSize[0] = CellSize;
		_kcellCounts[0] = 0;
		_kobjCounts[0] = 0;
		for (Levels = 1; Levels <= _klevel; ++Levels) {
			// calculate size of cells in each level
			_kcellSize[Levels] = _kcellSize[Levels - 1] / 2;

			// initialize
			_kcellCounts[Levels] = 0;
			_kobjCounts[Levels] = 0;
		}
	}

	KSpatialHash::~KSpatialHash() {
		if (_kcellSize) {
			delete[] _kcellSize;
			delete[] _kcellCounts;
			delete[] _kobjCounts;
		}
	}
	void KSpatialHash::removeCell(Cell *cell, U8 Level, std::unordered_map<U64, KSpatialHash::Cell>::iterator &TopIter) {
		// cehck cell
		if (cell->nodes) return;

		if (!cell->split) {
			// remove cell from parent splitter
			if (cell->parent) {
				if (cell->parent->split->link[0][0] == cell) cell->parent->split->link[0][0] = nullptr;
				if (cell->parent->split->link[1][1] == cell) cell->parent->split->link[1][1] = nullptr;
				if (cell->parent->split->link[0][1] == cell) cell->parent->split->link[0][1] = nullptr;
				if (cell->parent->split->link[1][0] == cell) cell->parent->split->link[1][0] = nullptr;
			}

			// check parent
			if (cell->parent) {
				removeCell(cell->parent, Level - 1, TopIter);
			}

			// remove cell
			cell->parent = nullptr;
			cell->nodes = nullptr;
			if (Level > 0) {
				_kcellPool.deallocate_node(cell);
			} else {
				_kmap.erase(TopIter);
			}

			// decrease cell counts
			--_kcellCounts[Level];
			return;
		}

		// check splitter
		if (!cell->split->link[0][0] && !cell->split->link[0][1]
			&& !cell->split->link[1][0] && !cell->split->link[1][1]) {
			// remove cell from parent splitter
			if (cell->parent) {
				if (cell->parent->split->link[0][0] == cell) cell->parent->split->link[0][0] = nullptr;
				if (cell->parent->split->link[1][1] == cell) cell->parent->split->link[1][1] = nullptr;
				if (cell->parent->split->link[0][1] == cell) cell->parent->split->link[0][1] = nullptr;
				if (cell->parent->split->link[1][0] == cell) cell->parent->split->link[1][0] = nullptr;
			}

			// check parent
			if (cell->parent) {
				removeCell(cell->parent, Level - 1, TopIter);
			}

			// remove cell
			cell->parent = nullptr;
			cell->nodes = nullptr;
			cell->split = nullptr;
			_ksplitPool.deallocate_node(cell->split);
			if (Level > 0) {
				_kcellPool.deallocate_node(cell);
			} else {
				_kmap.erase(TopIter);
			}

			// decrease cell counts
			--_kcellCounts[Level];
		}
	}

	void KSpatialHash::findSplitPos(std::pair<SplitDir, SplitDir> &Dir, const KVector2U32 &Pos, U8 Level) {
		// determine cell position
		U32 row = Pos.x / _kcellSize[Level];
		U32 col = Pos.y / _kcellSize[Level];

		if (row % 2 == 0) {
			Dir.first = SplitDir::LEFT;
		} else {
			Dir.first = SplitDir::Right;
		}

		// even col = bottom, positive odd col = top
		if (col % 2 == 0) {
			Dir.second = SplitDir::BOT;
		} else {
			Dir.second = SplitDir::TOP;
		}
	}

	void KSpatialHash::findUniquePoints(std::vector<KVector2U32> &Points, const KRectU32 &aabb, U8 level) {
		static std::vector<KVector2U32> uniquePoints(4);
		uniquePoints.resize(0);
		Points.resize(0);
		KVector2U32 p;

		U32 left = aabb.left / _kcellSize[level];
		U32 right = aabb.right / _kcellSize[level];
		U32 bot = aabb.bottom / _kcellSize[level];
		U32 top = aabb.top / _kcellSize[level];

		// bottom-left cell
		p.x = left;
		p.y = bot;
		uniquePoints.push_back(p);
		Points.push_back(KVector2U32(aabb.left, aabb.bottom));

		// top-left cell
		p.x = left;
		p.y = top;
		if (std::find(uniquePoints.begin(), uniquePoints.end(), p) == uniquePoints.end()) {
			uniquePoints.push_back(p);
			Points.push_back(KVector2U32(aabb.left, aabb.top));
		}

		// bottom-right cell
		p.x = right;
		p.y = bot;
		if (std::find(uniquePoints.begin(), uniquePoints.end(), p) == uniquePoints.end()) {
			uniquePoints.push_back(p);
			Points.push_back(KVector2U32(aabb.right, aabb.bottom));
		}

		// top-right cell
		p.x = right;
		p.y = top;
		if (std::find(uniquePoints.begin(), uniquePoints.end(), p) == uniquePoints.end()) {
			uniquePoints.push_back(p);
			Points.push_back(KVector2U32(aabb.right, aabb.top));
		}
	}

	void KSpatialHash::findUniqueCells(std::vector<KVector2U32> &Points, const KRectU32 &aabb, U8 level) {
		Points.resize(0);

		KVector2U32 p;
		U32 left = aabb.left / _kcellSize[level];
		U32 right = aabb.right / _kcellSize[level];
		U32 bot = aabb.bottom / _kcellSize[level];
		U32 top = aabb.top / _kcellSize[level];

		// bottom-left cell
		p.x = left;
		p.y = bot;
		Points.push_back(p);

		// top-left cell
		p.x = left;
		p.y = top;
		if (std::find(Points.begin(), Points.end(), p) == Points.end()) {
			Points.push_back(p);
		}

		// bottom-right cell
		p.x = right;
		p.y = bot;
		if (std::find(Points.begin(), Points.end(), p) == Points.end()) {
			Points.push_back(p);
		}

		// top-right cell
		p.x = right;
		p.y = top;
		if (std::find(Points.begin(), Points.end(), p) == Points.end()) {
			Points.push_back(p);
		}
	}

	void KSpatialHash::lineIntersectPoints(std::vector<KVector2I32> &OutPoints, const KVector2F32 &Start, const KVector2F32 &End, U8 Level) {
		OutPoints.resize(0);

		float sx = Start.x / _kcellSize[Level];
		float sy = Start.y / _kcellSize[Level];
		float ex = End.x / _kcellSize[Level];
		float ey = End.y / _kcellSize[Level];

		const F32 dx = fabs(ex - sx);
		const F32 dy = fabs(ey - sy);
		const auto ch = _kcellSize[Level] / 2;

		U32 x = int(std::floor(sx));
		U32 y = int(std::floor(sy));

		int n = 1;
		int x_inc, y_inc;
		F32 error;

		if (dx == 0) {
			x_inc = 0;
			error = std::numeric_limits<F32>::infinity();
		} else if (ex > sx) {
			x_inc = 1;
			n += int(floor(ex)) - x;
			error = (floor(sx) + 1 - sx) * dy;
		} else {
			x_inc = -1;
			n += x - int(floor(ex));
			error = (sx - floor(sx)) * dy;
		}

		if (dy == 0) {
			y_inc = 0;
			error -= std::numeric_limits<F32>::infinity();
		} else if (ey > sy) {
			y_inc = 1;
			n += int(floor(ey)) - y;
			error -= (floor(sy) + 1 - sy) * dx;
		} else {
			y_inc = -1;
			n += y - int(floor(ey));
			error -= (sy - floor(sy)) * dx;
		}

		for (; n > 0; --n) {
			OutPoints.push_back(KVector2I32((x * _kcellSize[Level]) + ch, (y * _kcellSize[Level] + ch)));

			if (error > 0) {
				y += y_inc;
				error -= dx;
			} else {
				x += x_inc;
				error += dy;
			}
		}
	}

	void KSpatialHash::addPoints(NodeData *Data, const std::vector<KVector2U32> &Points, U8 Level) {
		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		for (auto it = Points.begin(); it != Points.end(); ++it) {
			for (U8 i = 0; i <= Level; ++i) {
				// top-level cell 
				// compare with the previous top-level cell for avoiding extra hash lookup
				if (i == 0) {
					pos.first = it->x / _kcellSize[i];
					pos.second = it->y / _kcellSize[i];
					if (pos != topLvlpos) {
						topLvlpos = pos;
						U64 key = hashPair(pos.first, pos.second);
						auto mit = _kmap.insert({ key, Cell() });

						// cell created
						if (mit.second) {
							++_kcellCounts[i];
						}

						topLvlCell = &mit.first->second;
					}
					parent = topLvlCell;

				} else { // sub-level cell
						 // allocate splitter for parent level
					if (!parent->split) {
						parent->split = new(_ksplitPool.allocate_node()) KSpatialHash::Splitter;
					}

					// determine cell direction in the splitter
					std::pair<SplitDir, SplitDir> sdir;
					findSplitPos(sdir, *it, i);

					// create and attach sub-level cell to the parent splitter
					if (!parent->split->link[(U8)sdir.first][(U8)sdir.second]) {
						parent->split->link[(U8)sdir.first][(U8)sdir.second] = new(_kcellPool.allocate_node()) KSpatialHash::Cell;
						++_kcellCounts[i];
					}

					// set parent
					parent->split->link[(U8)sdir.first][(U8)sdir.second]->parent = parent;

					// go to the next level
					parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
				}
			}

			auto node = new(_knodePool.allocate_node()) KSpatialHash::Node;
			node->data = Data;
			node->next = parent->nodes;
			parent->nodes = node;
		}
	}

	void KSpatialHash::removePoints(NodeData *Data, const std::vector<KVector2U32> &Points, U8 Level) {
		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		auto found = _kmap.end();

		for (auto it = Points.begin(); it != Points.end(); ++it) {
			for (U8 i = 0; i <= Level; ++i) {
				// top-level cell 
				// compare with the previous top-level cell for avoiding extra hash lookup
				if (i == 0) {
					pos.first = it->x / _kcellSize[i];
					pos.second = it->y / _kcellSize[i];
					if (pos != topLvlpos) {
						topLvlpos = pos;
						U64 key = hashPair(pos.first, pos.second);
						found = _kmap.find(key);
						if (found == _kmap.end()) {
							topLvlCell = nullptr;
						} else {
							topLvlCell = &found->second;
						}
					}
					parent = topLvlCell;

				} else {
					// check link
					findSplitPos(sdir, *it, i);

					// next level
					parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
				}
			}

			// iterate over nodes list
			Node *node = parent->nodes;
			Node *prev = nullptr;
			while (node) {
				if (node->data == Data) {
					// remove node fromlist
					if (!prev) {
						// begin
						parent->nodes = node->next;
						if (!parent->nodes) removeCell(parent, Level, found); // checking cell for cleanup
					} else {
						// middle or end
						prev->next = node->next;
					}

					node->data = nullptr;
					node->next = nullptr;
					_knodePool.deallocate_node(node);
					break;
				}
				prev = node;
				node = node->next;
			}
		}
	}

	KSHObject KSpatialHash::add(const KRectI32 &aabb, U16 Tag, void *userData) {
		// allocate new node
		auto nodeData = new(_knodeDataPool.allocate_node()) KSpatialHash::NodeData;
		nodeData->owner = this;
		nodeData->userData = userData;
		nodeData->tag = Tag;

		// convert from the signed range to the unsighned range
		nodeData->aabb.left = aabb.left + INT32_MAX;
		nodeData->aabb.right = aabb.right + INT32_MAX;
		nodeData->aabb.top = aabb.top + INT32_MAX;
		nodeData->aabb.bottom = aabb.bottom + INT32_MAX;

		KSHObject obj;
		obj.nodeData = nodeData;

		// determine object level
		U32 max = nodeData->aabb.right - nodeData->aabb.left;
		if (max < nodeData->aabb.top - nodeData->aabb.bottom) max = nodeData->aabb.top - nodeData->aabb.bottom;
		if (max > _kcellSize[0]) {
			return KSHObject();
		}
		U8 level = _klevel;
		for (I16 i = _klevel; i >= 0; --i) {
			if (_kcellSize[i] >= max) {
				level = i;
				break;
			}
		}

		nodeData->level = level;

		// find each unique point of the object's aabb at the given level.
		// avoiding duplication in same cell
		static std::vector<KVector2U32> points(4);
		findUniquePoints(points, nodeData->aabb, level);

		// add points to the appropriate cell
		addPoints(nodeData, points, level);

		++_kobjCounts[level];
		return obj;
	}

	void KSpatialHash::remove(KSHObject &Object) {
		// checking object
		if (!Object.nodeData) {
			return;
		}
		auto nodeData = static_cast<KSpatialHash::NodeData *>(Object.nodeData);
		if (nodeData->owner != this) {
			return;
		}

		// find each unique point of the object's aabb at the given level.
		// avoiding duplication in same cell
		static std::vector<KVector2U32> points(4);
		findUniquePoints(points, nodeData->aabb, nodeData->level);

		// remove points from cells
		removePoints(nodeData, points, nodeData->level);

		--_kobjCounts[nodeData->level];

		// free node data
		nodeData->owner = nullptr;
		nodeData->userData = nullptr;
		nodeData->tick = 0;
		_knodeDataPool.deallocate_node(nodeData);
		Object.nodeData = nullptr;
	}

	void KSpatialHash::update(KSHObject &Object, const KRectI32 &aabb) {
		// checking object
		if (!Object.nodeData) {
			return;
		}
		auto nodeData = static_cast<KSpatialHash::NodeData *>(Object.nodeData);
		if (nodeData->owner != this) {
			return;
		}

		// determine object level (updated level)
		U32 max = aabb.right - aabb.left;
		if (max < aabb.top - aabb.bottom) max = aabb.top - aabb.bottom;
		if (max > _kcellSize[0]) {
			return;
		}
		U8 level = _klevel;
		for (I16 i = _klevel; i >= 0; --i) {
			if (_kcellSize[i] >= max) {
				level = i;
				break;
			}
		}

		// check level
		if (nodeData->level != level) {
			// save current data
			auto t1 = nodeData->userData;
			auto t2 = nodeData->tag;

			// remove old object
			remove(Object);

			// create new (updated) object with current data
			Object.nodeData = add(aabb, t2, t1).nodeData;
			return;
		}

		// old points
		static std::vector<KVector2U32> oldCells(4);
		findUniqueCells(oldCells, nodeData->aabb, nodeData->level);

		// convert from the signed range to the unsighned range
		KRectU32 uaabb;
		uaabb.left = aabb.left + INT32_MAX;
		uaabb.right = aabb.right + INT32_MAX;
		uaabb.top = aabb.top + INT32_MAX;
		uaabb.bottom = aabb.bottom + INT32_MAX;

		// new (updated) points
		static std::vector<KVector2U32> newCells(4);
		findUniqueCells(newCells, uaabb, level);

		// create or update(reuse) new nodes
		for (U8 i = 0; i < newCells.size();) { // we need to removing items while iteration so we cant use for/loop
											   // checking old list
			auto found = std::find(oldCells.begin(), oldCells.end(), newCells[i]);

			// there is a node at this point, reuse it
			if (found != oldCells.end()) {
				(*found) = oldCells.back();
				oldCells.pop_back();

				newCells[i] = newCells.back();
				newCells.pop_back();
			} else {
				// convert cell to point
				newCells[i].x *= _kcellSize[level];
				newCells[i].y *= _kcellSize[level];

				++i;
			}
		}

		// add new points
		addPoints(nodeData, newCells, level);

		// convert cell to point
		for (auto it = oldCells.begin(); it != oldCells.end(); ++it) {
			it->x *= _kcellSize[level];
			it->y *= _kcellSize[level];
		}

		// remove old points
		removePoints(nodeData, oldCells, level);

		nodeData->aabb = uaabb;
	}

	void KSpatialHash::resetData(KSHObject &Object, U16 Tag, void *userData) {
		// checking object
		if (!Object.nodeData) {
			return;
		}
		auto nodeData = static_cast<KSpatialHash::NodeData *>(Object.nodeData);
		if (nodeData->owner != this) {
			return;
		}

		nodeData->tag = Tag;
		nodeData->userData = userData;
	}

	KRectI32 KSpatialHash::getCellDimension(const KVector2I32 &Position, U8 Level) const {
		KVector2U32 upos(Position.x + INT32_MAX, Position.y + INT32_MAX);

		U32 row = upos.x / _kcellSize[Level];
		U32 col = upos.y / _kcellSize[Level];

		KRectI32 rect;
		rect.left = row * _kcellSize[Level] - INT32_MAX;
		rect.right = rect.left + _kcellSize[Level];
		rect.bottom = col * _kcellSize[Level] - INT32_MAX;
		rect.top = rect.bottom + _kcellSize[Level];

		return rect;
	}

	bool KSpatialHash::checkCell(const KVector2I32 &Position, U8 Level) {
		KVector2U32 upos(Position.x + INT32_MAX, Position.y + INT32_MAX);

		// first, checking top-level cell
		U32 row = upos.x / _kcellSize[0];
		U32 col = upos.y / _kcellSize[0];

		U64 key = hashPair(row, col);
		auto found = _kmap.find(key);
		if (found == _kmap.end()) return false;

		// iterate sub-levels
		Cell *parent = &found->second;
		std::pair<SplitDir, SplitDir> sdir;
		for (U8 i = 1; i <= Level; ++i) {
			// check splitter
			if (!parent->split) return false;

			// check link
			findSplitPos(sdir, upos, i);
			if (!parent->split->link[(U8)sdir.first][(U8)sdir.second]) return false;

			// next level
			parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
		}

		// check self
		if (!parent->nodes) return false;

		return true;
	}

	void KSpatialHash::queryRect(const KRectI32 &Area, U16 Tag, bool CheckShape, SHRectMode Mode, std::vector<void *> &OutUserData) {
		OutUserData.resize(0);

		KRectU32 uaabb;
		uaabb.left = Area.left + INT32_MAX;
		uaabb.right = Area.right + INT32_MAX;
		uaabb.top = Area.top + INT32_MAX;
		uaabb.bottom = Area.bottom + INT32_MAX;

		// collect all cells (at the deepest level) in the queried area
		U32 lrow = uaabb.left / _kcellSize[_klevel];
		U32 rrow = uaabb.right / _kcellSize[_klevel];
		U32 bcol = uaabb.bottom / _kcellSize[_klevel];
		U32 tcol = uaabb.top / _kcellSize[_klevel];

		U32 rowLen = rrow - lrow;
		U32 colLen = tcol - bcol;

		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		KVector2U32 upos(0, 0);
		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		// increase tick
		++_ktick;

		// row
		for (U32 rc = 0; rc <= rowLen; ++rc) {
			// col
			for (U32 cc = 0; cc <= colLen; ++cc) {
				upos.x = (lrow + rc) * _kcellSize[_klevel];
				upos.y = (bcol + cc) * _kcellSize[_klevel];

				for (U8 i = 0; i <= _klevel; ++i) {
					// top-level cell 
					// compare with the previous top-level cell for avoiding extra hash lookup
					if (i == 0) {
						pos.first = upos.x / _kcellSize[i];
						pos.second = upos.y / _kcellSize[i];
						if (pos != topLvlpos) {
							topLvlpos = pos;
							U64 key = hashPair(pos.first, pos.second);
							auto found = _kmap.find(key);
							if (found == _kmap.end()) {
								topLvlCell = nullptr;
							} else {
								topLvlCell = &found->second;
							}
						}
						parent = topLvlCell;
					}

					if (!parent) break;

					// check nodes
					for (auto nit = parent->nodes; nit != nullptr; nit = nit->next) {
						if (nit->data->tag == Tag && nit->data->tick != _ktick) {
							nit->data->tick = _ktick;
							if (CheckShape) {
								if (Mode == SHRectMode::INSIDE) {
									if (uaabb.contineRect(nit->data->aabb)) {
										OutUserData.push_back(nit->data->userData);
									}
								} else {
									if (uaabb.overlapRect(nit->data->aabb)) {
										OutUserData.push_back(nit->data->userData);
									}
								}

							} else {
								OutUserData.push_back(nit->data->userData);
							}
						}
					}


					// go to sub-level cell
					if (!parent->split) break;
					findSplitPos(sdir, upos, i + 1);
					parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
				}

			}
		}
	}

	void KSpatialHash::queryPoints(const std::vector<KVector2I32> &Points, U16 Tag, bool CheckShape, std::vector<void *> &OutUserData) {
		OutUserData.resize(0);

		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		KVector2U32 upos(0, 0);
		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		// increase tick
		++_ktick;

		for (auto it = Points.begin(); it != Points.end(); ++it) {
			upos.x = it->x + INT32_MAX;
			upos.y = it->y + INT32_MAX;

			for (U8 i = 0; i <= _klevel; ++i) {
				// top-level cell 
				// compare with the previous top-level cell for avoiding extra hash lookup
				if (i == 0) {
					pos.first = upos.x / _kcellSize[i];
					pos.second = upos.y / _kcellSize[i];
					if (pos != topLvlpos) {
						topLvlpos = pos;
						U64 key = hashPair(pos.first, pos.second);
						auto found = _kmap.find(key);
						if (found == _kmap.end()) {
							topLvlCell = nullptr;
						} else {
							topLvlCell = &found->second;
						}
					}
					parent = topLvlCell;
				}

				if (!parent) break;

				// check nodes
				for (auto nit = parent->nodes; nit != nullptr; nit = nit->next) {
					if (nit->data->tag == Tag && nit->data->tick != _ktick) {
						nit->data->tick = _ktick;
						if (CheckShape) {
							if (nit->data->aabb.continePoint(upos)) {
								OutUserData.push_back(nit->data->userData);
							}
						} else {
							OutUserData.push_back(nit->data->userData);
						}
					}
				}


				// go to sub-level cell
				if (!parent->split) break;
				findSplitPos(sdir, upos, i + 1);
				parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
			}
		}
	}

	void KSpatialHash::queryLine(const KVector2I32 &StartPoint, const KVector2I32 &EndPoint, U16 Tag, bool CheckShape, std::vector<void *> &OutUserData) {
		OutUserData.resize(0);

		static std::vector<KVector2I32> Points;
		const KVector2F32 fstart((F32)StartPoint.x, (F32)StartPoint.y);
		const KVector2F32 fend((F32)EndPoint.x, (F32)EndPoint.y);
		lineIntersectPoints(Points, fstart, fend, _klevel);

		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		KVector2U32 upos(0, 0);
		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		KRectI32 irect;

		// increase tick
		++_ktick;

		for (auto it = Points.begin(); it != Points.end(); ++it) {
			upos.x = it->x + INT32_MAX;
			upos.y = it->y + INT32_MAX;

			for (U8 i = 0; i <= _klevel; ++i) {
				// top-level cell 
				// compare with the previous top-level cell for avoiding extra hash lookup
				if (i == 0) {
					pos.first = upos.x / _kcellSize[i];
					pos.second = upos.y / _kcellSize[i];
					if (pos != topLvlpos) {
						topLvlpos = pos;
						U64 key = hashPair(pos.first, pos.second);
						auto found = _kmap.find(key);
						if (found == _kmap.end()) {
							topLvlCell = nullptr;
						} else {
							topLvlCell = &found->second;
						}
					}
					parent = topLvlCell;
				}

				if (!parent) break;

				// check nodes
				for (auto nit = parent->nodes; nit != nullptr; nit = nit->next) {
					if (nit->data->tag == Tag && nit->data->tick != _ktick) {
						nit->data->tick = _ktick;
						if (CheckShape) {
							irect.left = nit->data->aabb.left - INT32_MAX;
							irect.right = nit->data->aabb.right - INT32_MAX;
							irect.bottom = nit->data->aabb.bottom - INT32_MAX;
							irect.top = nit->data->aabb.top - INT32_MAX;
							if (irect.intersectLine(fstart, fend)) {
								OutUserData.push_back(nit->data->userData);
							}
						} else {
							OutUserData.push_back(nit->data->userData);
						}
					}
				}


				// go to sub-level cell
				if (!parent->split) break;
				findSplitPos(sdir, upos, i + 1);
				parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
			}
		}
	}

	void KSpatialHash::queryCircle(const KVector2I32 &Center, U32 Radius, U16 Tag, std::vector<void *> &OutUserData) {
		OutUserData.resize(0);

		KRectU32 uaabb;
		uaabb.left = (Center.x + INT32_MAX) - Radius;
		uaabb.right = (Center.x + INT32_MAX) + Radius;
		uaabb.top = (Center.y + INT32_MAX) + Radius;
		uaabb.bottom = (Center.y + INT32_MAX) - Radius;

		KRectI32 irect;

		// collect all cells (at the deepest level) in the queried area
		U32 lrow = uaabb.left / _kcellSize[_klevel];
		U32 rrow = uaabb.right / _kcellSize[_klevel];
		U32 bcol = uaabb.bottom / _kcellSize[_klevel];
		U32 tcol = uaabb.top / _kcellSize[_klevel];

		U32 rowLen = rrow - lrow;
		U32 colLen = tcol - bcol;

		Cell *topLvlCell = nullptr;
		Cell *parent = nullptr;

		KVector2U32 upos(0, 0);
		std::pair<U32, U32> pos(0, 0);
		std::pair<U32, U32> topLvlpos(0, 0);
		std::pair<SplitDir, SplitDir> sdir;

		// increase tick
		++_ktick;

		// row
		for (U32 rc = 0; rc <= rowLen; ++rc) {
			// col
			for (U32 cc = 0; cc <= colLen; ++cc) {
				upos.x = (lrow + rc) * _kcellSize[_klevel];
				upos.y = (bcol + cc) * _kcellSize[_klevel];

				for (U8 i = 0; i <= _klevel; ++i) {
					// top-level cell 
					// compare with the previous top-level cell for avoiding extra hash lookup
					if (i == 0) {
						pos.first = upos.x / _kcellSize[i];
						pos.second = upos.y / _kcellSize[i];
						if (pos != topLvlpos) {
							topLvlpos = pos;
							U64 key = hashPair(pos.first, pos.second);
							auto found = _kmap.find(key);
							if (found == _kmap.end()) {
								topLvlCell = nullptr;
							} else {
								topLvlCell = &found->second;
							}
						}
						parent = topLvlCell;
					}

					if (!parent) break;

					// check nodes
					for (auto nit = parent->nodes; nit != nullptr; nit = nit->next) {
						if (nit->data->tag == Tag && nit->data->tick != _ktick) {
							nit->data->tick = _ktick;
							irect.left = nit->data->aabb.left - INT32_MAX;
							irect.right = nit->data->aabb.right - INT32_MAX;
							irect.bottom = nit->data->aabb.bottom - INT32_MAX;
							irect.top = nit->data->aabb.top - INT32_MAX;
							if (irect.intersectCircle(KVector2F32(Center.x, Center.y), (F32)Radius)) {
								OutUserData.push_back(nit->data->userData);
							}
						}
					}


					// go to sub-level cell
					if (!parent->split) break;
					findSplitPos(sdir, upos, i + 1);
					parent = parent->split->link[(U8)sdir.first][(U8)sdir.second];
				}

			}
		}
	}
}