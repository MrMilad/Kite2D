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
		void setPosition(F32 X, F32 Y);
        inline const KVector2F32 *getPosition() const {return &_kposition;}

        /// move object
        /// adds to the current position
        void move(const KVector2F32 &Steps);
		void move(F32 X, F32 Y);

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
		void setScale(F32 X, F32 Y);
        inline const KVector2F32 *getScale() const {return &_kscale;}

        /// multiplies the current scale
        void scale(const KVector2F32 &Scale);
		void scale(F32 X, F32 Y);

		void setSkew(const KVector2F32 &Skew);
		void setSkew(F32 X, F32 Y);
		inline const KVector2F32 *getSkew() const { return &_kskew; }

		void skew(const KVector2F32 &Skew);
		void skew(F32 X, F32 Y);

        /// set center of (position, scale, rotation)
        /// relative to the top-left
        /// default (0, 0)
        void setCenter(const KVector2F32 &Center);
		void setCenter(F32 X, F32 Y);
        inline const KVector2F32 *getCenter() const {return &_kcenter;}

		/// combining the position/rotation/scale/center
		const KTransform *getTransform() const;

    private:
        KVector2F32 _kposition;
        F32 _krotation;
        KVector2F32 _kscale;
		KVector2F32 _kskew;
        KVector2F32 _kcenter;
        mutable KTransform _ktransform;
        mutable bool _kneedUpdate;
    };
}

#endif // KTRANSFORMABLE_H
