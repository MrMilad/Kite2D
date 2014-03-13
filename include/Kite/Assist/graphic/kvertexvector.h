/*
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KVERTEXVECTOR_H
#define KVERTEXVECTOR_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Core/system/kvector2.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KVertexVector{
    public:
        KVertexVector();
        ~KVertexVector();
        /// add vertex data
        /// input: Vertex
        /// output: Index
        //void addVertex(const std::vector<KVertex> &Vertex, std::vector<U32> &Index);

        /// add vertex data
        /// input: Vertex
        /// output: Range
        /// start: Range.x
        /// lenght: Range.y
        void addVertex(const std::vector<KVertex> &Vertex, KVector2U32 &Range);

        /// change stored vertex data
        //void changeVertex(const std::vector<KVertex> &NewVertex, const std::vector<U32> &Index);

        /// update stored vertex data
        /// start: Range.x
        /// lenght: Range.y
        void updateVertex(const std::vector<KVertex> &NewVertex, const KVector2U32 &Range);

        /// lock buffer
        /// only update is valid with locked buffer
        inline void setLock(bool Lock) {_klock = Lock;}
        inline bool getLock() const {return _klock;}

        /// get size of buffer
        inline U32 getSize() const {return _kdata.size();}
        inline const KVertex *getVertex() const {if (_kdata.empty()) return 0; return &_kdata[0];}

    private:
        std::vector<KVertex> _kdata;
        bool _klock;
    };
}

#endif // KVERTEXVECTOR_H
