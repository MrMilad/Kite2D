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
#include "Kite/Assist/graphic/kvertexvector.h"
#include <algorithm>


namespace Kite{

//    void KVertexVector::addVertex(const std::vector<KVertex> &Vertex, std::vector<U32> &Index){
//        // just in case
//        Index.clear();

//        // store position of last elemnt
//        U32 lastPos = _kdata.size();

//        // calculate enough size
//        U32 needSize = lastPos + Vertex.size();

//        // allocate enough size
//        _kdata.reserve(needSize);

//        // copy new data
//        _kdata.insert(_kdata.end(), Vertex.begin(), Vertex.end());

//        // Index
//        for (U32 i = lastPos; i < needSize; i++){
//            Index.push_back(i);
//        }
//    }

    void KVertexVector::addVertex(const std::vector<KVertex> &Vertex, KVector2U32 &Range){
        // store position of last elemnt
        U32 lastPos = _kdata.size();

        // calculate enough size
        U32 needSize = lastPos + Vertex.size();

        // allocate enough size
        _kdata.reserve(needSize);

        // copy new data
        _kdata.insert(_kdata.end(), Vertex.begin(), Vertex.end());

        // range
        Range.x = lastPos;
        Range.y = Vertex.size();
    }

//    void KVertexVector::changeVertex(const std::vector<KVertex> &NewVertex, const std::vector<U32> &Index){
//        if (_kdata.empty() || Index.size() > NewVertex.size()){
//            KDEBUG_PRINT("buffer is empty or index size is greater than vertex size.");
//            return;
//        }

//        // change data
//        for (U32 i = 0; i < Index.size(); i++){
//            if (Index[i] < _kdata.size()){
//                _kdata[Index[i]] = NewVertex[i];
//            }
//        }
//    }

    void KVertexVector::updateVertex(const std::vector<KVertex> &NewVertex, const KVector2U32 &Range){
        if (_kdata.empty() || Range.y > NewVertex.size()){
            KDEBUG_PRINT("buffer is empty or lenght is greater than vertex size.");
            return;
        }

        if ((Range.x + Range.y) > _kdata.size()){
            KDEBUG_PRINT("out of range");
            return;
        }

        // set iterator to end of range (source)
        std::vector<KVertex>::const_iterator itEnd = NewVertex.begin();
        std::advance(itEnd, Range.y);

        // set iterator to start of range (dest)
        std::vector<KVertex>::iterator itDest = _kdata.begin();
        std::advance(itDest, Range.x);

        // update data
        std::copy(NewVertex.begin(), itEnd, itDest);
    }



}