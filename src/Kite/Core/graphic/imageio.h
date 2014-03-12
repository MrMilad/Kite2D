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
#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Kite/core/system/knoncopyable.h"
#include "Kite/core/system/kvector2.h"
#include <string>
#include <vector>

namespace Kite{
namespace Internal{
    class ImageIO : KNonCopyable{
    public:
        static ImageIO *CreateInstance();
        static void DestroyInstance();

        void readFromFile(const std::string &FileName, std::vector<U8> &Pixels, KVector2U32 &Size);
        void readFromMemory(const void *Data, std::size_t DataSize,
                            std::vector<U8> &Pixels, KVector2U32 &Size);

        void writeToFile(const std::string &FileName, const std::vector<U8> &Pixels,
                         const KVector2U32& Size);
    private:
        ImageIO();
        ~ImageIO();

        // convert a string to lower case
        void toLower(std::string &str);
        static ImageIO *_kinstance;
    };
}
}

#endif // IMAGEIO_H
