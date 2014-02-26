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
#include "Kite/core/graphic/kimage.h"
#include "Kite/core/graphic/imageio.h"
#include <cstring>

namespace Kite{
    KImage::KImage():
        _ksize(0,0)
    {}

    KImage::~KImage(){}

    void KImage::create(U32 Width, U32 Height, const KColor &Color){
        if (Width && Height){
            // assign the new size
            _ksize.x = Width;
            _ksize.y = Height;

            // resize the pixel buffer
            _kpixels.resize(Width * Height * 4);

            // fill it with the specified color
            U8* ptr = &_kpixels[0];
            U8* end = ptr + _kpixels.size();
            while (ptr < end){
                *ptr++ = Color.r;
                *ptr++ = Color.g;
                *ptr++ = Color.b;
                *ptr++ = Color.a;
            }
        }else{
            // create an empty image
            _ksize.x = 0;
            _ksize.y = 0;
            _kpixels.clear();
        }
    }

    void KImage::create(U32 Width, U32 Height, const U8 *Pixels){
        if (Pixels && Width && Height){
            // assign the new size
            _ksize.x = Width;
            _ksize.y = Height;

            // copy the pixels
            std::size_t size = Width * Height * 4;
            _kpixels.resize(size);
            std::memcpy(&_kpixels[0], Pixels, size); // faster than vector::assign
        }else{
            // create an empty image
            _ksize.x = 0;
            _ksize.y = 0;
            _kpixels.clear();
        }
    }

    void KImage::loadFile(const std::string &FileName){
        return Internal::ImageIO::CreateInstance()->readFromFile(FileName, _kpixels, _ksize);
    }

    void KImage::loadMemory(const void *Data, std::size_t Size){
        return Internal::ImageIO::CreateInstance()->readFromMemory(Data, Size, _kpixels, _ksize);
    }

    void KImage::save(const std::string &FileName){
        return Internal::ImageIO::CreateInstance()->writeToFile(FileName, _kpixels, _ksize);
    }

    void KImage::makeColorMask(const KColor& Color, U8 Alpha){
        // make sure that the image is not empty
        if (!_kpixels.empty()){
            // replace the alpha of the pixels that match the transparent color
            U8* ptr = &_kpixels[0];
            U8* end = ptr + _kpixels.size();
            while (ptr < end){
                if ((ptr[0] == Color.r) && (ptr[1] == Color.g) && (ptr[2] == Color.b) && (ptr[3] == Color.a))
                    ptr[3] = Alpha;
                ptr += 4;
            }
        }
    }

    void KImage::flipH(){
        if (!_kpixels.empty()){
            std::vector<U8> before = _kpixels;
            for (U32 y = 0; y < _ksize.y; ++y){
                const U8* source = &before[y * _ksize.x * 4];
                U8* dest = &_kpixels[(y + 1) * _ksize.x * 4 - 4];
                for (unsigned int x = 0; x < _ksize.x; ++x){
                    dest[0] = source[0];
                    dest[1] = source[1];
                    dest[2] = source[2];
                    dest[3] = source[3];

                    source += 4;
                    dest -= 4;
                }
            }
        }
    }

    void KImage::flipV(){
        if (!_kpixels.empty()){
            std::vector<U8> before = _kpixels;
            const U8* source = &before[_ksize.x * (_ksize.y - 1) * 4];
            U8* dest = &_kpixels[0];
            std::size_t rowSize = _ksize.x * 4;

            for (U32 y = 0; y < _ksize.y; ++y)
            {
                std::memcpy(dest, source, rowSize);
                source -= rowSize;
                dest += rowSize;
            }
        }
    }

    void KImage::setPixel(KVector2U32 Position, const KColor &Color){
        U8* pixel = &_kpixels[(Position.x + Position.y * _ksize.x) * 4];
        *pixel++ = Color.r;
        *pixel++ = Color.g;
        *pixel++ = Color.b;
        *pixel++ = Color.a;
    }

    KColor KImage::getPixel(KVector2U32 Position) const{
        const U8* pixel = &_kpixels[(Position.x + Position.y * _ksize.x) * 4];
        return KColor(pixel[0], pixel[1], pixel[2], pixel[3]);
    }

}
