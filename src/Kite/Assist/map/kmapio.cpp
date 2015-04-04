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

#include "Kite/Assist/map/kmapio.h"
#include <cstdio>

namespace Kite{
	bool KMapIO::loadFile(const std::string &FileName, std::vector<KTileBitmapTypes> &Objects){
		// just in case
		Objects.clear();

		// open file
		FILE *file = fopen(FileName.c_str(), "r");
		bool ret = false;

		if (file != NULL){

			// first check for pbm format
			char pbm[4];

			// set read pointer to begin of file
			fseek(file, 0, SEEK_SET);

			// read first 3 byte "P6\n"
			if (fread(&pbm, 3, 1, file) == 1){

				// then adde a '\0' at the end of its array for strcmp()
				pbm[3] = '\0';
				if (strcmp(pbm, "P6\n\0") == 0){

					U32 width = 0;
					U32 height = 0;
					U32 colorDedpth;
					U8 pixel[3];

					if (fscanf(file, "%u\n%u\n%u\n\0", &width, &height, &colorDedpth) == 3){
						// reserve enough size
						Objects.reserve(width*height);

						// read color value (rgb) and convert it to KTileBitmapType
						for (U32 i = 0; i < width*height; i++){
							if (fread(pixel, sizeof(U8)* 3, 1, file) != 1){
								ret = false;
								break;
							}

							// convert red component to KTileBitmapType
							if (pixel[0] >= 0 && pixel[0] <= 50) Objects.push_back(KTB_CENTER);
							else if (pixel[0] >= 51 && pixel[0] <= 100) Objects.push_back(KTB_BOTTOM_LEFT);
							else if (pixel[0] >= 101 && pixel[0] <= 150) Objects.push_back(KTB_BOTTOM_RIGHT);
							else if (pixel[0] >= 151 && pixel[0] <= 200) Objects.push_back(KTB_TOP_LEFT);
							else if (pixel[0] >= 201 && pixel[0] <= 255) Objects.push_back(KTB_TOP_RIGHT);
						}
						ret = true;
					}

				// check file format (ktm)
				}else{
					KArrayHeader header;

					// set read pointer to begin of file
					fseek(file, 0, SEEK_SET);

					if (fread(&header, sizeof(KArrayHeader), 1, file) == 1){
						if (strcmp(header.format, "ktileb\0") == 0){

							KTileBitmapTypes tile;
							Objects.reserve(header.objCount);
							for (U32 i = 0; i < header.objCount; i++){
								if (fread(&tile, sizeof(U8), 1, file) != 1){
									ret = false;
									break;
								}

								Objects.push_back(tile);
							}
						}
					}
				}
			}
		}

		// close file
		fclose(file);
		return ret;
	}

	void KMapIO::saveFile(const std::string &FileName, const std::vector<KTileBitmapTypes> &Objects){
		if (Objects.empty())
			return;

		FILE *file = fopen(FileName.c_str(), "w");

		KArrayHeader header;
		header.format[0] = 'k';
		header.format[1] = 't';
		header.format[2] = 'i';
		header.format[3] = 'l';
		header.format[4] = 'e';
		header.format[5] = 'b';
		header.format[6] = '\0';

		header.objCount = (U32)Objects.size();

		fwrite(&header, sizeof(KArrayHeader), 1, file);

		fwrite(&Objects[0], sizeof(KTileBitmapTypes), Objects.size(), file);
		fclose(file);
	}
}