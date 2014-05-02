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
#include "Kite/Assist/graphic/kcamera.h"

namespace Kite{
    class KITE_FUNC_EXPORT KDrawable{
    public:
		KDrawable();
		virtual ~KDrawable();

        virtual void draw() = 0;

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
