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
#include "Kite/meta/kmetadef.h"
#include "ktransformcom.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(COMPONENT)
    class KITE_FUNC_EXPORT KTransformCom: public KComponent{
		friend class KTransformSys;
		KM_INFO("CType", "Transform");
		KMETA_KTRANSFORMCOM_BODY();
    public:
		/// construct a transform (unit matrix)
		KTransformCom(const std::string &Name = "");

		void attached() override;

		void deattached() override;

		KRecieveTypes onMessage(KMessage &Message, KMessageScopeTypes Scope) override;

        /// set position
        /// completely restore the previous position
        /// default (0, 0)
		KM_PRO_SET("Position")
        void setPosition(const KVector2F32& Position);

		KM_PRO_GET("Position", KVector2F32, "Position value")
        inline const KVector2F32 &getPosition() const {return _kposition;}

        /// completely restore the previous Rotation
        /// range [0, 360]
        /// default 0
		KM_PRO_SET("Rotation")
        void setRotation(F32 Angle);

		KM_PRO_GET("Rotation", F32, "Rotation value")
        inline F32 getRotation() const {return _krotation;}

        /// set scale
        /// completely restore the previous scale
        /// default (1, 1)
		KM_PRO_SET("Scale")
        void setScale(const KVector2F32 &Scale);

		KM_PRO_GET("Scale", KVector2F32, "Scale value")
        inline const KVector2F32 &getScale() const {return _kscale;}

		/// set scale
		/// completely restore the previous scale
		/// default (0, 0)
		KM_PRO_SET("Skew")
		void setSkew(const KVector2F32 &Skew);

		KM_PRO_GET("Skew", KVector2F32, "Skew value")
		inline const KVector2F32 &getSkew() const { return _kskew; }

        /// set center of (position, scale, rotation)
        /// relative to the top-left
        /// default (0, 0)
		KM_PRO_SET("Center")
        void setCenter(const KVector2F32 &Center);

		KM_PRO_GET("Center", KVector2F32, "Center value")
        inline const KVector2F32 &getCenter() const {return _kcenter;}

		/// return the combined matrix
		KM_FUN()
		inline const KMatrix3 &getMatrix() const { return _kmatrix; }

		/// return the combined matrix for direct manipulation
		KM_FUN()
		inline KMatrix3 &manipulateMatrix() { return _kmatrix; }

    private:
        KM_VAR() KVector2F32 _kposition;
		KM_VAR() F32 _krotation;
		KM_VAR() KVector2F32 _kscale;
		KM_VAR() KVector2F32 _kskew;
		KM_VAR() KVector2F32 _kcenter;
		KM_VAR() KMatrix3 _kmatrix;
    };
}

#endif // KTRANSFORMCOM_H
