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
#include "Kite/Core/utility/kutilitystructs.h"
#include "Kite/Core/utility/kmeminputstream.h"
#include "Kite/Core/utility/KBytesArray.h"
#include "Kite/Core/math/ktransformable.h"
#include "Kite/Assist/graphic/kanimeclip.h"
#include "extlibs/headers/xml/rapidxml.hpp"
#include <fstream>

using namespace rapidxml;

namespace Kite {
	KAnimeClip::KAnimeClip() :
		KResource(KRT_ANIMCLIP)
	{}

	bool KAnimeClip::loadMemory(const void *Data, std::size_t Size, U32 FileType) {
		KMemInputStream temp(Data, Size);

		if (FileType == KAF_XML)
			return _loadXML(temp);

		return loadStream(temp);
	}

	bool KAnimeClip::loadStream(KIStream &Stream, U32 FileType) {
		// just in case
		_kkeys.clear();

		if (FileType == KAF_XML)
			return _loadXML(Stream);

		KBytesArray serial;
		if (serial.loadStream(Stream)) {

			// check format
			std::string format;
			serial >> format;

			if (format.compare("kanimekey") == 0) {
				// check size
				U32 size = 0;
				serial >> size;

				for (U32 i = 0; i < size; i++) {
					KAnimeKey item;
					serial >> item;
					_kkeys.push_back(item);
				}
				return true;
			} else {
				KDEBUG_PRINT("wrong file format");
			}
		}
		return false;
	}

	bool KAnimeClip::saveStream(KOStream &Stream) {
		if (_kkeys.empty())
			return true;

		KBytesArray serial;
		serial << std::string("kanimekey");
		serial << _kkeys.size();

		for (U32 i = 0; i < _kkeys.size(); i++) {
			serial << _kkeys[i];
		}

		return serial.saveStream(Stream);
	}

	bool KAnimeClip::_loadXML(const std::string &FileName) {
		// open file
		std::ifstream ifs(FileName.c_str());
		if (ifs.fail()) {
			return false;
			ifs.close();
		}

		// read file content
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));

		return _xmlParser(content);
	}

	bool KAnimeClip::_loadXML(KIStream &Stream) {
		// open file
		if (!Stream.isOpen()) {
			return false;
		}

		std::string content;
		content.resize((U32)Stream.getSize());
		U64 rsize = 0;

		// set read pointer to begin of file
		Stream.seek(0, SEEK_SET);

		rsize = Stream.read(&content[0], Stream.getSize());

		if (rsize = Stream.getSize())
			return _xmlParser(content);

		return false;
	}

	bool KAnimeClip::_xmlParser(std::string &Content) {
		// just in case
		_kkeys.clear();

		// parse content
		xml_document<> doc;
		xml_node<> *root;
		xml_attribute<> *attr;
		doc.parse<0>(&Content[0]);
		root = doc.first_node();

		F32  width = 0, height = 0;
		F32 frameRate, fixedRate = 0;
		KVector2F32 pos, skew, scale;
		F32 rot = 0, x = 0, y = 0;

		KTransformable trans;
		const KTransform *tr;

		for (xml_node<> *node = root->first_node(); node; node = node->next_sibling()) {

			// Source
			if (strcmp(node->name(), "source") == 0) {
				xml_node<> *child = node->first_node("Source");

				// source attribute
				attr = child->first_attribute("frameRate");
				if (attr) {
					frameRate = (F32)atof(attr->value());
					fixedRate = 1.0f / frameRate;
				}

				attr = child->first_attribute("x");
				if (attr) {
					pos.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("y");
				if (attr) {
					pos.y = (F32)atof(attr->value());
				}

				attr = child->first_attribute("scaleX");
				if (attr) {
					scale.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("scaleY");
				if (attr) {
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
				if (attr) {
					rot = (F32)atof(attr->value());
				}

				// rectangle attribute
				child = node->first_node()->first_node()->first_node("geom:Rectangle");
				attr = child->first_attribute("left");
				if (attr) {
					_kshape.leftBottom.x = (F32)atof(attr->value());
					_kshape.leftTop.x = (F32)atof(attr->value());
				}

				attr = child->first_attribute("top");
				if (attr) {
					_kshape.leftBottom.y = (F32)atof(attr->value());
					_kshape.rightBottom.y = (F32)atof(attr->value());
				}

				attr = child->first_attribute("width");
				if (attr) {
					width = (F32)atof(attr->value());
					_kshape.rightBottom.x = _kshape.leftBottom.x + width;
					_kshape.rightTop.x = _kshape.leftBottom.x + width;
				}

				attr = child->first_attribute("height");
				if (attr) {
					height = (F32)atof(attr->value());
					_kshape.leftTop.y = _kshape.leftBottom.y + height;
					_kshape.rightTop.y = _kshape.rightBottom.y + height;
				}

				// transformation point (center)
				child = node->first_node()->first_node()->next_sibling()->first_node("geom:Point");
				attr = child->first_attribute("x");
				if (attr) {
					x = (width * (F32)atof(attr->value())) + _kshape.leftBottom.x;
				}

				attr = child->first_attribute("y");
				if (attr) {
					y = (height * (F32)atof(attr->value())) + _kshape.leftBottom.y;
				}

				// calculate source transform
				trans.setCenter(KVector2F32(x, y));
				trans.setSkew(skew);
				trans.setScale(scale);
				trans.setRotation(rot);
				trans.setPosition(pos);
				tr = trans.getTransform();
				_kshape.leftBottom = tr->transformPoint(_kshape.leftBottom);
				_kshape.rightBottom = tr->transformPoint(_kshape.rightBottom);
				_kshape.leftTop = tr->transformPoint(_kshape.leftTop);
				_kshape.rightTop = tr->transformPoint(_kshape.rightTop);
			}

			// keys
			if (strcmp(node->name(), "Keyframe") == 0) {
				KAnimeKey key;

				attr = node->first_attribute("index");
				if (attr) {
					// first key is always empty in xml file
					// we fill it with default key and skip
					if (atoi(attr->value()) == 0) {
						KAnimeKey firstKey;
						firstKey.center = pos;
						firstKey.position = pos;
						_kkeys.push_back(firstKey);
						continue;
					}


					key.time = atoi(attr->value()) * fixedRate;
				}

				attr = node->first_attribute("x");
				if (attr) {
					key.trChannel = true;
					key.translate.x = (F32)atof(attr->value());
				} else {
					// use previous value
					key.translate.x = _kkeys.back().translate.x;
				}

				attr = node->first_attribute("y");
				if (attr) {
					key.trChannel = true;
					key.translate.y = (F32)atof(attr->value());
				} else {
					// use previous value
					key.translate.y = _kkeys.back().translate.y;
				}

				attr = node->first_attribute("scaleX");
				if (attr) {
					key.scaleChannel = true;
					key.scale.x = (F32)atof(attr->value());
				} else {
					// use previous value
					key.scale.x = _kkeys.back().scale.x;
				}

				attr = node->first_attribute("scaleY");
				if (attr) {
					key.scaleChannel = true;
					key.scale.y = (F32)atof(attr->value());
				} else {
					key.scale.y = _kkeys.back().scale.y;
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
				if (attr) {
					key.rotateChannel = true;
					key.rotate = (F32)atof(attr->value());
				} else {
					key.rotate = _kkeys.back().rotate;
				}

				// relative position
				if (_krelative) {
					key.trChannel = true;
					key.position = pos;
				}

				key.center = _kkeys.back().center;
				_kkeys.push_back(key);

			}
		}

		return true;
	}
}