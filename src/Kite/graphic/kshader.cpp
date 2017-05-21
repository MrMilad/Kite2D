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

namespace Kite {

	KShader::KShader(const std::string &Code, ShaderType Type) :
		KResource(),
		_kisCreated(false),
		_kshtype(Type),
		_kcode(Code),
		_kglid(0)
	{}

	KShader::KShader(const std::string &Name, const std::string &Address):
		KResource(Name, Address),
		_kisCreated(false),
		_kglid(0)
	{}

	KSharedResource KShader::luaConstruct(const std::string &Code, ShaderType Type) {
		return KSharedResource(new KShader(Code, Type));
	}

    KShader::~KShader(){
		if (_kisCreated) {
			DGL_CALL(glDeleteShader(_kglid));
		}
    }

	bool KShader::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) {
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

		_kcode.clear();
		_kglid = 0;

		SIZE pos;
		std::string type;

		// extract format from address
		if ((pos = getAddress().find_last_of('.')) == std::string::npos) {
			KD_FPRINT("cant retrieve file format. faddress: %s", getAddress().c_str());
			return false;
		}
		type = getAddress().substr(pos);

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
			KD_FPRINT("incorrect file format. faddress: %s", getAddress().c_str());
			return false;
		}

		// open stream in text mode
		if (!Stream->openRead(getAddress(), IOMode::TEXT)) {
			KD_FPRINT("can't open stream. address: %s", getAddress().c_str());
			return false;
		}

		// create buffer
		SIZE rsize = 0;
		char *buffer = (char*)malloc(sizeof(char) * KTEXT_BUFF_SIZE);
		_kcode.reserve((U32)Stream->getSize());

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

	bool KShader::saveStream(KIOStream &Stream, const std::string &Address) {
		if (!Stream.openWrite(Address, IOMode::TEXT, WriteMode::NEW)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		if (Stream.write(_kcode.data(), _kcode.size()) != _kcode.size()) {
			KD_FPRINT("can't write data to stream. address: %s", Address.c_str());
			Stream.close();
			return false;
		}

		Stream.close();
		return true;
	}

	bool KShader::compile(){
		if (_kcode.empty()) {
			KD_PRINT("empty shader.");
			return false;
		}

		// create shader
		if (!_kisCreated) {
			if (_kshtype == ShaderType::VERTEX) {
				_kglid = DGL_CALL(glCreateShader(GL_VERTEX_SHADER));

			} else if (_kshtype == ShaderType::FRAGMENT) {
				_kglid = DGL_CALL(glCreateShader(GL_FRAGMENT_SHADER));

			} else if (_kshtype == ShaderType::GEOMETRY) {
				_kglid = DGL_CALL(glCreateShader(GL_GEOMETRY_SHADER));
			}

			// check gl id
			if (!_kglid) {
				KD_PRINT("cant create shader program id.");
				return false;
			}

			_kisCreated = true;
		}

		// associate the source with the shader id
		const GLchar *gcode = _kcode.c_str();
		DGL_CALL(glShaderSource(_kglid, 1, &gcode, NULL));

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

    std::string KShader::getShaderVersion(){
        std::string ver;
        ver.append((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        return ver;
    }

	KSHADER_SOURCE();
}
