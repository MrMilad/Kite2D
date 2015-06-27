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
#include "Kite/Assist/graphic/kflashio.h"
#include "extlibs/headers/xml/rapidxml.hpp"
#include <fstream>
#include <iostream>

using namespace rapidxml;

namespace Kite{
	bool KFlashIO::loadFile(const std::string &FileName, std::vector<KAnimeKey> &Objects) {
		// just in case
		Objects.clear();

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

		I32 left = 0, top = 0, width = 0, height = 0;
		F32 scaleX = 0, scaleY = 0, rotation = 0;
		F32 frameRate, fixedRate = 0;

		for (xml_node<> *node = root->first_node(); node; node = node->next_sibling()){

			// Source
			if (strcmp(node->name(), "source") == 0){
				xml_node<> *child = node->first_node("Source");

				// source attribute
				attr = child->first_attribute("frameRate");
				if (attr){ frameRate = atof(attr->value()); fixedRate = 1.0f / frameRate; }

				attr = child->first_attribute("scaleX");
				if (attr) scaleX = atof(attr->value());

				attr = child->first_attribute("scaleY");
				if (attr) scaleY = atof(attr->value());

				attr = child->first_attribute("rotation");
				if (attr) rotation = atof(attr->value());

				// rectangle attribute
				child = node->first_node()->first_node()->first_node("geom:Rectangle");
				attr = child->first_attribute("left");
				if (attr) left = atoi(attr->value());

				attr = child->first_attribute("top");
				if (attr) top = atoi(attr->value());

				attr = child->first_attribute("width");
				if (attr) width = atoi(attr->value());

				attr = child->first_attribute("height");
				if (attr) height = atoi(attr->value());
			}

			// keys
			if (strcmp(node->name(), "Keyframe") == 0){
				KAnimeKey key;
				attr = node->first_attribute("index");
				if (attr){ key.time = atoi(attr->value()) * fixedRate; }

				attr = node->first_attribute("x");
				if (attr){ key.trChannel = true; key.translate.x = atof(attr->value()); }

				attr = node->first_attribute("y");
				if (attr){ key.trChannel = true; key.translate.y = atof(attr->value()); }

				attr = node->first_attribute("scaleX");
				if (attr){ key.scaleChannel = true; key.scale.x = atof(attr->value()); }

				attr = node->first_attribute("scaleY");
				if (attr){ key.scaleChannel = true; key.scale.y = atof(attr->value()); }

				attr = node->first_attribute("rotation");
				if (attr){ key.rotateChannel = true; key.rotate = atof(attr->value()); }

				Objects.push_back(key);

			}
		}

		ifs.close();
		return false;
	}
}