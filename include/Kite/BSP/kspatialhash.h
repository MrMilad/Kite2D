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
#ifndef KSPATIALHASH_H
#define KSPATIALHASH_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/bsp/kbspstructs.h"
#include "Kite/bsp/kbsptypes.h"
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/namespace_alias.hpp>
#include <unordered_map>
#include <luaintf/LuaIntf.h>
#include <vector>
#include "kspatialhash.khgen.h"

KMETA
namespace Kite{
#define KSHASH_NODE_POOL 256
#define KSHASH_CELL_POOL 256
#define KSHASH_HASH_POOL 256
#define KSHASH_SPLIT_POOL 256
#define KSHASH_NODEDATA_POOL 256

	struct KSHObject;
	KM_CLASS(SCRIPTABLE)
	class KITE_FUNC_EXPORT KSpatialHash {
	public:
		/// CellSize is size of the one side of the top-level cell
		/// only the top-level cells will be saved in the hash-table and sub-levels will be sotred inside 2d arrays
		/// each level divide the parent area into 4 subchilds
		/// note: each object in any state (rotated, scaled, ...) must fit into an top-level cell. otherwise undefined behavior
		/// note: using an power of two as CellSize can improve accuracy
		KSpatialHash(U32 CellSize, U8 Levels);

		~KSpatialHash();

		/// add an object to the appropriate cell.
		/// cells can act as stack.
		/// save handle for later use (remove or update)
		KSHObject add(const KRectI32 &aabb, U16 Tag, void *userData);

		/// remove object from associated cell
		/// object will remove even with changed position
		void remove(KSHObject &Object);

		/// update object(s) if its bounding-box/layer/filter was changed
		void update(KSHObject &Object, const KRectI32 &aabb);

		void resetData(KSHObject &Object, U16 Tag, void *userData);

		inline U8 getLevels() const { return _klevel; }

		/// number of objects in each level
		inline U32 getObjectCounts(U8 Level) const { if (Level <= _klevel) return _kobjCounts[Level]; return 0; }

		/// number of active cells in each level
		inline U32 getCellCounts(U8 Level) const { if (Level <= _klevel) return _kcellCounts[Level]; return 0; }

		inline U32 getCellSize(U8 Level) const { if (Level <= _klevel) return _kcellSize[Level]; return 0; }

		KRectI32 getCellDimension(const KVector2I32 &Position, U8 Level) const;

		bool KSpatialHash::checkCell(const KVector2I32 &Position, U8 Level);

		void queryRect(const KRectI32 &Area, U16 Tag, bool CheckShape, SHRectMode Mode, std::vector<void *> &OutUserData);

		void queryPoints(const std::vector<KVector2I32> &Points, U16 Tag, bool CheckShape, std::vector<void *> &OutUserData);

		void queryLine(const KVector2I32 &StartPoint, const KVector2I32 &EndPoint, U16 Tag, bool CheckShape, std::vector<void *> &OutUserData);

		void queryCircle(const KVector2I32 &Center, U32 Radius, U16 Tag, std::vector<void *> &OutUserData);

	private:
		enum class SplitDir : U8 {
			TOP = 0,
			BOT = 1,
			LEFT = 0,
			Right = 1
		};

		struct Cell;
		struct NodeData {
			KSpatialHash *owner = nullptr;
			void *userData = nullptr;
			KRectU32 aabb;
			U32 tick = 0; // equal with current tick for avoiding duplicate checking
			U16 tag = 0;
			U8 level = 0;
		};

		struct Node {
			NodeData *data;
			Node *next = nullptr;
		};

		struct Splitter {
			Cell *link[2][2] = { { nullptr, nullptr },{ nullptr, nullptr } };
		};

		struct Cell {
			Cell *parent = nullptr;
			Node *nodes = nullptr;
			Splitter *split = nullptr;
		};

		// OutDir <row, col>
		void findSplitPos(std::pair<SplitDir, SplitDir> &OutDir, const KVector2U32 &Pos, U8 Level);
		void findUniquePoints(std::vector<KVector2U32> &OutPoints, const KRectU32 &aabb, U8 level);
		void findUniqueCells(std::vector<KVector2U32> &OutPoints, const KRectU32 &aabb, U8 level);
		void lineIntersectPoints(std::vector<KVector2I32> &OutPoints, const KVector2F32 &Start, const KVector2F32 &End, U8 Level);
		void removeCell(Cell *cell, U8 Level, std::unordered_map<U64, Cell>::iterator &TopIter);
		void addPoints(NodeData *Data, const std::vector<KVector2U32> &Points, U8 Level);
		void removePoints(NodeData *Data, const std::vector<KVector2U32> &Points, U8 Level);

		U8 _klevel;
		U32 *_kcellSize; // size of cells in each level (array)
		U32 *_kcellCounts;
		U32 *_kobjCounts;
		U32 _ktick = 0;
		std::unordered_map<U64, Cell> _kmap;
		
		memory::memory_pool<> _knodePool = memory::memory_pool<>(sizeof(Node), KSHASH_NODE_POOL * sizeof(Node));
		memory::memory_pool<> _knodeDataPool = memory::memory_pool<>(sizeof(NodeData), KSHASH_NODEDATA_POOL * sizeof(NodeData));
		memory::memory_pool<> _kcellPool = memory::memory_pool<>(sizeof(Cell), KSHASH_CELL_POOL * sizeof(Cell));
		memory::memory_pool<> _ksplitPool = memory::memory_pool<>(sizeof(Splitter), KSHASH_SPLIT_POOL * sizeof(Splitter)); 
	};

	KM_CLASS(POD)
	struct KSHObject {
		friend KSpatialHash;
	private:
		void *nodeData = nullptr; // nodeData pointer
	};
}

#endif // KSPATIALHASH_H

