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
#ifndef KINDEXBATCHOBJECT_H
#define KINDEXBATCHOBJECT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"

namespace Kite{
    class KITE_FUNC_EXPORT KIndexBatchObject{
    public:
        KIndexBatchObject(U32 VertexSize, U32 IndexSize):
            _kvsize(VertexSize),
            _kisize(IndexSize),
            _kvertex(new KVertex[VertexSize]),
            _kindex(new U16[IndexSize])
        {}

        ~KIndexBatchObject(){
            delete[] _kvertex;
            delete[] _kindex;
        }

        virtual const KVertex *getVertex() const = 0;
        inline const U16 *getIndex() const {return _kindex;}

        inline U32 getVertexSize() const { return _kvsize; }
        inline U32 getIndexSize() const { return _kisize; }

    protected:
        mutable KVertex *_kvertex;
        U16 *_kindex;

    private:
        const U32 _kvsize;
        const U32 _kisize;
    };
}

#endif // KINDEXBATCHOBJECT_H
