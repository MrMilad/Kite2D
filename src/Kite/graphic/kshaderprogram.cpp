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
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/kgraphicutil.h"
#include "src/Kite/graphic/glcall.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdstring.h"
#include "Kite/serialization/types/kstdpair.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include <luaintf/LuaIntf.h>
#include <cstdio>

namespace Kite{
	U32 KShaderProgram::_klastProgId = 0;

	KShaderProgram::KShaderProgram(const std::string &Name) :
		KResource(Name, false, true),
		_klinked(false),
		_kprogId(0),
		_kvert(nullptr),
		_kfrag(nullptr),
		_kgeom(nullptr)
	{}

	KShaderProgram::~KShaderProgram(){
		// Delete the shader program from the graphics card memory to
		// free all the resources it's been using
		if (_kprogId > 0) {
			DGL_CALL(glDeleteProgram(_kprogId));
		}

		// reset last program id
		if (_klastProgId == _kprogId){
			_klastProgId = 0;
		}
	}

	bool KShaderProgram::inite() {
		if (isInite()) {
			return true;
		}
		// Generate a unique Id / handle for the shader program
		_kprogId = DGL_CALL(glCreateProgram());

		if (!_kprogId) {
			KD_PRINT("cant create gl program id.");
			return false;
		}

		// Attach the shaders to the program
		// vertex
		if (_kvert) {
			_kvert->inite();
			if (_kvert->compile()) {
				DGL_CALL(glAttachShader(_kprogId, _kvert->getGLID()));
			} else {
				KD_PRINT("vertex shader compilation failed.");
				return false;
			}
		} else {
			KD_PRINT("there is no attached vertex shader.");
			return false;
		}

		// fragment
		if (_kfrag) {
			_kfrag->inite();
			if (_kfrag->compile()) {
				DGL_CALL(glAttachShader(_kprogId, _kfrag->getGLID()));
			} else {
				KD_PRINT("fragment shader compilation failed.");
				return false;
			}
		} else {
			KD_PRINT("there is no attached fragment shader.");
			return false;
		}

		// geometry (optional)
		if (_kgeom) {
			_kgeom->inite();
			if (_kgeom->compile()) {
				DGL_CALL(glAttachShader(_kprogId, _kgeom->getGLID()));
			} else {
				KD_PRINT("geometry shader compilation failed.");
				return false;
			}
		}


		// bind attributes
		for (auto it = _kattribList.begin(); it != _kattribList.end(); ++it) {
			DGL_CALL(glBindAttribLocation(_kprogId, (GLuint)it->first, it->second.c_str()));
		}

		setInite(true);
		return true;
	}

	bool KShaderProgram::_loadStream(KIStream &Stream, const std::string &Address) {
		setInite(false);

		if (!Stream.isOpen()) {
			Stream.close();
		}

		if (!Stream.open(Address, IOMode::BIN)) {
			KD_FPRINT("can't open stream. address: %s", Address.c_str());
			return false;
		}

		KBinarySerial bserial;
		if (!bserial.loadStream(Stream, Address)) {
			KD_PRINT("can't load stream");
			Stream.close();
			return false;
		}
		std::string format;

		bserial >> format;

		if (format != "KSHProg") {
			KD_PRINT("incorrect file format.");
			Stream.close();
			return false;
		}

		bserial >> _kattribList;

		// load composite resources (shaders)
		auto slist = getCompositeList();
		for (auto it = slist.begin(); it != slist.end(); ++it) {
			auto shd = (KShader *)(*it);
			if (shd->getShaderType() == ShaderType::VERTEX) {
				_kvert = shd;
			} else if (shd->getShaderType() == ShaderType::FRAGMENT) {
				_kfrag = shd;
			} else if (shd->getShaderType() == ShaderType::GEOMETRY) {
				_kgeom = shd;
			}
		}

		Stream.close();
		return true;
	}

