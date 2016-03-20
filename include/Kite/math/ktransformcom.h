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
#ifndef KTRANSFORMCOM_H
#define KTRANSFORMCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcomponent.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/kmatrix3.h"

namespace Kite{
    class KITE_FUNC_EXPORT KTransformCom: public KComponent{
		friend class KTransformSys;
    public:
		/// construct a transform (unit matrix)
		KTransformCom(const std::string &Name);

		void attached(U32 EntityID) override;

		void deattached(U32 EntityID) override;

        /// set position
        /// completely restore the previous position
        /// default (0, 0)
        void setPosition(const KVector2F32& Position);
        inline const KVector2F32 &getPosition() const {return _kposition;}

        /// completely restore the previous Rotation
        /// range [0, 360]
        /// default 0
        void setRotation(F32 Angle);
        inline F32 getRotation() const {return _krotation;}


        /// set scale
        /// completely restore the previous scale
        /// default (1, 1)
        void setScale(const KVector2F32 &Scale);
        inline const KVector2F32 &getScale() const {return _kscale;}

		/// set scale
		/// completely restore the previous scale
		/// default (0, 0)
		void setSkew(const KVector2F32 &Skew);
		inline const KVector2F32 &getSkew() const { return _kskew; }

        /// set center of (position, scale, rotation)
        /// relative to the top-left
        /// default (0, 0)
        void setCenter(const KVector2F32 &Center);
        inline const KVector2F32 &getCenter() const {return _kcenter;}

		/// return the combined matrix
		inline const KMatrix3 *getMatrix() const { return &_kmatrix; }

		/// return the combined matrix for direct manipulation
		inline KMatrix3 *manipulateMatrix() { return &_kmatrix; }

    private:
        KVector2F32 _kposition;
        F32 _krotation;
        KVector2F32 _kscale;
		KVector2F32 _kskew;
        KVector2F32 _kcenter;
		KMatrix3 _kmatrix;
    };
}

#endif // KTRANSFORMCOM_H
