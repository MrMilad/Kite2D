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
#include "Kite/Assist/graphic/kindexbatchobject.h"
#include <cstring>
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KText : public KIndexBatchObject{
	public:
		KText(U32 MaxSize);
		KText(const std::string &Text, const std::vector<KAtlasObject> &Font, const KColor &Color);

		void setFont(const std::vector<KAtlasObject> &Font);
        inline const std::vector<KAtlasObject> &getFont() const {return *_kfont;}

		void setText(const std::string &Text);
        inline const std::string &getText() const {return _ktext;}

		void setMiddleSpace(U32 Size);
		inline U32 getMiddleSpace() const { return _kmid; }

		void setColor(const KColor &Color);
		inline const KColor &getColor() const { return _kcolor; }

		inline F32 getWidth() const { return _kwidth; }

    private:
		void _fillIndex();
		void _reshape();
        const std::vector<KAtlasObject> *_kfont;
        std::string _ktext;
		F32 _kwidth;
		U32 _ksize;
		U32 _kmid;
		KColor _kcolor;
    };
}

#endif // KTEXT_H
