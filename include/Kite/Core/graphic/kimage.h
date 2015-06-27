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
#ifndef KIMAGE_H
#define KIMAGE_H

/*! \file kimage.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kcoreinstance.h"
#include "Kite/Core/math/kmathstructs.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/system/kinputstream.h"
#include <string>
#include <vector>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KImage class allow loading/saving image files and manipulating image data.
	/*!
		This class can create the image with only size and color of pixels,
		or read and decode PNG, BMP, TGA formats from a file or memory or a input stream and also write that formats into a file.
	*/
    class KITE_FUNC_EXPORT KImage : public KCoreInstance{
    public:
		//! Constructs an empty image object.
        KImage();

		//! Destructor
        ~KImage();

		//! Create the image with the specific size and color
		/*!
			\param Width Width of the image (in pixels)
			\param Height Height of the image (in pixels)
			\param Color Color of the pixels
		*/
        void create(U32 Width, U32 Height, const KColor &Color);

		//! Create the image with the specific size and color
		/*!
			The pixelx array is assumed to contain 32-bits RGBA pixels,
			and have the given width and height (width * height * 4 = size of array).
			If not, this is an undefined behaviour.
			If pixels is null, an empty image is created.

			\param Width Width of the image (in pixels)
			\param Height Height of the image (in pixels)
			\param Pixels Array of pixels to copy to the image
		*/
        void create(U32 Width, U32 Height, const U8 *Pixels);

		//! Load the image from a file on disk
		/*!
			Supported formats: PNG, BMP, TGA.
			if this function fails, the image is left unchanged.

			\param FileName Address of the image file on the disk

			\return True if loading was successful
		*/
        bool loadFile(const std::string &FileName);

		//! Load the image from a memory block
		/*!
			Supported formats: PNG, BMP, TGA.
			if this function fails, the image is left unchanged.

			\param Data Pointer to the file data in memory
			\param Size Size of the data (in bytes)

			\return True if loading was successful
		*/
        bool loadMemory(const void *Data, std::size_t Size);

		//! Read and decode pixels from input stream.
		/*!
			\param Stream Input stream.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if loading was successful
		*/
		bool loadStream(KInputStream &Stream);

		//! Load the image to a file on disk
		/*!
			Supported formats: PNG, BMP, TGA.
			if this function fails, the image is left unchanged.

			\param FileName Address of the file on the disk

			\return True if saving was successful
		*/
        bool save(const std::string &FileName);

		//! Create a transparency mask from a specified color - key
		/*!
			This function sets the alpha value of every pixel matching
			the given color to alpha (0 by default).

			\param Color Color to make transparent
			\param Alpha Alpha value to assign to transparent pixels
		*/
        void makeColorMask(const KColor& Color, U8 Alpha = 0);

		//! Flip the image horizontally (left <-> right)
        void flipH();

		//! Flip the image vertically (top <-> bottom)
        void flipV();

		//! Change the color of a pixel
		/*!
			Using out-of-range values will result in
			an undefined behaviour.

			\param Position Coordinate of pixel
			\param Color New color of the pixel
		*/
        void setPixel(KVector2U32 Position, const KColor &Color);

		//! Get the color of a pixel
		/*!
			Using out-of-range values will result in
			an undefined behaviour.

			\param Position Coordinate of pixel
			\return Color of the color
		*/
        KColor getPixel(KVector2U32 Position) const;

		//! Get the size of the image
		/*!
			\return The size of the image
		*/
        inline KVector2U32 getSize() const {return _ksize;}

		//! Get a read-only (const) pointer to the array of pixels
		/*!
			width * height * 4 = size of the array.
			the returned pointer may become invalid if you
			modify the image, so you should never store it for too long.
			If the image is empty, a null pointer is returned.

			\return Read-only (const) pointer to the array of pixels
		*/
		inline const U8 *getPixelsData() const { if (!_kpixels.empty()) return &_kpixels[0]; return 0; }

		//! Get size of resource in memory
		/*!
			\return Size of resource in bytes
		*/
		U64 getInstanceSize() const;

    private:
        std::vector<U8> _kpixels;	//!< Array of pixels
        KVector2U32 _ksize;			//!< Size of the image
    };
}

#endif // KIMAGE_H
