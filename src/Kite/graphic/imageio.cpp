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
#include "Kite/system/ksystemdef.h"
#include "src/Kite/graphic/imageio.h"
#include "src/Kite/graphic/tp/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "src/Kite/graphic/tp/stb_image_write.h"

namespace Kite{
namespace Internal{
    ImageIO::ImageIO(){}
    ImageIO::~ImageIO(){}

    ImageIO *ImageIO::_kinstance = 0;
    ImageIO *ImageIO::CreateInstance(){
        if (_kinstance == 0)
            _kinstance = new ImageIO();

        return _kinstance;
    }

    void ImageIO::DestroyInstance(){
        delete _kinstance;
        _kinstance = 0;
    }

    bool ImageIO::readFromFile(const std::string &FileName, std::vector<U8> &Pixels, KVector2U32 &Size){
        // we need an empty array
        Pixels.clear();

        // read the image data and get a pointer to the pixels in memory
        I32 width, height, channels;
        U8* pixPtr = stbi_load(FileName.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (pixPtr && width && height){
            // assign the image properties
            Size.x = width;
            Size.y = height;

            // copy the loaded pixels data to our pixel buffer
            Pixels.resize(width * height * 4);
            memcpy(&Pixels[0], pixPtr, Pixels.size());

            // free the loaded pixels (they are now in our own pixel buffer)
            stbi_image_free(pixPtr);

            return true;
        }
        KDEBUG_PRINT("Failed to load image");
        KDEBUG_BREAK;
        return false;
    }

    bool ImageIO::readFromMemory(const void *Data, std::size_t DataSize,
                        std::vector<U8> &Pixels, KVector2U32 &Size){
        // check input parameters
        if (Data && DataSize){
            // we need an empty array
            Pixels.clear();

            // read the image data and get a pointer to the pixels in memory
            I32 width, height, channels;
            const U8* buffer = (const U8*)(Data);
            U8* pixPtr = stbi_load_from_memory(buffer, (I32)(DataSize), &width, &height, &channels, STBI_rgb_alpha);

            if (pixPtr && width && height){
                // assign the image properties
                Size.x = width;
                Size.y = height;

                // Copy the loaded pixels to the pixel buffer
                Pixels.resize(width * height * 4);
                memcpy(&Pixels[0], pixPtr, Pixels.size());

                // Free the loaded pixels (they are now in our own pixel buffer)
                stbi_image_free(pixPtr);

                return true;
            }
            KDEBUG_PRINT("Failed to read image");
            KDEBUG_BREAK;
            return false;
        }
        KDEBUG_PRINT("Failed to read image");
        KDEBUG_BREAK;
        return false;
    }

    bool ImageIO::writeToFile(const std::string &FileName, const std::vector<U8> &Pixels,
                     const KVector2U32 &Size){
        // make sure the image is not empty
        if (!Pixels.empty() && (Size.x > 0) && (Size.y > 0)){
            // Deduce the image type from its extension
            if (FileName.size() > 3){
                // Extract the extension
                std::string extension = FileName.substr(FileName.size() - 3);

                if (toLower(extension) == "bmp"){
                    // BMP format
                    if (stbi_write_bmp(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0]))
                        return true;
                }else if (toLower(extension) == "tga"){
                    // TGA format
                    if (stbi_write_tga(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0]))
                        return true;
                }else if(toLower(extension) == "png"){
                    // PNG format
                    if (stbi_write_png(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0], 0))
                        return true;
                }/*else if (toLower(extension) == "jpg"){
                    // JPG format
                    if (writeJpg(FileName, Pixels, Size.x, Size.y))
                        return true;
                }*/
            }
        }

        KDEBUG_PRINT("Failed to write image");
        KDEBUG_BREAK;
        return false;
    }

    // Convert a string to lower case
    std::string ImageIO::toLower(std::string str){
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }
}
}
