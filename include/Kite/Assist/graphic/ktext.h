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
#ifndef KTEXT_H
#define KTEXT_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/math/kvector2.h"
#include "Kite/Assist/graphic/kquad.h"
#include <vector>
#include <cstring>

/*namespace Kite{
    class KITE_FUNC_EXPORT KText{
	public:
        KText(const std::vector<KAtlasObject> &Font);
        ~KText();

        inline void setFont(const std::vector<KAtlasObject> &Font) {_kfont = &Font; _kneedUpdate = true;}

        inline const std::vector<KAtlasObject> &getFont() const {return *_kfont;}

        inline void setText(const std::string &Text) {_ktext = Text; _kneedUpdate = true;}

        inline const std::string &getText() const {return _ktext;}

		void setPositon(const KVector2F32 &Position);

		inline const KVector2F32 &getPosition() const { return _kposition; }

		inline F32 getWidth() const { return _kwidth; }

        const std::vector<KQuad> &getQuads();

    private:
        const std::vector<KAtlasObject> *_kfont;
        std::vector<KQuad> _kquads;
        std::string _ktext;
        KVector2F32 _kposition;
		F32 _kwidth;
        bool _kneedUpdate;
    };
}*/

#endif // KTEXT_H
