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
#ifndef KFLASHIO_H
#define KFLASHIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <cstring>
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KFlashIO{
	public:
		// load flash motion xml files and convert it to KAnimeKey with associated object
		// when RelativePosition is true, function use original position of object (in flash editor) for our animation keys
		// else function use center position (0, 0) for animation keys (disable Position value)
		static bool loadMotionXMLFile(const std::string &FileName, std::vector<KAnimeKey> &Keys,
			KRect2F32 &Object, bool RelativePosition);

		static bool loadAtlasJSONFile(const std::string &FileName, std::vector<KAtlas> &Atlas, std::string &ImageName);
	};
}


#endif // KFLASHIO_H