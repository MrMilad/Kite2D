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
#ifndef KCAMERA_H
#define KCAMERA_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kvector2.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktransform.h"
#include <cmath>

namespace Kite{
    class KITE_FUNC_EXPORT KCamera{
    public:
        KCamera();
        KCamera(const KRectI32 &Viewport);

        inline const KRectI32 &getViewport() const {return _kviewport;}
        inline const KVector2F32 &getCenter() const {return _kcenter;}
        inline F32 getRotation() const {return _krotation;}
        inline F32 getZoom() const {return _kzoom;}

        inline const KTransform &getTransform() const {return _ktransform;}
        KTransform getInverseTransform() const;

        inline void setViewport(const KRectI32 &Viewport) {_kviewport = Viewport;}
        inline void setCenter(const KVector2F32 Center) {_kcenter = Center; _kneedUpdateIn = true;}
        void setRotation(F32 Angle);
        void setZoom(F32 Factor);

        KVector2F32 convertPoint(const KVector2I32 &Point);

    private:
        KRectI32 _kviewport;
        KVector2F32 _kcenter;
        F32 _krotation;
        F32 _kzoom;
        //mutable bool _kneedUpdateTr;
        mutable bool _kneedUpdateIn;
        mutable KTransform _ktransform;
        mutable KTransform _ktransformIn;
    };
}
#endif // KCAMERA_H
