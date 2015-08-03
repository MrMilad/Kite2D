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
#include "Kite/Assist/flash/kflashio.h"
#include "Kite/Core/math/ktransformable.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include "extlibs/headers/xml/rapidxml.hpp"
#include <fstream>
#include <iostream>

#define JSMN_PARENT_LINKS
#include "extlibs/headers/json/jsmn.h"


using namespace rapidxml;

namespace Kite{
	bool KFlashIO::loadMotionXMLFile(const std::string &FileName, std::vector<KAnimeKey> &Keys, KRect2F32 &Object, bool RelativePosition) {
		// just in case
		Keys.clear();

		// open file
		std::ifstream ifs(FileName.c_str());
		if (ifs.fail()){
			return false;
			ifs.close();
		}

		// read file content
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		// parse content
		xml_document<> doc;
		xml_node<> *root;
		xml_attribute<> *attr;
		doc.parse<0>(&content[0]);
		root = doc.first_node();

		F32  width = 0, height = 0;
		F32 frameRate, fixedRate = 0;
		KVector2F32 pos, skew, scale;
		F32 rot = 0, x = 0, y = 0;

		KTransformable trans;
		const KTransform *tr;

		for (xml_node<> *node = root->first_node(); node; node = node->next_sibling()){

			// Source
			if (strcmp(node->name(), "source") == 0){
				xml_node<> *child = node->first_node("Source");

				// source attribute
				attr = child->first_attribute("frameRate");
				if (attr){
					frameRate = (F32)atof(attr->value());
					fixedRate = 1.0f / frameRate;
				}

				attr = child->first_attribute("x");
				if (attr){
					pos.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("y");
				if (attr){
					pos.y = (F32)atof(attr->value());
				}

				attr = child->first_attribute("scaleX");
				if (attr){ 
					scale.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("scaleY");
				if (attr){
					scale.y = (F32)atof(attr->value());
				}

				//// skew not supported
				/*attr = child->first_attribute("skewX");
				if (attr){
					// x and y are diffrent in flash and our transform system
					skew.y = -atof(attr->value());
				}

				attr = child->first_attribute("skewY");
				if (attr){
					// x and y are diffrent in flash and our transform system
					skew.x = -atof(attr->value());
				}*/

				attr = child->first_attribute("rotation");
				if (attr){ 
					rot = (F32)atof(attr->value());
				}

				// rectangle attribute
				child = node->first_node()->first_node()->first_node("geom:Rectangle");
				attr = child->first_attribute("left");
				if (attr){ 
					Object.leftBottom.x = (F32)atof(attr->value());
					Object.leftTop.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("top");
				if (attr){
					Object.leftBottom.y = (F32)atof(attr->value());
					Object.rightBottom.y = (F32)atof(attr->value());
				}

				attr = child->first_attribute("width");
				if (attr){
					width = (F32)atof(attr->value());
					Object.rightBottom.x = Object.leftBottom.x + width;
					Object.rightTop.x = Object.leftBottom.x + width;
				}

				attr = child->first_attribute("height");
				if (attr){
					height = (F32)atof(attr->value());
					Object.leftTop.y = Object.leftBottom.y + height;
					Object.rightTop.y = Object.rightBottom.y + height;
				}

				// transformation point (center)
				child = node->first_node()->first_node()->next_sibling()->first_node("geom:Point");
				attr = child->first_attribute("x");
				if (attr){
					x = (width * (F32)atof(attr->value())) + Object.leftBottom.x;
				}

				attr = child->first_attribute("y");
				if (attr){
					y = (height * (F32)atof(attr->value())) + Object.leftBottom.y;
				}

				// calculate source transform
				trans.setCenter(KVector2F32(x, y));
				trans.setSkew(skew);
				trans.setScale(scale);
				trans.setRotation(rot);
				trans.setPosition(pos);
				tr = trans.getTransform();
				Object.leftBottom = tr->transformPoint(Object.leftBottom);
				Object.rightBottom = tr->transformPoint(Object.rightBottom);
				Object.leftTop = tr->transformPoint(Object.leftTop);
				Object.rightTop = tr->transformPoint(Object.rightTop);
			}

			// keys
			if (strcmp(node->name(), "Keyframe") == 0){
				KAnimeKey key;

				attr = node->first_attribute("index");
				if (attr){
					// first key is always empty in xml file
					// we fill it with default key and skip
					if (atoi(attr->value()) == 0){
						KAnimeKey firstKey;
						firstKey.center = pos;
						firstKey.position = pos;
						Keys.push_back(firstKey);
						continue;
					}


					key.time = atoi(attr->value()) * fixedRate;
				}

				attr = node->first_attribute("x");
				if (attr){
					key.trChannel = true;
					key.translate.x = (F32)atof(attr->value());
				}else{
					// use previous value
					key.translate.x = Keys.back().translate.x;
				}

				attr = node->first_attribute("y");
				if (attr){ 
					key.trChannel = true;
					key.translate.y = (F32)atof(attr->value());
				}else{
					// use previous value
					key.translate.y = Keys.back().translate.y;
				}

				attr = node->first_attribute("scaleX");
				if (attr){ 
					key.scaleChannel = true;
					key.scale.x = (F32)atof(attr->value());
				}else{
					// use previous value
					key.scale.x = Keys.back().scale.x;
				}

				attr = node->first_attribute("scaleY");
				if (attr){ 
					key.scaleChannel = true;
					key.scale.y = (F32)atof(attr->value());
				}else{
					key.scale.y = Keys.back().scale.y;
				}

				//// skew not supported
				/*attr = node->first_attribute("skewX");
				if (attr){
					key.skewChannel = true;
					// x and y are diffrent in flash and our transform system
					key.skew.y = -atof(attr->value());
				}
				else{
					// use previous value
					key.skew.y = Keys.back().skew.y;
				}

				attr = node->first_attribute("skewY");
				if (attr){
					key.skewChannel = true;
					// x and y are diffrent in flash and our transform system
					key.skew.x = -atof(attr->value());
				}
				else{
					// use previous value
					key.skew.x = Keys.back().skew.x;
				}*/

				attr = node->first_attribute("rotation");
				if (attr){
					key.rotateChannel = true;
					key.rotate = (F32)atof(attr->value());
				}else{
					key.rotate = Keys.back().rotate;
				}

				// relative position
				if (RelativePosition){
					key.trChannel = true;
					key.position = pos;
				}

				key.center = Keys.back().center;
				Keys.push_back(key);
				
			}
		}

		ifs.close();
		return false;
	}

	bool KFlashIO::loadAtlasJSONFile(const std::string &FileName, std::vector<KAtlas> &Atlas, std::string &ImageName){
		// just in case
		Atlas.clear();
		ImageName.clear();

		// open file
		std::ifstream ifs(FileName.c_str());
		if (ifs.fail()){
			return false;
			ifs.close();
		}

		// read file content
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		// erase junky data from beginning of file (adobe flash write that junky data at the beginning of json files!)
		content.erase(0, content.find("{", 0));

		// we need 32 tokens per atlas object in the file
		// and 20 bytes per token
		// we expect no more than 65536 tokens = 2048 Objects = 1.3 mb
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
		ret = jsmn_parse(&jpars, content.c_str(), content.size(), tok, tokenSize);
		if (ret < 0) {
			KDEBUG_PRINT("Failed to parse JSON:");
			ifs.close();
			delete[] tok;
			return false;
		}

		// find frames
		for (I32 i = 0; i < ret; i++){

			// frames
			if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "frames", strlen("frames")) == 0){
				frames = i;
			}

			// frames object
			if (tok[i].type == JSMN_OBJECT && tok[i].parent == frames){
				frameObjects = i;
			}

			// set object for read
			if (tok[i].parent == frameObjects){
				object = i + 1;
			}

			// read object
			if (tok[i].parent == object){
				U16 obj = i + 1;

				// we only need frame field of any object
				if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "frame", strlen("frame")) == 0){
					KAtlas atl;
					for (I32 count = 0; (count < tok[obj].size) && (i < ret); i++){
						// x
						if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "x", strlen("x")) == 0){
							atl.blu = (F32)atof(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// y
						if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "y", strlen("y")) == 0){
							atl.blv = (F32)atof(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// w
						if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "w", strlen("w")) == 0){
							atl.w = (F32)atof(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}

						// h
						if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "h", strlen("h")) == 0){
							atl.h = (F32)atof(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
							++count;
						}
					}

					Atlas.push_back(atl);
				}
			}

			// meta
			if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "meta", strlen("meta")) == 0){
				for (i; i < ret; i++){

					// image size
					if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "size", strlen("size")) == 0){
						U16 obj = i + 1;

						for (U16 count = 0; (count < tok[obj].size) && (i < ret); i++){
							// width
							if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "w", strlen("w")) == 0){
								width = atoi(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
								++count;
							}

							// height
							if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "h", strlen("h")) == 0){
								height = atoi(content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start).c_str());
								++count;
							}
						}
					}

					// image name
					if (tok[i].type == JSMN_STRING && strncmp(&content.at(tok[i].start), "image", strlen("image")) == 0){
						ImageName = content.substr(tok[i + 1].start, tok[i + 1].end - tok[i + 1].start);
					}
				}
			}
		}

		// calculate gl uv size [0 ... 1]
		for (U32 i = 0; i < Atlas.size(); i++){
			Atlas[i].id = i;
			Atlas[i].tru = KCINT_TO_F_TEXTURE((Atlas[i].blu + Atlas[i].w), width);
			Atlas[i].trv = KCINT_TO_F_TEXTURE((Atlas[i].blv + Atlas[i].h), height);
			Atlas[i].blu = KCINT_TO_F_TEXTURE(Atlas[i].blu, width);
			Atlas[i].blv = KCINT_TO_F_TEXTURE(Atlas[i].blv, height);
		}

		ifs.close();
		delete[] tok;
		return true;
	}
}