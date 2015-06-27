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
#include "Kite/Assist/graphic/katlasio.h"
#include <cstdio>

namespace Kite{
    bool KAtlasIO::loadFile(const std::string &FileName, std::vector<KAtlas> &Objects){
		// just in case
		Objects.clear();

		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "r");

		if (file != NULL){

			// read header
			KArrayHeader header;
			size_t rsize;

			// set read pointer to begin of file
			fseek(file, 0, SEEK_SET);

			// read header
			rsize = fread(&header, sizeof(KArrayHeader), 1, file);
			if (rsize == 1){

				// check file format
				if (strcmp(header.format, "katlas\0") == 0){

					// check size of atlas objects
					if (header.objCount > 0){

						// temp object
						KAtlas temp;

						// read objects one bye one
						for (U32 i = 0; i < header.objCount; i++){
							rsize = fread(&temp, sizeof(KAtlas), 1, file);
							Objects.push_back(temp);
						}
						ret = true;
					}
				}
			}
		}

		fclose(file);
		return ret;
    }

//    bool KAtlas::loadMemory(const void *Data){

//    }

	bool KAtlasIO::saveFile(const std::string &FileName, const std::vector<KAtlas> &Objects){
		if (Objects.empty())
			return false;

		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "wb");
		if (file != NULL){

			// inite header
			KArrayHeader header;
			header.format[0] = 'k';
			header.format[1] = 'a';
			header.format[2] = 't';
			header.format[3] = 'l';
			header.format[4] = 'a';
			header.format[5] = 's';
			header.format[6] = '\0';
			header.objCount = Objects.size();

			// write header
			fwrite(&header, sizeof(KArrayHeader), 1, file);

			// write objects array 
			for (U32 i = 0; i < Objects.size(); i++){
				fwrite(&Objects.at(i), sizeof(KAtlas), 1, file);
			}

			ret = true;
		}

		// cleanup
		fclose(file);
		return ret;
    }
}
