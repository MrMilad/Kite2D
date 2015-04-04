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
#include "Kite/Assist/graphic/kanimeio.h"
#include <cstdio>

namespace Kite{
	bool KAnimeIO::loadFile(const std::string &FileName, KAnimeObjects &Objects){
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
				if (strcmp(header.format, "kanime\0") == 0){

					// check size of keys objects
					if (header.objCount > 0){

						// temp key
						KAnimeKey temp;

						// read keys one bye one
						for (U32 i = 0; i < header.objCount; i++){
							rsize = fread(&temp, sizeof(KAnimeKey), 1, file);
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

	bool KAnimeIO::saveFile(const std::string &FileName, const KAnimeObjects &Objects){
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
			header.format[2] = 'n';
			header.format[3] = 'i';
			header.format[4] = 'm';
			header.format[5] = 'e';
			header.format[6] = '\0';
			header.objCount = Objects.size();

			// write header
			fwrite(&header, sizeof(KArrayHeader), 1, file);

			// write keys array 
			for (U32 i = 0; i < Objects.size(); i++){
				fwrite(&Objects.at(i), sizeof(KAnimeKey), 1, file);
			}

			ret = true;
		}

		// cleanup
		fclose(file);
		return ret;
	}

	/*void KAnimeIO::deleteObject(std::vector<KAnimeKey *> &Objects){
		for (U32 i = 0; i < Objects.size(); i++){
			delete Objects[i];
			Objects[i] = 0;
		}
		Objects.clear();
	}*/
}