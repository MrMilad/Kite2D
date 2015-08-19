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
#ifndef KCAMERA_H
#define KCAMERA_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/math/ktransform.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "kite/Assist/graphic/kanimeobject.h"


namespace Kite{
	class KITE_FUNC_EXPORT KCamera : public KAnimeObject{
    public:
        KCamera();
        KCamera(const KRectI32 &Viewport);

		void setViewport(const KRectI32 &Viewport);

		/// default: 0.0, 0.0
		inline void setCenter(const KVector2I32 &Center) { _kcenter = Center; _kneedUpdate = true; }
		inline const KVector2I32 &getCenter() const { return _kcenter; }
		inline void move(const KVector2I32 &Move){ _kcenter += Move;  _kneedUpdate = true; }

		/// default: 0.0
		inline void setRotation(F32 Angle) { _krotation = Angle; _kneedUpdate = true; }
		inline F32 getRotation() const { return _krotation; }
		inline void rotate(F32 Angle) {_krotation += Angle;  _kneedUpdate = true; }

		/// default: 1.0
		inline void setZoom(F32 Factor) { _kzoom = Factor; _kneedUpdate = true; }
		inline F32 getZoom() const { return _kzoom; }

		/// flip screen coordinate
		inline void flipY() { _kflipy = _kflipy * -1; _kneedUpdate = true; } // (Micro$oft window system)
		inline void flipX() { _kflipx = _kflipx * -1; _kneedUpdate = true; }
        
		const KTransform *getTransform() const;

	protected:
		// from KAnimeObject (input/recive)
		void animeUpdate(const KAnimeValue *AnimatedValue);

    private:
		KVector2I32 _ksize;
        KVector2I32 _kcenter;
        F32 _krotation;
        F32 _kzoom;
		F32 _kflipy;
		F32 _kflipx;
		mutable bool _kneedUpdate;
        mutable KTransform _ktransform;
    };
}
#endif // KCAMERA_H
