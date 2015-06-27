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
#ifndef KMAPIO_H
#define KMAPIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <string>
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KMapIO{
	public:
		// load KTileBitmap array from ppm (8-bit portable bitmap file) or ktb (compiled tile bitmap) files
		// ktb format is 'engine-digestible' format and very optimized than ppm format (in size and load time)
		// color key in ppm file: only red componnet of each pixel
		// center: 0 to 50
		// bottom left: 51 to 100
		// bootom right: 101 to 150
		// top left: blue 151 to 200
		// top right: 201 to 255
		// note: always flip image vertically before save (ppm file)
		// because we use bottom-left coordinate but image stored in top-left coordinate
		static bool loadFile(const std::string &FileName, std::vector<KTileBitmapTypes> &Objects);

		// save objects to ktb file
		static void saveFile(const std::string &FileName, const std::vector<KTileBitmapTypes> &Objects);
	};
}

#endif // KMAPIO_H