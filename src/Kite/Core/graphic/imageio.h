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

/*! \file imageio.h */

#include "Kite/core/system/knoncopyable.h"
#include "Kite/Core/math/kmathstructs.h"
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

		//static void readFromStream();


		//! Encode and write pixels from a pixels storage to the file.
		/*!
			\param FileName Address of image file on disk.
			\param Pixels Pixels storage.
			\param Size Size of image.

			\return True if saving was successful
		*/
		static bool writeToFile(const std::string &FileName, const std::vector<U8> &Pixels,
                         const KVector2U32& Size);

		//! Convert a string to lower case. this is a utility function.
		/*!
			\param String Input string.
		*/
        static void toLower(std::string &String);
    };
}
}

#endif // IMAGEIO_H
