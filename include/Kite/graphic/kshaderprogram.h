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
#ifndef KSHADERPROGRAM_H
#define KSHADERPROGRAM_H

/*! \file kshaderprogram.h */

#include "Kite/core/kcoredef.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/ecs/kresource.h"
#include "Kite/graphic/kshader.h"
#include <string>
#include "kshaderprogram.khgen.h"

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{
	//! The KShaderProgram class allows OpenGL shader programs to be linked and used.
	/*!
	This class supports shader programs written in the OpenGL Shading Language (GLSL).
	*/
	KM_CLASS(RESOURCE)
	class KITE_FUNC_EXPORT KShaderProgram : public KResource{
		KM_INFO(KI_NAME = "ShaderProgram");
		KSHADERPROGRAM_BODY();
	public:
		//! Default constructors
		/*!
		Geometry is optional and can be nullptr
		*/
		KShaderProgram(const KSharedResource &Vertex, const KSharedResource &Fragment, const KSharedResource &Geometry);

		KShaderProgram() = delete;
		KShaderProgram(const KShaderProgram &Copy) = delete;
		KShaderProgram &operator=(const KShaderProgram &Right) = delete;

		~KShaderProgram();

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

#ifdef KITE_EDITOR
		/// pass nullptr for removing current shader
		bool setShader(const KSharedResource &Shader);

		const KSharedResource &getShader(ShaderType Type);
#endif 

		//! Associate a generic vertex attribute index with a named attribute variable
		/*!
		Bind any attribute befor link()

		\param Index The index of the generic vertex attribute to be bound.
		\param Name a null terminated string containing the name of the vertex shader attribute variable to which index is to be bound
		*/
		KM_FUN()
		void bindAttribute(U16 Index, const std::string &Name);

		//! Get location of the uniform in the shader
		/*!
		\return return -1 if failed
		*/
		KM_FUN()
		I16 getUniformLocation(const std::string &ParamName) const;

		//! Get location of the attribute in the shader
		/*!
		\return return -1 if failed
		*/
		KM_FUN()
		I16 getAttributeLocation(const std::string &ParamName) const;

		//! Set a float parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value Value to assign
		*/
		KM_FUN()
		void setParam1(I16 Location, F32 Value) const;

		//! Set a 2x1 vector (vec2 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value2 Second component of the value to assign
		*/
		KM_FUN()
		void setParam2(I16 Location, F32 Value1, F32 Value2) const;

		//! Set a 3x1 vector (vec3 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value1 Second component of the value to assign
		\param Value3 Third component of the value to assign
		*/
		KM_FUN()
		void setParam3(I16 Location, F32 Value1, F32 Value2, F32 Value3) const;

		//! Set a 4x1 vector (vec4 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value1 Second component of the value to assign
		\param Value3 Third component of the value to assign
		\param Value4 Fourth component of the value to assign
		*/
		KM_FUN()
		void setParam4(I16 Location, F32 Value1, F32 Value2, F32 Value3, F32 Value4) const;

		//! Set a color parameter of the shader
		/*!
		A KColor will be transformed to a normalized vec4(1.0, 0.5, 0.0, 1.0) in the shader

		\param Location Location of parameter in the shader
		\param Color Color to assign
		*/
		KM_FUN()
		void setParamColor(I16 Location, const KColor& Color) const;

		/// pass current texture
		//void setParam(I16 Location, KShaderTextureTypes Texture);

		//! Bind the shader program
		/*!
		Do not forget to link the program before bind it.
		(automatic handle by internal render system)
		*/
		bool bind();

		//! Unbind the shader program if it is currently bound.
		void unbind();

		//! Unbind current shader program
		static void unbindShader();

		//! Get OpenGL ID of the shader program
		/*!
		\return OpenGL ID of shader program
		*/
		KM_PRO_GET(KP_NAME = "glID", KP_TYPE = U32, KP_CM = "opengl id of the shader program")
		inline U32 getGLID() const { return _kprogId; }

	private:
		KShaderProgram(const std::string &Name, const std::string &Address);
		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		bool _create();

		KM_FUN(KP_NAME = "__call")
		static KSharedResource luaConstruct(const KSharedResource &Vertex, const KSharedResource &Fragment);

		bool _kisCreated;
		U32 _kprogId;				//!< ID of shader program
		static U32 _klastProgId;	//!< Last id of shader program 
		std::vector<std::pair<U16, std::string>> _kattribList;		//!< shader attribute list
		KSharedResource _kvert;		//!< Vertex shader
		KSharedResource _kfrag;		//!< Fragment shader
		KSharedResource _kgeom;		//!< Geometry shader
	};
}

#endif // KSHADERPROGRAM_H