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
#ifndef KDRAWABLE_H
#define KDRAWABLE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Core/graphic/kgl2drender.h"
#include "Kite/Assist/graphic/kvertexvector.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KDrawable{
    public:
        virtual void pushVertex(KVertexVector &Vector) = 0;
        virtual void update(KVertexBuffer &Buffer) = 0;
        virtual void draw(KGL2DRender &Renderer, const KVertexBuffer &Buffer) = 0;
    };
}

#endif // KDRAWABLE_H
