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
#ifndef KRENDER_H
#define KRENDER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KRender{
    public:
        KRender(const KRectI32 &Viewport);
        ~KRender();

        /// clear the scene
        void clear();

        /// draw (range)
        void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive);

        /// draw instanced (range)
        void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

        /// draw (index)
        void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive);

        /// draw instanced (index)
        void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

        /// clear color
        void setClearColor(const KColor &Color);

        /// set line/point options (size/smooth)
        /// only KGP_POINTS and KGP_LINES is valid for type
        void setLPSize(KGeoPrimitiveTypes Type, F32 Size);

        /// viewport
        void setViewport(const KRectI32 &Viewport);
        inline KRectI32 getViewport() const {return _kviewport;}

//        void setBlendMode(KBlendMode BlendMode);


    private:
        static const U32 geoTypes[11];
        KRectI32 _kviewport;
        KGeoPrimitiveTypes _kgeoType;
    };
}

#endif // KRENDER_H
