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
#ifndef KATLAS_H
#define KATLAS_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kresource.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <vector>

namespace Kite {
	class KITE_FUNC_EXPORT KAtlas : public KResource {
	public:

		// pass KAF_JSON for adobe flas JSON files
		bool loadFile(const std::string &FileName, U32 FileType = 0);

		// pass KAF_JSON for adobe flas JSON files
		bool loadMemory(const void *Data, std::size_t Size, U32 FileType = 0);

		// pass KAF_JSON for adobe flas JSON files
		bool loadStream(KInputStream &Stream, U32 FileType = 0);

		bool saveFile(const std::string &FileName);

		// const access
		inline const std::vector<KAtlasItem> *getItems() const { return &_kitems; }

		inline std::vector<KAtlasItem> *getVector() { return &_kitems; }

		inline const std::string *getImageName() const { return &_kimgName; }

	private:
		// load generated flash JSON files (atlas) and convert it to KAtlasItem
		// maximum suported item in the file is 256
		// if you need more objects (more than 256),
		// split your items in multiple JSON file with maximum size then merge them in single vector
		// contents of the Atlas array will be deleted in first step
		bool _loadJSON(const std::string &FileName);
		bool _loadJSON(KInputStream &Stream);
		bool _jsonParser(std::string &Content);
		std::vector<KAtlasItem> _kitems;
		std::string _kimgName;
	};
}

#endif // KATLAS_H