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
#include "Kite/Assist/graphic/katlas.h"
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kmeminputstream.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include <fstream>
#include <iostream>

#define JSMN_PARENT_LINKS
#include "extlibs/headers/json/jsmn.h"

namespace Kite {
	bool KAtlas::loadFile(const std::string &FileName, U32 FileType) {
		_kitems.clear();

		// checking file types
		if (FileType == KAF_JSON)
			return _loadJSON(FileName);

		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "rb");

		if (file != NULL) {

			// file header
			KArrayHeader header;
			size_t rsize;

			// set read pointer to begin of file
			fseek(file, 0, SEEK_SET);

			// read header
			rsize = fread(&header, sizeof(KArrayHeader), 1, file);
			if (rsize == 1) {

				// check file format
				if (strcmp(header.format, "katlasi\0") == 0) {

					// check size of array objects
					if (header.objCount > 0) {

						// temp object
						KAtlasItem temp;

						// read objects one bye one
						for (U32 i = 0; i < header.objCount; i++) {
							rsize = fread(&temp, (size_t)header.objSize, 1, file);
							_kitems.push_back(temp);
						}
						ret = true;
					}
				}
			}
		}

		fclose(file);
		return ret;
	}

	bool KAtlas::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		KMemInputStream temp(Data, Size);

		// checking file types
		if (FileType == KAF_JSON)
			return _loadJSON(temp);

		return loadStream(temp);
	}

	bool KAtlas::loadStream(KInputStream &Stream, U32 FileType) {
		// just in case
		_kitems.clear();

		// checking file types
		if (FileType == KAF_JSON)
			return _loadJSON(Stream);

		bool ret = false;

		if (Stream.isOpen()) {

			// file header
			KArrayHeader header;
			U64 rsize = 0;

			// set read pointer to begin of file
			Stream.seek(0, SEEK_SET);

			// read header
			rsize = Stream.read(&header, sizeof(KArrayHeader));
			if (rsize == sizeof(KArrayHeader)) {

				// check file format
				if (strcmp(header.format, "katlasi\0") == 0) {

					// check size of array objects
					if (header.objCount > 0) {

						// temp object
						KAtlasItem temp;

						// read objects one bye one
						for (U32 i = 0; i < header.objCount; i++) {
							Stream.read(&temp, header.objSize);
							_kitems.push_back(temp);
						}
						ret = true;
					}
				}
			}
		}

		return ret;
	}

	bool KAtlas::saveFile(const std::string &FileName) {
		if (_kitems.empty())
			return false;

		bool ret = false;

		// open file
		FILE *file = fopen(FileName.c_str(), "wb");
		if (file != NULL) {

			// inite header
			KArrayHeader header;
			header.format[0] = 'k';
			header.format[1] = 'a';
			header.format[2] = 't';
			header.format[3] = 'l';
			header.format[4] = 'a';
			header.format[5] = 's';
			header.format[6] = 'i';
			header.format[7] = '\0';
			header.objCount = _kitems.size();
			header.objSize = sizeof(KAtlasItem);

			// write header
			fwrite(&header, sizeof(KArrayHeader), 1, file);

			// write array elements 
			for (U32 i = 0; i < _kitems.size(); i++) {
				fwrite(&_kitems[i], sizeof(KAtlasItem), 1, file);
			}

			ret = true;
		}

		// cleanup
		fclose(file);
		return ret;
	}

	bool KAtlas::_loadJSON(const std::string &FileName) {
		// just in case
		_kitems.clear();
		_kimgName.clear();

		// open file
		std::ifstream ifs(FileName.c_str());
		if (ifs.fail()) {
			return false;
			ifs.close();
		}

		// read file content
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));

		return _jsonParser(content);
	}

	bool KAtlas::_loadJSON(KInputStream &Stream) {
		// just in case
		_kitems.clear();
		_kimgName.clear();

		// open file
		if (!Stream.isOpen()) {
			return false;
		}

		// read file content
		std::string content;
		content.resize((U32)Stream.getSize());

		Stream.seek(0, SEEK_SET);

		U64 rsize = 0;
		rsize = Stream.read(&content[0], Stream.getSize());
		if (rsize != Stream.getSize())
			return false;

		return _jsonParser(content);
	}

	bool KAtlas::_jsonParser(std::string &Content) {
		// erase junky data from beginning of file (adobe flash write that junky data at the beginning of json files!)
		Content.erase(0, Content.find("{", 0));

		// we need 32 tokens per atlas item in the file
		// and 20 bytes per token
		// we expect no more than 65536 tokens = 2048 items = 1.3 mb
		const U32 tokenSize = 65536;
		jsmn_parser jpars;
		jsmntok_t *tok = new jsmntok_t[tokenSize];
		I32 ret = 0;

		I32 frames = -2;
		I32 frameObjects = -2;
		I32 object = -2;

		U32 width = 0, height = 0;

		jsmn_init(&jpars);

		// read tokens
		ret = jsmn_parse(&jpars, Content.c_str(), Content.size(), tok, tokenSize);
		if (ret < 0) {
			KDEBUG_PRINT("Failed to parse JSON:");
			delete[] tok;
			return false;
		}

		// find frames
		for (I32 i = 0; i < ret; i++) {

			// frames
			if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "frames", strlen("frames")) == 0) {
				frames = i;
			}

			// frames object
			if (tok[i].type == JSMN_OBJECT && tok[i].parent == frames) {
				frameObjects = i;
			}

			// set object for read
			if (tok[i].parent == frameObjects) {
				object = i + 1;
			}

			// read object
			if (tok[i].parent == object) {
				U16 obj = i + 1;

				// we only need frame field of any object
				if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "frame", strlen("frame")) == 0) {
					KAtlasItem atl;
					for (I32 count = 0; (count < tok[obj].size) && (i < ret); i++) {
						// x
						if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "x", strlen("x")) == 0) {
							atl.blu = (F32)atof(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// y
						if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "y", strlen("y")) == 0) {
							atl.blv = (F32)atof(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// w
						if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "w", strlen("w")) == 0) {
							atl.w = (F32)atof(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// h
						if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "h", strlen("h")) == 0) {
							atl.h = (F32)atof(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}
					}

					_kitems.push_back(atl);
				}
			}

			// meta
			if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "meta", strlen("meta")) == 0) {
				for (i; i < ret; i++) {

					// image size
					if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "size", strlen("size")) == 0) {
						U16 obj = i + 1;

						for (U16 count = 0; (count < tok[obj].size) && (i < ret); i++) {
							// width
							if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "w", strlen("w")) == 0) {
								width = atoi(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
								++count;
							}

							// height
							if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "h", strlen("h")) == 0) {
								height = atoi(Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
								++count;
							}
						}
					}

					// image name
					if (tok[i].type == JSMN_STRING && strncmp(&Content.at(tok[i].start), "image", strlen("image")) == 0) {
						_kimgName = Content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start);
					}
				}
			}
		}

		// calculate gl uv size [0 ... 1]
		for (U32 i = 0; i < _kitems.size(); i++) {
			_kitems[i].id = i;
			_kitems[i].tru = KCINT_TO_F_TEXTURE((_kitems[i].blu + _kitems[i].w), width);
			_kitems[i].trv = KCINT_TO_F_TEXTURE((_kitems[i].blv + _kitems[i].h), height);
			_kitems[i].blu = KCINT_TO_F_TEXTURE(_kitems[i].blu, width);
			_kitems[i].blv = KCINT_TO_F_TEXTURE(_kitems[i].blv, height);
		}

		delete[] tok;
		return true;
	}
}