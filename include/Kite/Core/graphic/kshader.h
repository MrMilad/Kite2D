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
#include "Kite/Core/system/kinputstream.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include <string>

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

	//! The KShader class allows OpenGL shader programs to be compiled and get ready for shader program.
	/*!
		This class supports shader programs written in the OpenGL Shading Language (GLSL).
	*/
    class KITE_FUNC_EXPORT KShader : KNonCopyable{
    public:

		//! Default constructors
		/*!
			This constructor creates an invalid shader.

			\param Types type of the shader (vertex or fragment)
		*/
        KShader(KShaderTypes Types);

		//! Destructor
        ~KShader();

		//! Load the shader (vertex or fragment) from the files on disk
		/*!
			\param ShaderFile Address of the shader file on the disk

			\return True if loading was successful
		*/
        bool loadFile(const std::string &ShaderFile);

		//! Load the shader (vertex or fragment) from a string
		/*!
			\param ShaderCod Source code of the sahder

			\return True if loading was successful
		*/
        bool loadMemory(const std::string &ShaderCod);

		//! Load both the vertex and fragment shaders from input stream
		/*!
			\param Vertex InputStream of the vertex shader
			\param Fragment InputStream of the fragment shader

			\return True if loading was successful
		*/
		bool loadStream(KInputStream &ShaderStream);

		//! Method to compile a shader and display any problems if compilation fails
		/*!
			\return True if compile was successful
		*/
		bool compile();

		//! Get OpenGL ID of the shader
		/*!
			\return OpenGL ID of the shader
		*/
		inline U32 getGLID() const { return _kglid; }

		//! Get version of the shading language
		/*!
			\return A version or release number for the shading language
		*/
        static const std::string getShaderVersion();

    private:
		KShaderTypes _ktype;
		U32 _kglid;
    };
}

#endif // KSHADER_H
