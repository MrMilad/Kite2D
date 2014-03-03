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
#ifndef KATLAS_H
#define KATLAS_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include <string>
#include <vector>

namespace Kite{
    class KITE_FUNC_IMPORT KAtlas{
    public:
        KAtlas();
        ~KAtlas();

        /// load atlas file
        bool loadFile(const std::string &FileName);
        //bool loadMemory(const void *Data);

        /// save atlas file
        void saveFile(const std::string &FileName, const std::vector<KAtlasObject> &Objects);

        /// get number of loaded objects
        inline U32 getSize() const {return _ksize;}

        /// get a copy of object(s) by ID
        KAtlasObject getObject(U32 ID) const;

    private:
        KAtlasObject *_kobjects;
        U32 _ksize;
    };
}

#endif // KATLAS_H
