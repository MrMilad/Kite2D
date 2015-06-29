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
#ifndef KARCHIVE_H
#define KARCHIVE_H

/*! \file karchive.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kinputstream.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include <cstring>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{
	namespace Internal{
		class ZipIO;
	}
	//! The KArchive class allow manipulate the ZIP-format archives.
	/*!
		This class can read files in the archive. stream operations (seek, stream-read) is allowed only for uncompressed files in the archive.
		to read compressed files use only one read() call, along with a buffer large enough to read the entire file.
		use this class as InputStream only for uncompressed data.
	*/
	class KITE_FUNC_EXPORT KArchive : public KInputStream{
	public:

		//! Default constructors
		KArchive();

		//! Destructor
		~KArchive();

		//! Open the archive from a file on disk
		/*!
			Supported formats: ZIP

			\param ArchiveName Address of the archive file on the disk

			\return True if opening was successful
		*/
		bool openArchive(const std::string &ArchiveName);

		//! Open the archive from a memory block
		/*!
			Supported formats: ZIP

			\param Data Pointer to the archive data in memory
			\param Size Size of the data (in bytes)

			\return True if opening was successful
		*/
		bool openArchive(const void *Data, size_t Size);

		//! Get name of the opened archive
		/*!
			\return Name of the opened archive
		*/
		const std::string &getArchiveName() const;

		//! Get number of the files in the opened archive
		/*!
			\return Number of the files
		*/
		U32 getFilesNumber();

		//! Search for a file in the opened archive
		/*!
			\param FileName Name of the file

			\return Index of the file. -1 in case of failure.
		*/
		I32 searchFile(const std::string &FileName);

		//! Get information of the file in the opened archive
		/*!
			\param FileIndex Index of the file
			\param FileInfo Information of the file

			\return True if operation was successful
		*/
		bool getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo);

		//! Open a file in the archive for reading operations
		/*!
			\param FileName Name of the file

			\return True if operation was successful
		*/
		bool openFile(const std::string &FileName);

		//! Get name of the opened file
		/*!
			\return Name of the opened file
		*/
		const std::string &getFileName() const;

		//! Read opened file in the archive
		/*!
			\param Data Pointer to a block of memory with a size of at least (DataSize) bytes, converted to a void*
			\param DataSize Size of each data to be read (in bytes)

			\return The total number of elements successfully read is returned.
		*/
		U64 read(void *Data, U64 DataSize);

		//! Sets the position indicator associated with the stream to a new position
		/*!
			\param Offset Number of bytes to offset from origin
			\param Origin Position used as reference for the offset. see fseek() for more information.

			\return If successful, the function returns zero
		*/
		I32 seek(I64 Offset, I32 Origin);

		//! Returns the current value of the position indicator of the stream.
		/*!
			\return On success, the current value of the position indicator is returned.
					on failure, -1L is returned
		*/
		I64 tell();

		//! Return true if we have an open archive
		/*!
			\return True if we have an open archive
		*/
		bool isArchiveOpen() const;

		//! Return true if we have an open file in the archive
		/*!
			\return True if we have an open file in the archive
		*/
		bool isOpen();

		//! Checks whether the end-of-File indicator associated with stream is set
		/*!
			\return A non-zero value is returned in the case that the end-of-file indicator associated with the stream is set.
					Otherwise, zero is returned.
		*/
		I32 eof();

		//! Return size of stream
		/*!
			\return Size of stream
		*/
		U64 getSize();

		//! Closes the file associated with the stream and disassociates it.
		/*!
			\return Always return 0
		*/
		I32 close();

	private:
		Internal::ZipIO *_kzipIO;
	};
}

#endif // KARCHIVE_H