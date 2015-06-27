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
#ifndef KSHADERPROGRAM_H
#define KSHADERPROGRAM_H

/*! \file kshaderprogram.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kcoreinstance.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/core/graphic/kshader.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/math/ktransform.h"
#include <map>
#include <vector>
#include <string>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{
	//! The KShaderProgram class allows OpenGL shader programs to be linked and used.
	/*!
	This class supports shader programs written in the OpenGL Shading Language (GLSL).
	*/
	class KShaderProgram : public KCoreInstance{
	public:
		//! Default constructors
		/*!
		This constructor creates an invalid shader program.
		*/
		KShaderProgram();
		~KShaderProgram();

		//! Attach a shader (vertex or fragment) to the shader program
		/*!
		\param Shader The compiled shader object
		*/
		void attachShader(const KShader &Shader);

		//! Associate a generic vertex attribute index with a named attribute variable
		/*!
		Bind any attribute befor link()

		\param Index The index of the generic vertex attribute to be bound.
		\param Name a null terminated string containing the name of the vertex shader attribute variable to which index is to be bound
		*/
		void bindAttribute(U16 Index, const std::string &Name);

		//! Link the shader program
		/*!
		\return True if linking was successful
		*/
		bool link();

		//! Get location of the uniform in the shader
		/*!
		\return return -1 if failed
		*/
		I16 getUniformLocation(const std::string &ParamName) const;

		//! Set a float parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value Value to assign
		*/
		void setParam(I16 Location, F32 Value) const;

		//! Set a 2x1 vector (vec2 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value2 Second component of the value to assign
		*/
		void setParam(I16 Location, F32 Value1, F32 Value2) const;

		//! Set a 3x1 vector (vec3 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value1 Second component of the value to assign
		\param Value3 Third component of the value to assign
		*/
		void setParam(I16 Location, F32 Value1, F32 Value2, F32 Value3) const;

		//! Set a 4x1 vector (vec4 in GLSL) parameter of the shader
		/*!
		\param Location Location of parameter in the shader
		\param Value1 First component of the value to assign
		\param Value1 Second component of the value to assign
		\param Value3 Third component of the value to assign
		\param Value4 Fourth component of the value to assign
		*/
		void setParam(I16 Location, F32 Value1, F32 Value2, F32 Value3, F32 Value4) const;

		//! Set a color parameter of the shader
		/*!
		A KColor will be transformed to a normalized vec4(1.0, 0.5, 0.0, 1.0) in the shader

		\param Location Location of parameter in the shader
		\param Color Color to assign
		*/
		void setParam(I16 Location, const KColor& Color) const;

		/// pass current texture
		//void setParam(I16 Location, KShaderTextureTypes Texture);

		//! Bind the shader program
		/*!
		Do not forget to link the program before bind it.
		(automatic handle by internal render system)
		*/
		void bind() const;

		//! Unbind the shader program if it is currently bound.
		void unbind();

		//! Unbind current shader program
		static void unbindShader();

		//! Get OpenGL ID of the shader program
		/*!
		\return OpenGL ID of shader program
		*/
		inline U32 getGLID() const { return _kprogId; }

		//! Get size of resource in memory
		/*!
		\return Size of resource in bytes
		*/
		U64 getInstanceSize() const;

	private:
		U32 _kshaderCount; 
		U32 _kprogId;	//!< ID of shader program
		static U32 _klastProgId;	//!< Last id of shader program 
	};
}

#endif // KSHADERPROGRAM_H