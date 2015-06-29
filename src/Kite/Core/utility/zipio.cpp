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
#include "Kite/Core/utility/kutilitydef.h"
#include "src/Kite/Core/utility/zipio.h"
#include "src/Kite/Core/utility/tp/miniz.c"

namespace Kite{
namespace Internal{
	ZipIO::ZipIO() :
		_kcurOfst(0),
		_kfileCurOfst(0),
		_kfindex(0),
		_kisopen(false),
		_kready(false),
		_kisCmprsd(false)
	{}

	ZipIO::~ZipIO(){
		if (_kisopen){
			mz_zip_reader_end(&_kzarchive);
		}
	}

	// open from file
	bool ZipIO::openArchive(const std::string &ArchiveName){
		// first close last opened archive
		_kready = false;
		_kaname = "";
		if (_kisopen){
			mz_zip_reader_end(&_kzarchive);
			_kisopen = false;
		}

		// inite object
		memset(&_kzarchive, 0, sizeof(_kzarchive));

		// open archive
		mz_bool status = mz_zip_reader_init_file(&_kzarchive, ArchiveName.c_str(), 0);
		if (!status) 
			return false;

		_kisopen = true;
		_kaname = ArchiveName;
		return true;
	}

	// open from memory
	bool ZipIO::openArchive(const void *Memory, size_t Size){
		// first close last opened archive
		_kready = false;
		_kaname = "";
		if (_kisopen){
			mz_zip_reader_end(&_kzarchive);
			_kisopen = false;
		}

		// inite object
		memset(&_kzarchive, 0, sizeof(_kzarchive));

		// open archive
		mz_bool status = mz_zip_reader_init_mem(&_kzarchive, Memory, Size, 0);
		if (!status) return false;

		_kaname = "memory-block";
		_kisopen = true;
		return true;
	}

	// open archived file
	bool ZipIO::openFile(const std::string &FileName){
		_kfname = "";
		_kready = false;
		if (!_kisopen)
			return false;

		if ((_kfindex = mz_zip_reader_locate_file(&_kzarchive, FileName.c_str(), NULL, 0)) < 0)
			return false;

		return openFile(_kfindex);
	}

	bool ZipIO::openFile(U32 FileIndex){
		_kready = false;
		_kfindex = FileIndex;
		_kcurOfst = 0;
		_kfileCurOfst = 0;
		if (!_kisopen)
			return false;

		if (!mz_zip_reader_file_stat(&_kzarchive, FileIndex, &_kfstat))
			return false;

		// Empty file, or a directory (but not always a directory - I've seen odd zips with directories that have compressed data which inflates to 0 bytes)
		if (!_kfstat.m_comp_size)
			return false;

		// Entry is a subdirectory (I've seen old zips with dir entries which have compressed deflate data which inflates to 0 bytes, but these entries claim to uncompress to 512 bytes in the headers).
		// I'm torn how to handle this case - should it fail instead?
		if (mz_zip_reader_is_file_a_directory(&_kzarchive, _kfindex))
			return false;

		// Encryption and patch files are not supported.
		if (_kfstat.m_bit_flag & (1 | 32))
			return false;

		// This function only supports stored and deflate.
		if ((!(0 & MZ_ZIP_FLAG_COMPRESSED_DATA)) && (_kfstat.m_method != 0) && (_kfstat.m_method != MZ_DEFLATED))
			return false;

		// Read and parse the local directory entry.
		mz_uint32 local_header_u32[(MZ_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(mz_uint32)-1) / sizeof(mz_uint32)]; mz_uint8 *pLocal_header = (mz_uint8 *)local_header_u32;
		_kcurOfst = _kfstat.m_local_header_ofs;
		if (_kzarchive.m_pRead(_kzarchive.m_pIO_opaque, _kcurOfst, pLocal_header, MZ_ZIP_LOCAL_DIR_HEADER_SIZE) != MZ_ZIP_LOCAL_DIR_HEADER_SIZE)
			return false;
		if (MZ_READ_LE32(pLocal_header) != MZ_ZIP_LOCAL_DIR_HEADER_SIG)
			return false;

		_kcurOfst += MZ_ZIP_LOCAL_DIR_HEADER_SIZE + MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_FILENAME_LEN_OFS) + MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_EXTRA_LEN_OFS);
		if ((_kcurOfst + _kfstat.m_comp_size) > _kzarchive.m_archive_size)
			return false;

		if (_kfstat.m_method == MZ_DEFLATED)
			_kisCmprsd = true;
		else if (_kfstat.m_method == 0 && _kfstat.m_uncomp_size == _kfstat.m_comp_size)
			_kisCmprsd = false;

