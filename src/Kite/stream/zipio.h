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
#ifndef ZIPIO_H
#define ZIPIO_H

#include "Kite/core/kcoredef.h"
#include "Kite/stream/kstreamtypes.h"
#include "Kite/stream/kstreamstructs.h"
#include "extlibs/headers/zip/miniz.h"
#include <cstring>

#define KINFLAT_BUFF_SIZE 32768
namespace Kite{
namespace Internal{
	// stream operations (read, seek) only allowed for uncompressed files in the archive.
	class ZipIO{
	public:
		ZipIO();
		~ZipIO();

		// open from file
		bool openArchive(const std::string &Address, OpenMode Mode);

		// open from memory
		// only for reading 
		// write not implemented yet
		bool openArchive(const void *Memory, size_t Size);

		inline const std::string &getArchiveName() const { return _kaname; }

		// get total number of the files in the archive
		// effective when archive opened in read mode
		U32 getFilesNumber();

		// return -1 if file not found
		// else return the index of the file
		// effective when archive opened in read mode
		I32 searchFile(const std::string &FileName);

		// get information of the file in the archive
		// effective when archive opened in read mode
		bool getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo);

		// open archived file by name
		bool openFile(const std::string &FileName);

		// open archived file by index
		// effective when archive opened in read mode
		bool openFile(U32 FileIndex);

		inline const std::string &getFileName() const { return _kfname; }
		inline OpenMode getArchiveOpenMode() const { return _kmode; }

		// read a portion of the file (stream - allowed only for uncompressed files in the archive)
		// read entire file with a single call (allowed for both compressed/uncompressed files in the archive)
		// note: to read a compressed file, the DataSize must be equal to or greater than the uncompressed size
		// effective when archive opened in read mode
		U64 readFile(void *Data, U64 DataSize);

		// write entire data to a single file
		bool writeFile(const void *Data, U64 DataSize, bool Compress);

		// use foreward slash "/" after directory name
		// create sub-directories one bye one (one call for each sub-directory)
		// effective when archive opened in write mode
		bool addDirectory(const std::string &Name);

		// set read offset (stream - allowed only for uncompressed files in the archive)
		// effective when archive opened in read mode
		I32 setReadOffset(I64 Offset, I32 Origin);

		// get read offset (stream - allowed only for uncompressed files in the archive)
		// effective when archive opened in read mode
		I64 getReadOffset() const;

		bool isArchiveOpen() const;

		bool isFileOpen() const;

		I32 eof() const;

		// effective when archive opened in read mode
		U64 getStreamSize() const;

		void closeArchive();

	private:
		// stream not allowed
		U64 _readCompressed(void *Data, U64 DataSize);
		// stream allowed
		U64 _readUncompressed(void *Data, U64 DataSize);
		mz_zip_archive _kzarchive;
		mz_zip_archive_file_stat _kfstat;
		U64 _kcurOfst;
		U64 _kfileCurOfst;
		I32 _kfindex;
		bool _kisopen;
		bool _kready;
		bool _kisCmprsd;
		std::string _kaname;
		std::string _kfname;
		OpenMode _kmode;
	};
}
}

#endif // ZIPIO_H