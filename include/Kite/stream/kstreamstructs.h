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
#ifndef KSTREAMSTRUCTS_H
#define KSTREAMSTRUCTS_H

#include "Kite/meta/kmetadef.h"
#include "kstreamstructs.khgen.h"

KMETA
namespace Kite {
	KM_CLASS(POD)
	struct KFileInfo {
		KFILEINFO_BODY();

		KM_VAR() std::string name;
		KM_VAR() std::string path;
		KM_VAR() std::string fullPath;
	};

	KM_CLASS(POD)
	struct KArchiveFileInfo {
		KARCHIVEFILEINFO_BODY();

		KM_VAR() U32 index;
		KM_VAR() U64 compressedSize;
		KM_VAR() U64 uncompressedSize;
		KM_VAR() char name[260];
		KM_VAR() char comment[256];
		KM_VAR() bool isCompressed;
	};
}
#endif // KSTREAMSTRUCTS_H