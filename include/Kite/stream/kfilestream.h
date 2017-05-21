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
#ifndef KFILESTREAM_H
#define KFILESTREAM_H

#include "Kite/core/kcoredef.h"
#include "Kite/stream/kiostream.h"
#include "Kite/meta/kmetadef.h"
#include <cstring>
#include "kfilestream.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(IOSTREAM)
	class KITE_FUNC_EXPORT KFileStream : public KIOStream{
		KM_INFO(KI_NAME = "FileStream");
		KFILESTREAM_BODY();
	public:
		KM_CON()
		KFileStream();

		~KFileStream();

		bool openRead(const std::string &Address, IOMode IOType) override;

		bool openWrite(const std::string &Address, IOMode IOType, WriteMode WriteType) override;

		SIZE read(void *Data, SIZE DataSize) override;

		SIZE write(const void *Data, SIZE DataSize) override;

		I32 seek(U64 Offset, I32 Origin) override;

		I64 tell() const override;

		I32 eof() const override;

		// return binary size (if file opened with BIN mode)
		U64 getSize() const override;

		I32 close() override;

		inline const KFileInfo &getFileInfo() const override { return _kfinfo; }

		inline IOMode getIOMode() const override { return _kio; }

		inline bool isOpen() const override { return _kisOpen; }

	private:
		void _getFileInfoStr(const std::string &Address, KFileInfo &FInfo) const;
		KFileInfo _kfinfo;
		IOMode _kio;
		FILE *_kfile;
		bool _kisOpen;
	};
}

#endif // KFILESTREAM_H