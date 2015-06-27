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
#ifndef KDRAWABLE_H
#define KDRAWABLE_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Assist/graphic/kcamera.h"

namespace Kite{
    class KITE_FUNC_EXPORT KDrawable{
    public:
		KDrawable();
		virtual ~KDrawable();

        virtual void draw() = 0;
		virtual void draw(U32 FirstIndex, U32 Size) = 0;

		inline void setCamera(const KCamera &Camera) { _kcam = &Camera; }
		inline const KCamera &getCamera() const { return *_kcam; }

        inline void setVisible(bool Visible) {_kvisible = Visible;}
        inline bool getVisible() const {return _kvisible;}

    protected:
		const KCamera *_kcam;
		static const KCamera _kdefcam;
        bool _kvisible;
    };
}

#endif // KDRAWABLE_H
