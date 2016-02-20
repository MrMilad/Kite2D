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
#include "Kite/core/system/ksystemdef.h"
#include "src/Kite/core/graphic/imageio.h"
#include <string>
#include <algorithm>
#include "extlibs/headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "extlibs/headers/stb_image_write.h"

namespace Kite{
namespace Internal{
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
			return false;
        }
        KDEBUG_PRINT("Failed to read image");
		return false;
    }

	bool ImageIO::readFromStream(KIStream &Stream, std::vector<U8> &Pixels, KVector2U32 &Size){
		// we need an empty array
		Pixels.clear();

		// set callback
		static stbi_io_callbacks _kcallb;
		_kcallb.read = _read;
		_kcallb.skip = _skip;
		_kcallb.eof = _eof;

		// read the image data and get a pointer to the pixels in memory
		I32 width, height, channels;
		U8* pixPtr = stbi_load_from_callbacks(&_kcallb, (void *) &Stream, &width, &height, &channels, STBI_rgb_alpha);

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

				toLower(extension);
                if (extension == "bmp"){
                    // BMP format
                    if (stbi_write_bmp(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0]))
                        return true;
                }else if (extension == "tga"){
                    // TGA format
                    if (stbi_write_tga(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0]))
						return true;
                }else if(extension == "png"){
                    // PNG format
                    if (stbi_write_png(FileName.c_str(), Size.x, Size.y, 4, &Pixels[0], 0))
						return true;
                }/*else if (extension == "jpg"){
                    // JPG format
                    if (writeJpg(FileName, Pixels, Size.x, Size.y))
                        return true;
                }*/
            }
        }

        KDEBUG_PRINT("Failed to write image");
		return false;
    }

    // Convert a string to lower case
    void ImageIO::toLower(std::string &str){
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

	int ImageIO::_read(void *user, char *data, int size){
		KIStream *stream = (KIStream *)user;
		return (int)stream->read(data, size);
	}
	void ImageIO::_skip(void *user, unsigned n){
		KIStream *stream = (KIStream *)user;
		stream->seek(stream->tell() + n, SEEK_CUR);
	}

	int ImageIO::_eof(void *user){
		KIStream *stream = (KIStream *)user;
		return stream->eof();
	}
}
}
