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
        /// initialize
        static bool inite();

        /// clear the scene
        static void clear();

        /// draw (range)
        static void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive);

        /// draw instanced (range)
        static void draw(U32 FirstIndex, U32 Size, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

        /// draw (index)
        static void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive);

        /// draw instanced (index)
        static void draw(U32 Count, const U16 *Indices, KGeoPrimitiveTypes Primitive, U32 InstanceCount);

        /// clear color
        static void setClearColor(const KColor &Color);

        /// set line/point option (size)
        /// only KGP_POINTS and KGP_LINES is valid for type
        static void setPointSize(F32 Size);

		/// render in wireframe mode
		static void setWireframe(bool Wireframe);

        /// viewport
        static void setViewport(const KRectU32 &Viewport);
        static const KRectU32 &getViewport();

//        void setBlendMode(KBlendMode BlendMode);
    private:
        KRender();
        ~KRender();
        static const U32 geoTypes[11];
        static KRectU32 _kviewport;
    };
}

#endif // KRENDER_H
