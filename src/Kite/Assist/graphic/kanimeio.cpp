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
	bool KAnimeIO::loadFile(const std::string &FileName, std::vector<KAnimeObject *> &Objects){
		Objects.clear();

		KAnimeState *stateArray = 0;
		U16 *jointArray = 0;
		U16 *frameArray = 0;
		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "rb");

		if (file != NULL){
			// set read pointer to begin of file
			fseek(file, 0, SEEK_SET);

			// read header
			KAnimeHeader header;
			size_t rsize;
			rsize = fread(&header, sizeof(KAnimeHeader), 1, file);
			if (rsize == 1){

				// check file format
				if (strcmp(header.format, "kanime\0") == 0){

					// allocate enough size for animations
					Objects.reserve(header.animeCount);

					// read joints array
					jointArray = new U16[header.animeCount];
					rsize = fread(jointArray, sizeof(U16), header.animeCount, file);
					if (rsize == header.animeCount){

						// read frames array
						frameArray = new U16[header.animeCount];
						rsize = fread(frameArray, sizeof(U16), header.animeCount, file);
						if (rsize == header.animeCount){

							// read states array
							for (U32 i = 0; i < header.animeCount; i++){
								KAnimeObject *obj = new KAnimeObject;
								obj->frames = frameArray[i];
								obj->joints = jointArray[i];
								obj->states.resize(obj->frames * obj->joints);
								rsize = fread(&obj->states[0], sizeof(KAnimeState), obj->states.size(), file);
								if (rsize == obj->states.size()){
									Objects.push_back(obj);
									ret = true;
								}
								else{
									ret = false;
									delete obj;
									break;
								}
							}
							
						}
					}

				}
			}
		}

		// cleanup
		fclose(file);
		delete[] jointArray;
		delete[] frameArray;
		delete[] stateArray;

		return ret;
	}

	void KAnimeIO::saveFile(const std::string &FileName, const std::vector<KAnimeObject *> &Objects){
		if (Objects.empty())
			return;

		// open file
		FILE *file = fopen(FileName.c_str(), "wb");
		if (file != NULL){

			// inite header
			KAnimeHeader header;
			// format
			header.format[0] = 'k';
			header.format[1] = 'a';
			header.format[2] = 'n';
			header.format[3] = 'i';
			header.format[4] = 'm';
			header.format[5] = 'e';
			header.format[6] = '\0';
			// count
			header.animeCount = Objects.size();
			// write header to file
			fwrite(&header, sizeof(KAnimeHeader), 1, file);

			// write joint count array
			for (U32 i = 0; i < Objects.size(); i++){
				fwrite(&Objects.at(i)->joints, sizeof(U16), 1, file);
			}

			// write frame count array
			for (U32 i = 0; i < Objects.size(); i++){
				fwrite(&Objects.at(i)->frames, sizeof(U16), 1, file);
			}

			// write all state to file
			for (U32 i = 0; i < Objects.size(); i++){
				U32 stateSize = Objects.at(i)->frames * Objects.at(0)->joints;
				fwrite(&Objects.at(i)->states[0], sizeof(KAnimeState), stateSize, file);
			}
			
		}

		// cleanup
		fclose(file);
	}

	void KAnimeIO::deleteObject(std::vector<KAnimeObject *> &Objects){
		for (U32 i = 0; i < Objects.size(); i++){
			delete Objects[i];
			Objects[i] = 0;
		}
		Objects.clear();
	}
}