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
#ifndef KARCHIVESTREAM_H
#define KARCHIVESTREAM_H

/*! \file karchiveistream.h */

#include "Kite/stream/kiostream.h"
#include "karchivestream.khgen.h"

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
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
	KM_CLASS(IOSTREAM)
	class KITE_FUNC_EXPORT KArchiveStream : public KIOStream{
		KM_INFO(KI_NAME = "ArchiveStream");

		KARCHIVESTREAM_BODY();
	public:

		//! Default constructors
		KM_CON()
			KArchiveStream();

		//! Destructor
		~KArchiveStream();

		//! Open the archive from a file on disk for reading.
		//! splite address of the archive itself and the name of file inside the archive with ">" character.
		//! supported by Lua.
		/*!
			Supported formats: ZIP

			\param Address Address of the archive file on the disk and name of the stored file inside the archive
			\param Type Ignored. its always binary
			\return True if opening was successful
		*/
		bool openRead(const std::string &Address, IOMode Type) override;

		//! Open the archive from a memory block for reading.
		/*!
			Supported formats: ZIP

			\param Data Pointer to the archive data in memory
			\param Size Size of the data (in bytes)
			\param FileName Name of the file inside the archive

			\return True if opening was successful
		*/
		bool openRead(const void *Data, size_t Size, const std::string &FileName);

		// WriteMode::NEW will remove current archive. 
		bool openWrite(const std::string &Address, IOMode IOType, WriteMode WriteType) override;

		// writing compressed
		inline void setCompressWrite(bool Compress) { _kcomp = Compress; }

		KM_PRO_GET(KP_NAME = "isCompressed", KP_TYPE = std::string)
			inline bool getCompressWrite() const { return _kcomp; }

		SIZE write(const void *Data, SIZE DataSize) override;

		//! Read opened file in the archive
		/*!
			\param Data Pointer to a block of memory with a size of at least (DataSize) bytes, converted to a void*
			\param DataSize Size of each data to be read (in bytes)

			\return The total number of elements successfully read is returned.
		*/
		SIZE read(void *Data, SIZE DataSize) override;

		//! Sets the position indicator associated with the stream to a new position.
		//! effective only for reading.
		/*!
			\param Offset Number of bytes to offset from origin
			\param Origin Position used as reference for the offset. see fseek() for more information.

			\return If successful, the function returns zero
		*/
		I32 seek(U64 Offset, I32 Origin) override;

		//! Returns the current value of the position indicator of the stream.
		/*!
			\return On success, the current value of the position indicator is returned.
					on failure, -1L is returned
		*/
		I64 tell() const override;

		//! Return true if we have an open file in the archive
		/*!
			\return True if we have an open file in the archive
		*/
		bool isOpen() const override;

		//! Checks whether the end-of-File indicator associated with stream is set
		//! effective only for reading.
		/*!
			\return A non-zero value is returned in the case that the end-of-file indicator associated with the stream is set.
					Otherwise, zero is returned.
		*/
		I32 eof() const override;

		//! Return size of stream
		/*!
			\return Size of stream
		*/
		U64 getSize() const override;

		//! Closes the file associated with the stream and disassociates it.
		/*!
			\return Always return 0
		*/
		I32 close() override;

		//! Get name of the opened archive
		/*!
		\return Name of the opened archive
		*/
		KM_PRO_GET(KP_NAME = "archiveName", KP_TYPE = std::string)
		const std::string &getArchiveName() const;

		//! Get number of the files in the opened archive
		/*!
		\return Number of the files
		*/
		KM_PRO_GET(KP_NAME = "filesNumber", KP_TYPE = U32)
		U32 getFilesNumber();

		//! Search for a file in the opened archive
		/*!
		\param FileName Name of the file

		\return Index of the file. -1 in case of failure.
		*/
		KM_FUN()
		I32 searchFile(const std::string &FileName);

		//! Get information of the file in the opened archive
		/*!
		\param FileIndex Index of the file
		\param FileInfo Information of the file

		\return True if operation was successful
		*/
		KM_FUN()
		bool getArchiveFileInfo(U32 FileIndex, KArchiveFileInfo &FileInfo);

		//! Get name of the opened file
		/*!
		\return Name of the opened file
		*/
		const std::string &getFileName() const;

		inline const KFileInfo &getFileInfo() const override { return _kinfo; }

		inline IOMode getIOMode() const override { return IOMode::BIN; }

	private:
		KFileInfo _kinfo;
		Internal::ZipIO *_kzipIO;
		bool _kcomp;
	};
}

#endif // KARCHIVESTREAM_H