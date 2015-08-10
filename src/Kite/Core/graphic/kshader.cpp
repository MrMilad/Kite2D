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
#include "Kite/core/graphic/kshader.h"
#include "src/Kite/core/graphic/glcall.h"
#include <fstream>
#include <vector>

namespace Kite{

	KShader::KShader(KShaderTypes Types):
		KCoreInstance(KCI_SHADER)
	{
		_ktype = Types;
		switch (Types){
		case Kite::KS_VERTEX:
			_kglid = DGL_CALL(glCreateShader(GL_VERTEX_SHADER));
			break;
		case Kite::KS_FRAGMENT:
			_kglid = DGL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
			break;
		case Kite::KS_GEOMETRY:
			_kglid = DGL_CALL(glCreateShader(GL_GEOMETRY_SHADER));
			break;
		default:
			_kglid = 0;
			KDEBUG_PRINT("Invalid shader type");
			break;
		}
	}

    KShader::~KShader(){
		DGL_CALL(glDeleteShader(_kglid));
    }

	bool KShader::loadFile(const std::string &ShaderFile, U32 FileType) {
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

		if (ShaderFile.empty()){
			return false;
        }

		// open file in binary mode
		std::ifstream reader(ShaderFile, std::ios_base::binary);
		KDEBUG_ASSERT_T(reader);

		// calculate size of file
		reader.seekg(0, std::ios_base::end);
		std::streamsize size = reader.tellg();
		if (size <= 0){
			KDEBUG_PRINT("empty file");
			return false;
		}

		// reading file
		std::vector<char> shCod;
		reader.seekg(0, std::ios_base::beg);
		shCod.resize((U32)size);
		reader.read(&shCod[0], size);
		reader.close();

		// append '\0' at the end of cod
		shCod.push_back('\0');

		// get the source as a pointer to an array of characters
		const GLchar *cod = &shCod[0];

		// associate the source with the shader id
		DGL_CALL(glShaderSource(_kglid, 1, &cod, NULL));
		return true;
    }

	bool KShader::loadMemory(const void *Data, std::size_t DataSize, U32 FileType) {
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

		if (!Data){
			return false;
        }

		// get the source as a pointer to an array of characters
		const char *cod = (char *)Data;

		// associate the source with the shader id
		DGL_CALL(glShaderSource(_kglid, 1, &cod, NULL));
        return true;
    }

	bool KShader::loadStream(KInputStream &ShaderStream, U32 FileType) {
		std::vector<char> data;
		// calculate size of stream
		ShaderStream.seek(0, std::ios_base::end);
		std::streamsize size = ShaderStream.tell();
		if (size <= 0){
			KDEBUG_PRINT("empty stream.");
			return false;
		}

		// reading file
		ShaderStream.seek(0, std::ios_base::beg);
		data.resize((U32)size);
		ShaderStream.read(&data[0], size);

		// append '\0' at the end of cod
		data.push_back('\0');

		// get the source as a pointer to an array of characters
		const GLchar *cod = &data[0];

		// associate the source with the shader id
		DGL_CALL(glShaderSource(_kglid, 1, &cod, NULL));
		return true;
	}

	bool KShader::compile(){
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

			KDEBUG_PRINT("shader compilation failed: ");
			KDEBUG_PRINT(strInfoLog);
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

	U64 KShader::getInstanceSize() const{
		return sizeof(KShader);
	}
}
