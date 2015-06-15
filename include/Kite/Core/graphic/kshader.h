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
#ifndef KSHADER_H
#define KSHADER_H

/*! \file kshader.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/math/ktransform.h"
#include <map>
#include <vector>
#include <string>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KShader class allows OpenGL shader programs to be linked and used.
	/*!
		This class supports shader programs written in the OpenGL Shading Language (GLSL).
	*/
    class KITE_FUNC_EXPORT KShader : KNonCopyable{
    public:

		//! Default constructors
		/*!
			This constructor creates an invalid shader.
		*/
        KShader();

		//! Destructor
        ~KShader();

		//! Load both the vertex and fragment shaders from files on disk
		/*!
			\param VertexFile Address of the vertex shader file on the disk
			\param FragmentFile Address of the fragment shader file on the disk

			\return True if loading was successful
		*/
        bool loadFile(const std::string &VertexFile, const std::string &FragmentFile);

		//! Load both the vertex and fragment shaders from a string
		/*!
			\param VertexCod Source code of vertex sahder
			\param FragmentCod Source code of fragment sahder

			\return True if loading was successful
		*/
        bool loadMemory(const std::string &VertexCod, const std::string &FragmentCod);

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

        /// pass 3x3 matrix
        //void setParam(I16 Location, const KTransform& Transform) const;

		//! Set a texture parameter of the shader
		/*!
			\param Location Location of parameter in the shader
			\param Texture Texture to assign
		*/
        void setParam(I16 Location, const KTexture& Texture);

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
        inline U32 getGLID() const {return _kprogId;}

		//! Get version of the shading language
		/*!
			\return A version or release number for the shading language
		*/
        static const std::string getShaderVersion();

    private:
		//! Read content of the file
        void _readFile(const char *FileName, std::vector<char> &data);

		//! Create a shader program (both vertex and fragment)
        bool _createShader(const char *ShaderCod, KShaderTypes ShaderType);

		//! Fill texture units
        void _fillTextureUnits() const;

        U32 _kprogId;	//!< ID of shader program
        std::map<I32, const KTexture *> _ktextureTable;	//!< Texture table
        I32 _kcurrentTexture;		//!< Current texture 
        static U32 _klastProgId;	//!< Last id of shader program 
    };
}

#endif // KSHADER_H
