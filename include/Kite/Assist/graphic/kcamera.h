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
#include "Kite/Core/math/kmatrix3.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/graphic/kgraphicstructs.h"

namespace Kite{
    class KITE_FUNC_EXPORT KCamera{
    public:
        KCamera();
        KCamera(const KRectF32 &Viewport);

		void setViewport(const KRectF32 &Viewport);

		/// default: 0.0, 0.0
		inline void setCenter(const KVector2F32 Center) { _kcenter = Center; _kneedUpdate = true; }
		inline const KVector2F32 &getCenter() const { return _kcenter; }

		/// default: 0.0
		inline void setRotation(F32 Angle) { _krotation = Angle; _kneedUpdate = true; }
		inline F32 getRotation() const { return _krotation; }

		/// default: 1.0
		inline void setZoom(F32 Factor) { _kzoom = Factor; _kneedUpdate = true; }
		inline F32 getZoom() const { return _kzoom; }

		// flip screen coordinate
		inline void flipY() { _kflipy = _kflipy * -1; _kneedUpdate = true; } // flip y coordinate in Microsoft Windows OS
		inline void flipX() { _kflipx = _kflipx * -1; _kneedUpdate = true; }
        
		const KMatrix3 &getMatrix() const;
    private:
		KVector2F32 _ksize;
        KVector2F32 _kcenter;
        F32 _krotation;
        F32 _kzoom;
		F32 _kflipy;
		F32 _kflipx;
		mutable bool _kneedUpdate;
        mutable KMatrix3 _kmatrix;
    };
}
#endif // KCAMERA_H
