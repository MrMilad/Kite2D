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
#include "Kite/core/graphic/kshaderprogram.h"
#include "Kite/core/graphic/kshaderprogram.h"
#include "Kite/core/graphic/kgraphicutil.h"
#include "src/Kite/core/graphic/glcall.h"

namespace Kite{
	U32 KShaderProgram::_klastProgId = 0;

	KShaderProgram::KShaderProgram() :
		KCoreInstance(KCI_SHPROGRAM),
		_kshaderCount(0)
	{
		// Generate a unique Id / handle for the shader program
		// Note: We MUST have a valid rendering context before generating
		// the programId or it causes a segfault!
		_kprogId = DGL_CALL(glCreateProgram());
	}

	KShaderProgram::~KShaderProgram(){
		// Delete the shader program from the graphics card memory to
		// free all the resources it's been using
		DGL_CALL(glDeleteProgram(_kprogId));

		// reset last program id
		if (_klastProgId == _kprogId){
			_klastProgId = 0;
		}
	}

	void KShaderProgram::attachShader(const KShader &Shader){
		// Attach the shader to the program
		DGL_CALL(glAttachShader(_kprogId, Shader.getGLID()));

		// Increment the number of shaders we have associated with the program
		++_kshaderCount;
	}

	void KShaderProgram::bindAttribute(U16 Index, const std::string &Name){
		DGL_CALL(glBindAttribLocation(_kprogId, (GLuint)Index, Name.c_str()));
	}

	bool KShaderProgram::link(){
		if (!_kprogId)
			return false;

		if (_kshaderCount < 2){
			KDEBUG_PRINT("Invalid shader count");
			return false;
		}


		// link the program
		DGL_CALL(glLinkProgram(_kprogId));

		// check the link log
		GLint isLinked;
		DGL_CALL(glGetProgramiv(_kprogId, GL_LINK_STATUS, (int *)&isLinked));
		//DGL_CALL(glGetObjectParameterivARB(_kprogram, GL_OBJECT_LINK_STATUS_ARB, &success)); // old version
		if (isLinked == GL_FALSE){
			KDEBUG_PRINT("Failed to link shader program.");
			return false;
		}

		return true;
	}

	I16 KShaderProgram::getUniformLocation(const std::string &ParamName) const{
		if (_kprogId){
			GLint location = glGetUniformLocationARB(_kprogId, ParamName.c_str());
			return (I16)location;
		}
		return -1;
	}

	void KShaderProgram::setParam(Kite::I16 Location, F32 Value) const{
		if (_kprogId && Location >= 0){
			// get parameter location and assign it new values
			DGL_CALL(glUniform1fARB(Location, Value));
		}
	}

	void KShaderProgram::setParam(Kite::I16 Location, F32 Value1, F32 Value2) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform2fARB(Location, Value1, Value2));
		}
	}

	void KShaderProgram::setParam(Kite::I16 Location, F32 Value1, F32 Value2, F32 Value3) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform3fARB(Location, Value1, Value2, Value3));
		}
	}

	void KShaderProgram::setParam(Kite::I16 Location, F32 Value1, F32 Value2, F32 Value3, F32 Value4) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform4fARB(Location, Value1, Value2, Value3, Value4));
		}
	}

	void KShaderProgram::setParam(Kite::I16 Location, const KColor& Color) const{
		setParam(Location, Color.r / 255.f, Color.g / 255.f, Color.b / 255.f, Color.a / 255.f);
	}

	void KShaderProgram::bind() const{
		if (_klastProgId != _kprogId){
			// enable the program
			DGL_CALL(glUseProgram(_kprogId));
			_klastProgId = _kprogId;
		}
	}

	void KShaderProgram::unbind(){
		// disable the program
		if (_klastProgId == _kprogId){
			DGL_CALL(glUseProgram(0));
			_klastProgId = 0;
		}
	}

	void KShaderProgram::unbindShader(){
		// disable the program
		DGL_CALL(glUseProgramObjectARB(0));
		_klastProgId = 0;
	}

	U64 KShaderProgram::getInstanceSize() const{
		return sizeof(KShaderProgram);
	}
}