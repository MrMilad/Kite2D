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
#ifndef KIMAGE_H
#define KIMAGE_H

#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <string>
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KImage : KNonCopyable{
    public:
        KImage();
        ~KImage();

        void create(U32 Width, U32 Height, const KColor &Color);
        void create(U32 Width, U32 Height, const U8 *Pixels);

        /// load from file
        void loadFile(const std::string &FileName);

        /// load from memory
        void loadMemory(const void *Data, std::size_t Size);

        /// save to file
        void save(const std::string &FileName);

        void makeColorMask(const KColor& Color, U8 Alpha = 0);
        void flipH();
        void flipV();

        void setPixel(KVector2U32 Position, const KColor &Color);
        KColor getPixel(KVector2U32 Position) const;
        inline KVector2U32 getSize() const {return _ksize;}
		inline const U8 *getPixelsData() const { if (!_kpixels.empty()) return &_kpixels[0]; return 0; }
    private:
        std::vector<U8> _kpixels;
        KVector2U32 _ksize;
    };
}

#endif // KIMAGE_H
