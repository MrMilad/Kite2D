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
#ifndef KARRAYIO_H
#define KARRAYIO_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kinputstream.h"
#include <vector>
#include <cstring>
#include <cstdio>

namespace Kite{
	class KITE_FUNC_EXPORT KArrayIO{
	public :
		template <typename T>
		static bool loadFile(const std::string &FileName, std::vector<T> &Objects){
			// just in case
			Objects.clear();

			bool ret = false;

			// open file
			FILE *file = fopen(FileName.c_str(), "rb");

			if (file != NULL){

				// file header
				KArrayHeader header;
				size_t rsize;

				// set read pointer to begin of file
				fseek(file, 0, SEEK_SET);

				// read header
				rsize = fread(&header, sizeof(KArrayHeader), 1, file);
				if (rsize == 1){

					// check file format
					if (strcmp(header.format, "karrayf\0") == 0){

						// check size of array objects
						if (header.objCount > 0){

							// temp object
							T temp;

							// read objects one bye one
							for (U32 i = 0; i < header.objCount; i++){
								rsize = fread(&temp, header.objSize, 1, file);
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

		template <typename T>
		static bool loadStream(KInputStream &Stream, std::vector<T> &Objects){
			// just in case
			Objects.clear();

			bool ret = false;

			if (file != NULL){

				// file header
				KArrayHeader header;
				size_t rsize;

				// set read pointer to begin of file
				Stream.seek(0, SEEK_SET);

				// read header
				rsize = Stream.read(&header, sizeof(KArrayHeader));
				if (rsize == 1){

					// check file format
					if (strcmp(header.format, "karrayf\0") == 0){

						// check size of array objects
						if (header.objCount > 0){

							// temp object
							T temp;

							// read objects one bye one
							for (U32 i = 0; i < header.objCount; i++){
								rsize = fread(&temp, header.objSize, 1, file);
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

		template <typename T>
		static bool saveFile(const std::string &FileName, std::vector<T> &Objects){
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
				header.format[2] = 'r';
				header.format[3] = 'r';
				header.format[4] = 'a';
				header.format[5] = 'y';
				header.format[6] = 'f';
				header.format[7] = '\0';
				header.objCount = Objects.size();
				header.objSize = sizeof(T);

				// write header
				fwrite(&header, sizeof(KArrayHeader), 1, file);

				// write array elements 
				for (U32 i = 0; i < Objects.size(); i++){
					fwrite(&Objects.at(i), sizeof(T), 1, file);
				}

				ret = true;
			}

			// cleanup
			fclose(file);
			return ret;
		}
	};
}

#endif // KARRAYIO_H