	bool KShaderProgram::_saveStream(KOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << std::string("KSHProg");
		bserial << _kattribList;

		if (!bserial.saveStream(Stream, Address, 0)) {
			KD_PRINT("can't save stream.");
			Stream.close();
			return false;
		}

		Stream.close();

		// save composite list (shaders)
		std::vector<KResource *> composite;
		composite.reserve(3);
		composite.push_back((KResource *)_kvert);
		composite.push_back((KResource *)_kfrag);
		composite.push_back((KResource *)_kgeom);
		setCompositeList(composite);

		return true;
	}

	bool KShaderProgram::setShader(KShader *Shader, ShaderType Type){
		if (Shader != nullptr) {
			if (Shader->getShaderType() != Type) {
				KD_PRINT("shader type missmatch.");
				return false;
			}
		}
		if (Type == ShaderType::VERTEX) {
			_kvert = Shader;
		} else if (Type == ShaderType::FRAGMENT) {
			_kfrag = Shader;
		} else if (Type == ShaderType::GEOMETRY) {
			_kgeom = Shader;
		} else {
			KD_PRINT("incorrect shader type.");
			return false;
		}

		return true;
	}

	const KShader *KShaderProgram::getShader(ShaderType Type) {
		if (Type == ShaderType::VERTEX) {
			return _kvert;
		} else if (Type == ShaderType::FRAGMENT) {
			return _kfrag;
		} else if (Type == ShaderType::GEOMETRY) {
			return _kgeom;
		}
		return nullptr;
	}

	void KShaderProgram::bindAttribute(U16 Index, const std::string &Name){
		if (isInite()) {
			KD_FPRINT("rebinding attribute is not allowed. rname: %s", getName().c_str());
			return;
		}
		_kattribList.push_back({ Index, Name });
	}

	bool KShaderProgram::link(){
		if (!isInite()) {
			KD_FPRINT("shader program not initialized. rname: %s", getName().c_str());
			return false;
		}

		if (_klinked) {
			return true;
		}

		// link the program
		DGL_CALL(glLinkProgram(_kprogId));

		// check the link log
		GLint isLinked;
		DGL_CALL(glGetProgramiv(_kprogId, GL_LINK_STATUS, (int *)&isLinked));
		//DGL_CALL(glGetObjectParameterivARB(_kprogram, GL_OBJECT_LINK_STATUS_ARB, &success)); // old version
		if (isLinked == GL_FALSE){
			KD_PRINT("failed to link shader program.");
			return false;
		}

		_klinked = true;
		return true;
	}

	I16 KShaderProgram::getUniformLocation(const std::string &ParamName) const{
		if (_kprogId){
			GLint location = glGetUniformLocationARB(_kprogId, ParamName.c_str());
			return (I16)location;
		}
		return -1;
	}

	I16 KShaderProgram::getAttributeLocation(const std::string &ParamName) const {
		if (_kprogId) {
			GLint location = glGetUniformLocationARB(_kprogId, ParamName.c_str());
			return (I16)location;
		}
		return -1;
	}

	void KShaderProgram::setParam1(Kite::I16 Location, F32 Value) const{
		if (_kprogId && Location >= 0){
			// get parameter location and assign it new values
			DGL_CALL(glUniform1fARB(Location, Value));
		}
	}

	void KShaderProgram::setParam2(Kite::I16 Location, F32 Value1, F32 Value2) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform2fARB(Location, Value1, Value2));
		}
	}

	void KShaderProgram::setParam3(Kite::I16 Location, F32 Value1, F32 Value2, F32 Value3) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform3fARB(Location, Value1, Value2, Value3));
		}
	}

	void KShaderProgram::setParam4(Kite::I16 Location, F32 Value1, F32 Value2, F32 Value3, F32 Value4) const{
		if (_kprogId && Location >= 0){
			DGL_CALL(glUniform4fARB(Location, Value1, Value2, Value3, Value4));
		}
	}

	void KShaderProgram::setParamColor(Kite::I16 Location, const KColor& Color) const{
		setParam4(Location, Color.getGLR(), Color.getGLG(), Color.getGLB(), Color.getGLA());
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

	KMETA_KSHADERPROGRAM_SOURCE();
}