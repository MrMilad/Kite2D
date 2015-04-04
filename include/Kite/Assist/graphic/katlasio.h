/*
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KATLASIO_H
#define KATLASIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <string>
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KAtlasIO{
    public:

        /// load atlas file
        static bool loadFile(const std::string &FileName, KAtlasObjects &Objects);
        //bool loadMemory(const void *Data);

        /// save atlas file
		static bool saveFile(const std::string &FileName, const KAtlasObjects &Objects);
    };
}

#endif // KATLASIO_H
