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
#ifndef IMAGEIO_H
#define IMAGEIO_H

/*! \file imageio.h */

#include "Kite/core/knoncopyable.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/Core/kistream.h"
#include <string>
#include <vector>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

/*! \namespace Kite::Internal
	\brief Private namespace.
*/
namespace Internal{

	//! The ImageIO class is a low-level class for reading and writing image file formats.
	/*!
		This class can read and decode PNG, BMP, TGA formats from a file or memory or a input stream and also write that formats into a file.
		this is a low-level io class. use KImage instead.
	*/
    class ImageIO{
    public:
		//! Read and decode pixels from a file.
		/*!
			\param FileName Address of image file on disk.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if loading was successful
		*/
        static bool readFromFile(const std::string &FileName, std::vector<U8> &Pixels, KVector2U32 &Size);

		//! Read and decode pixels from a memory block.
		/*!
			\param Data Pointer to image data.
			\param DataSize Size of data.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if loading was successful
		*/
		static bool readFromMemory(const void *Data, std::size_t DataSize,
                            std::vector<U8> &Pixels, KVector2U32 &Size);

		//! Read and decode pixels from input stream.
		/*!
			\param Stream Input stream.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if loading was successful
		*/
		static bool readFromStream(KIStream *Stream, const std::string &Address, std::vector<U8> &Pixels, KVector2U32 &Size);

		//! Encode and write pixels from a pixels storage to the file.
		/*!
			\param FileName Address of image file on disk.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if saving was successful
		*/
		static bool writeToFile(const std::string &FileName, const std::vector<U8> &Pixels,
                         const KVector2U32 &Size);

		//! Convert a string to lower case. this is a utility function.
		/*!
			\param String Input string.
		*/
        static void toLower(std::string &String);

	private:
		static int _read(void *user, char *data, int size);   // fill 'data' with 'size' bytes.  return number of bytes actually read 
		static void _skip(void *user, unsigned n);            // skip the next 'n' bytes
		static int _eof(void *user);                       // returns nonzero if we are at end of file/data
    };
}
}

#endif // IMAGEIO_H
