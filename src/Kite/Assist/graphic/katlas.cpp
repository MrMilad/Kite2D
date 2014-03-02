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
#include "Kite/Assist/graphic/katlas.h"
#include <cstdio>
#include <cstring>

namespace Kite{
    KAtlas::KAtlas():
        _kobjects(0),
        _ksize(0)
    {}
    KAtlas::~KAtlas(){
        if (_kobjects != 0)
            delete[] _kobjects;
    }

    bool KAtlas::loadFile(const std::string &FileName){
        if (_kobjects != 0){
            delete[]  _kobjects;
            _kobjects = NULL;
            _ksize = 0;
        }

        // open file
        FILE *file = fopen(FileName.c_str(), "r");

        if (file != NULL){

            // read header
            KAtlasHeader header;
            size_t rsize;

            // set read pointer to begin of file
            fseek(file, 0, SEEK_SET);
            rsize = fread(&header, sizeof(KAtlasHeader), 1, file);

            if (rsize == 1){

                // check file format
                if (strcmp(header.format, "katlas\0") == 0){


                    // check size of atlas objects
                    if (header.objCount > 0){

                        // allocate enough size
                        _kobjects = new KAtlasObject[header.objCount];

                        // read atlas objects
                        rsize = fread(_kobjects, sizeof(KAtlasObject), header.objCount, file);
                            if (rsize == header.objCount){
                                _ksize = header.objCount;
                                fclose(file);
                                return true;
                            }
                    }
                }
            }
        }

        fclose(file);
        return false;
    }

//    bool KAtlas::loadMemory(const void *Data){

//    }

    void KAtlas::saveFile(const std::string &FileName, const std::vector<KAtlasObject> &Objects){
        if (Objects.empty())
            return;

        FILE *file = fopen(FileName.c_str(), "w");

        KAtlasHeader header;
        header.format[0] = 'k';
        header.format[1] = 'a';
        header.format[2] = 't';
        header.format[3] = 'l';
        header.format[4] = 'a';
        header.format[5] = 's';
        header.format[6] = '\0';

        header.objCount = Objects.size();

        fwrite(&header, sizeof(KAtlasHeader), 1, file);

        fwrite(&Objects[0], sizeof(KAtlasObject), Objects.size(), file);
        fclose(file);
    }

    KAtlasObject KAtlas::getObject(U32 ID) const{
        if (ID < this->getSize()){
            return _kobjects[ID];
        }
        KAtlasObject temp;
        return temp;
    }
}
