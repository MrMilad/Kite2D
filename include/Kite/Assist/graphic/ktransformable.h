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
#ifndef KTRANSFORMABLE_H
#define KTRANSFORMABLE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/math/ktransform.h"
#include "Kite/Core/math/kmatrix3.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTransformable{
    public:
        KTransformable();
        virtual ~KTransformable();
        /// set position
        /// completely restore the previous position
        /// default (0, 0)
        void setPosition(const KVector2F32& Position);
        inline const KVector2F32 *getPosition() const {return &_kposition;}

        /// move object
        /// adds to the current position
        void move(const KVector2F32 &Steps);

        /// completely restore the previous Rotation
        /// range [0, 360]
        /// default 0
        void setRotation(F32 Angle);
        inline F32 getRotation() const {return _krotation;}

        /// adds to the current rotation
        void rotate(F32 Angle);

        /// set scale
        /// completely restore the previous scale
        /// default (1, 1)
        void setScale(const KVector2F32 &Scale);
        inline const KVector2F32 *getScale() const {return &_kscale;}

        /// multiplies the current scale
        void scale(const KVector2F32 &Scale);

        /// set center of (position, scale, rotation)
        /// relative to the top-left
        /// default (0, 0)
        void setCenter(const KVector2F32 &Center);
        inline const KVector2F32 *getCenter() const {return &_kcenter;}

		/// combining the position/rotation/scale/center
		const KTransform *getTransform() const;

    private:
        KVector2F32 _kposition;
        F32 _krotation;
        KVector2F32 _kscale;
        KVector2F32 _kcenter;
        mutable KTransform _ktransform;
        mutable bool _kneedUpdate;
    };
}

#endif // KTRANSFORMABLE_H
