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
#include "Kite/graphic/kshader.h"
#include "src/Kite/graphic/glcall.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf/LuaIntf.h>

namespace Kite{

	KShader::KShader(const std::string &Name) :
		KResource(Name, false),
		_kshtype(ShaderType::UNKNOWN),
		_kglid(0)
	{}

    KShader::~KShader(){
		if (_kglid > 0) {
			DGL_CALL(glDeleteShader(_kglid));
		}
    }

	bool KShader::inite() {
		if (_kcode.empty()) {
			KD_PRINT("empty shader.");
			return false;
		}

		if (isInite()) {
			return true;
		}

		// create shader
		if (_kshtype == ShaderType::VERTEX) {
			_kglid = DGL_CALL(glCreateShader(GL_VERTEX_SHADER));

			// fragment
		} else if (_kshtype == ShaderType::FRAGMENT) {
			_kglid = DGL_CALL(glCreateShader(GL_FRAGMENT_SHADER));

			// geometry
		} else if (_kshtype == ShaderType::GEOMETRY) {
			_kglid = DGL_CALL(glCreateShader(GL_GEOMETRY_SHADER));

			// incorrect format
		} else {
			KD_PRINT("incorrect type.");
			return false;
		}

		// associate the source with the shader id
		const GLchar *gcode = _kcode.c_str();
		DGL_CALL(glShaderSource(_kglid, 1, &gcode, NULL));

		setInite(true);
		return true;
	}

	bool KShader::_loadStream(KIStream *Stream, const std::string &Address, U32 Flag) {
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

		setInite(false);
		_kcode.clear();
		_kglid = 0;
		_kshtype = ShaderType::UNKNOWN;

		SIZE pos;
		std::string type;

		// extract format from address
		if ((pos = Address.find_last_of('.')) == std::string::npos) {
			KD_FPRINT("cant retrieve file format. faddress: %s", Address.c_str());
			return false;
		}
		type = Address.substr(pos);

		// create gl shader
		// vertex
		if (type == ".vert") {
			_kshtype = ShaderType::VERTEX;

			// fragment
		} else if (type == ".frag") {
			_kshtype = ShaderType::FRAGMENT;

			// geometry
		} else if (type == ".geom") {
			_kshtype = ShaderType::GEOMETRY;

			// incorrect format
		} else {
			KD_FPRINT("incorrect file format. faddress: %s", Address.c_str());
			return false;
		}

		// check stream
		if (!Stream->isOpen()) {
			Stream->close();
		}

		// open stream in text mode
		if (!Stream->open(Address, IOMode::TEXT)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		// create buffer
		SIZE rsize = 0;
		char *buffer = (char*)malloc(sizeof(char) * KTEXT_BUFF_SIZE);
		_kcode.reserve(Stream->getSize());

		// reading content
		while (!Stream->eof()) {
			rsize = Stream->read(buffer, sizeof(char) * KTEXT_BUFF_SIZE);
			buffer[rsize] = 0;
			_kcode.append(buffer);
		}

		// cleanup buffer and stream
		free(buffer);
		Stream->close();
		return true;
    }

	bool KShader::_saveStream(KOStream *Stream, const std::string &Address, U32 Flag) {
		if (!Stream->isOpen()) {
			Stream->close();
		}

		if (!Stream->open(Address, IOMode::TEXT)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		if (Stream->write(_kcode.data(), _kcode.size()) != _kcode.size()) {
			KD_FPRINT("can't write data to stream. address: %s", Address.c_str());
			Stream->close();
			return false;
		}

		Stream->close();
		return true;
	}

	bool KShader::loadString(const std::string &Code, ShaderType Type) {
		setInite(false);
		_kcode = Code;
		_kshtype = ShaderType::UNKNOWN;

		if (Code.empty()) {
			return false;
		}

		// create gl shader
		// vertex
		if (Type == ShaderType::VERTEX) {
			_kshtype = ShaderType::VERTEX;

			// fragment
		} else if (Type == ShaderType::FRAGMENT) {
			_kshtype = ShaderType::FRAGMENT;

			// geometry
		} else if (Type == ShaderType::GEOMETRY) {
			_kshtype = ShaderType::GEOMETRY;

			// incorrect format
		} else {
			KD_PRINT("incorrect type.");
			return false;
		}

		return true;
	}

	bool KShader::compile(){
		if (_kcode.empty()) {
			KD_PRINT("empty shader.");
			return false;
		}

		if (!isInite()) {
			KD_PRINT("shader not initialized.");
			return false;
		}

		// compile the shader
		DGL_CALL(glCompileShader(_kglid));

		// check the compilation status and report any errors
		GLint shaderStatus;
		DGL_CALL(glGetShaderiv(_kglid, GL_COMPILE_STATUS, &shaderStatus));

		// if the shader failed to compile, display the info log and quit out
		if (shaderStatus == GL_FALSE){
			GLint infoLogLength;
			DGL_CALL(glGetShaderiv(_kglid, GL_INFO_LOG_LENGTH, &infoLogLength));

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			DGL_CALL(glGetShaderInfoLog(_kglid, infoLogLength, NULL, strInfoLog));

			KD_PRINT("shader compilation failed: ");
			KD_PRINT(strInfoLog);
			delete[] strInfoLog;
			return false;
		}

		return true;
	}

    const std::string KShader::getShaderVersion(){
        std::string ver;
        ver.append((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        return ver;
    }

	KMETA_KSHADER_SOURCE();
}