		_kfname.append(_kfstat.m_filename);
		_kready = true;
		return true;
	}

	U32 ZipIO::getFilesNumber(){
		if (!_kisopen)
			return 0;

		return mz_zip_reader_get_num_files(&_kzarchive);
	}

	I32 ZipIO::searchFile(const std::string &FileName){
		if (!_kisopen)
			return -1;

		return mz_zip_reader_locate_file(&_kzarchive, FileName.c_str(), NULL, 0);
	}

	bool ZipIO::getFileInformation(U32 FileIndex, KArchiveFileInfo &FileInfo){
		if (!_kisopen)
			return false;
		
		mz_zip_archive_file_stat stat;
		if (!mz_zip_reader_file_stat(&_kzarchive, FileIndex, &stat))
			return false;

		FileInfo.index = stat.m_file_index;
		strcpy(FileInfo.name, stat.m_filename);
		strcpy(FileInfo.comment, stat.m_comment);
		FileInfo.compressedSize = stat.m_comp_size;
		FileInfo.uncompressedSize = stat.m_uncomp_size;
		if (stat.m_method == MZ_DEFLATED)
			FileInfo.isCompressed = true;
		else if (stat.m_method == 0 && stat.m_uncomp_size == stat.m_comp_size)
			FileInfo.isCompressed = false;

		return true;
	}

	U64 ZipIO::readFile(void *Data, U64 DataSize){
		if (!_kisopen || !_kready || _kfindex < 0)
			return 0;
		
		if (_kisCmprsd)
			return _readCompressed(Data, DataSize);
		else
			return _readUncompressed(Data, DataSize);

		KDEBUG_PRINT("Error in reading file");
		return 0;
	}

	I32 ZipIO::setReadOffset(I64 Offset, I32 Origin){
		if (!_kisopen || !_kready || _kfindex < 0)
			return -1;

		if (_kisCmprsd)
			return -1;

		switch (Origin){
		case SEEK_SET:
			// calibration new offset
			if (Offset > _kfstat.m_uncomp_size)
				Offset = _kfstat.m_uncomp_size;

			// return current offset to base offset
			_kcurOfst -= _kfileCurOfst;

			// set new offset
			_kcurOfst += Offset;
			_kfileCurOfst = Offset;

			return 0;

		case SEEK_CUR:
			// calibration new offset
			if ((Offset + _kfileCurOfst) > _kfstat.m_uncomp_size)
				Offset = _kfstat.m_uncomp_size - _kfileCurOfst;

			// set new offset
			_kcurOfst += Offset;
			_kfileCurOfst = Offset;

			return 0;

		case SEEK_END:
			// calibration new offset
			if (Offset > _kfstat.m_uncomp_size)
				Offset = 0;

			// return current offset to base offset
			_kcurOfst -= _kfileCurOfst;

			// set new offset
			_kcurOfst += _kfstat.m_uncomp_size - Offset;
			_kfileCurOfst = _kfstat.m_uncomp_size - Offset;

			return 0;

		default:
			break;
		}

		KDEBUG_PRINT("Invalid param");
		return -1;
	}

	I64 ZipIO::getReadOffset(){
		if (!_kisopen || !_kready || _kfindex < 0)
			return -1;

		if (_kisCmprsd)
			return -1;

		return _kfileCurOfst;
	}

	bool ZipIO::isArchiveOpen() const {
		return _kisopen;
	}

	bool ZipIO::isFileOpen() const{
		return _kready;
	}

	U64 ZipIO::_readCompressed(void *Data, U64 DataSize){
		if (!_kisopen || !_kready || _kfindex < 0)
			return 0;

		if (mz_zip_reader_extract_to_mem(&_kzarchive, _kfindex, Data, DataSize, 0))
			return DataSize;

		KDEBUG_PRINT("Error in reading file");
		return 0;
	}

	U64 ZipIO::_readUncompressed(void *Data, U64 DataSize){
		if (!_kisopen || !_kready || _kfindex < 0)
			return 0;

		if (!Data || DataSize == 0)
			return 0;

		U64 buffSize = MZ_MIN(DataSize, (_kfstat.m_uncomp_size - _kfileCurOfst));
		if (_kzarchive.m_pRead(_kzarchive.m_pIO_opaque, _kcurOfst, Data, (size_t)buffSize) == buffSize){
			_kcurOfst += buffSize;
			_kfileCurOfst += buffSize;
			return buffSize;
		}

		KDEBUG_PRINT("Error in reading file");
		return 0;
	}

	I32 ZipIO::eof() const{
		if (!_kisopen || !_kready || _kfindex < 0)
			return -1;

		if (_kfstat.m_uncomp_size == _kfileCurOfst)
			return 1;

		return 0;
	}

	U64 ZipIO::getStreamSize() const{
		if (!_kisopen || !_kready || _kfindex < 0)
			return 0;

		return _kfstat.m_uncomp_size;
	}

	void ZipIO::closeArchive(){
		if (_kisopen)
			mz_zip_reader_end(&_kzarchive);

		_kisopen = false;
		_kready = false;
	}

}
}