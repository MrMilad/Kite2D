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
#ifndef KARCHIVEOSTREAM_H
#define KARCHIVEOSTREAM_H

/*! \file karchiveostream.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/ksystemtypes.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kostream.h"
#include <cstring>

/*! \namespace Kite
\brief Public namespace.
*/
namespace Kite {
	namespace Internal {
		class ZipIO;
	}
	class KITE_FUNC_EXPORT KArchiveOStream : public KOStream {
	public:

		//! Default constructors
		KArchiveOStream();

		//! Destructor
		~KArchiveOStream();

		//! Open the archive from a file on disk
		/*!
		Supported formats: ZIP

		\param ArchiveName Address of the archive file on the disk

		\return True if opening was successful
		*/
		bool openArchive(const std::string &ArchiveName, KIOTypes Mode);

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

		//! Open a file in the archive for writing operations
		/*!
		\param FileName Name of the file

		\return True if operation was successful
		*/
		bool openFile(const std::string &FileName);

		// writing compressed
		inline void setCompressWrite(bool Compress) { _kcomp = Compress; }
		inline bool getCompressWrite() const { return _kcomp; }

		bool write(void *Data, U64 DataSize);

		//! Get name of the opened file
		/*!
		\return Name of the opened file
		*/
		const std::string &getFileName() const;

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

		//! Closes the file associated with the stream and disassociates it.
		/*!
		\return Always return 0
		*/
		I32 close();

	private:
		Internal::ZipIO *_kzipIO;
		bool _kcomp;
	};
}

#endif // KARCHIVEOSTREAM_H