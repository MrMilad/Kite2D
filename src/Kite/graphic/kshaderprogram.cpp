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
	KShaderProgram::KShaderProgram(const KSharedResource &Vertex, const KSharedResource &Fragment, const KSharedResource &Geometry) :
		KResource(),
		_kisCreated(false),
		_kprogId(0),
		_kvert(Vertex),
		_kfrag(Fragment),
		_kgeom(Geometry)
	{
		KD_ASSERT(!Vertex.isNull());
		KD_ASSERT(!Fragment.isNull());
	}

	KShaderProgram::KShaderProgram(const std::string &Name, const std::string &Address) :
		KResource(Name, Address),
		_kisCreated(false),
		_kprogId(0)
	{}

	KShaderProgram::~KShaderProgram(){
		// Delete the shader program from the graphics card memory to
		// free all the resources it's been using
		if (_kisCreated) {
			DGL_CALL(glDeleteProgram(_kprogId));
		}

		// reset last program id
		if (_klastProgId == _kprogId){
			_klastProgId = 0;
		}
	}

	bool KShaderProgram::_create() {
		// Generate a unique Id / handle for the shader program
		if (_kprogId == 0) {
			_kprogId = DGL_CALL(glCreateProgram());
		}

		if (!_kprogId) {
			KD_PRINT("cant create gl program id.");
			return false;
		}

		// Attach the shaders to the program
		// vertex
		if (_kvert) {
			auto vert = static_cast<KShader *>(_kvert.get());
			if (vert->compile()) {
				DGL_CALL(glAttachShader(_kprogId, vert->getGLID()));
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
			auto frag = static_cast<KShader *>(_kfrag.get());
			if (frag->compile()) {
				DGL_CALL(glAttachShader(_kprogId, frag->getGLID()));
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
			auto geom = static_cast<KShader *>(_kgeom.get());
			if (geom->compile()) {
				DGL_CALL(glAttachShader(_kprogId, geom->getGLID()));
			} else {
				KD_PRINT("geometry shader compilation failed.");
				return false;
			}
		}


		// bind attributes
		for (auto it = _kattribList.begin(); it != _kattribList.end(); ++it) {
			DGL_CALL(glBindAttribLocation(_kprogId, (GLuint)it->first, it->second.c_str()));
		}

		// link the program
		DGL_CALL(glLinkProgram(_kprogId));

		// check the link log
		GLint isLinked;
		DGL_CALL(glGetProgramiv(_kprogId, GL_LINK_STATUS, (int *)&isLinked));
		//DGL_CALL(glGetObjectParameterivARB(_kprogram, GL_OBJECT_LINK_STATUS_ARB, &success)); // old version
		if (isLinked == GL_FALSE) {
			KD_PRINT("failed to link shader program.");
			return false;
		}

		_kisCreated = true;
		return true;
	}

	bool KShaderProgram::_loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) {
		KBinarySerial bserial;
		if (!bserial.loadStream(*Stream, getAddress())) {
			KD_PRINT("can't load stream");
			return false;
		}

		// checking format
		std::string format;
		bserial >> format;
		if (format != "ksp") {
			KD_PRINT("incorrect file format.");
			return false;
		}

		// reading data
		bserial >> _kattribList;

		// vertex shader
		std::string vert;
		bserial >> vert;
		_kvert = RManager->load(vert);
		if (_kvert.isNull()) {
			KD_FPRINT("cant load composite resource: resource name: %s", vert.c_str());
			return false;
		}

		// fragment shader
		std::string frag;
		bserial >> frag;
		_kfrag = RManager->load(frag);
		if (_kfrag.isNull()) {
			KD_FPRINT("cant load composite resource: resource name: %s", frag.c_str());
			return false;
		}

		bool hasGeom;
		bserial >> hasGeom;
		if (hasGeom) {
			std::string geom;
			bserial >> geom;

			_kgeom = RManager->load(geom);
			if (_kgeom.isNull()) {
				KD_FPRINT("cant load composite resource: resource name: %s", geom.c_str());
				return false;
			}
		}

		return true;
	}

	bool KShaderProgram::saveStream(KIOStream &Stream, const std::string &Address) {
		KBinarySerial bserial;
		bserial << std::string("ksp");
		bserial << _kattribList;
		bserial << _kvert->getResourceName();
		bserial << _kfrag->getResourceName();
		bserial << !_kgeom.isNull();
		if (_kgeom) bserial << _kgeom->getResourceName();

		return bserial.saveStream(Stream, Address, 0);
	}

	bool KShaderProgram::setShader(const KSharedResource &Shader){
		auto shader = static_cast<const KShader *>(Shader.constGet());
		if (shader->getShaderType() == ShaderType::VERTEX) {
			_kvert = Shader;
		} else if (shader->getShaderType() == ShaderType::FRAGMENT) {
			_kfrag = Shader;
		} else if (shader->getShaderType() == ShaderType::GEOMETRY) {
			_kgeom = Shader;
		} else {
			KD_PRINT("incorrect shader type.");
			return false;
		}

		return true;
	}

	const KSharedResource &KShaderProgram::getShader(ShaderType Type) {
		if (Type == ShaderType::VERTEX) {
			return _kvert;
		} else if (Type == ShaderType::FRAGMENT) {
			return _kfrag;
		} else if (Type == ShaderType::GEOMETRY) {
			return _kgeom;
		}
		KD_PRINT("incorrect shader type");
		return KSharedResource();
	}

	void KShaderProgram::bindAttribute(U16 Index, const std::string &Name){
		if (_kisCreated) {
			KD_PRINT("this function is ineffective after binding the shader program");
			return;
		}
		_kattribList.push_back({ Index, Name });
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

	bool KShaderProgram::bind(){
		// create texture only first time
		if (!_kisCreated) {
			if (!_create()) {
				KD_PRINT("can't initialize shader program");
				return false;
			}
		}

		if (_klastProgId != _kprogId){
			// enable the program
			DGL_CALL(glUseProgram(_kprogId));
			_klastProgId = _kprogId;
		}

		return true;
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

	KSHADERPROGRAM_SOURCE();
}