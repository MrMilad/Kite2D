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
#ifndef KSHADER_H
#define KSHADER_H

/*! \file kshader.h */

#include "Kite/core/kcoredef.h"
#include "Kite/core/kresource.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphictypes.h"
#include <string>
#include "kshader.khgen.h"

/*! \namespace Kite
	\brief Public namespace.
*/
KMETA
namespace Kite{

	//! The KShader class allows OpenGL shader programs to be compiled and get ready for shader program.
	/*!
		This class supports shader programs written in the OpenGL Shading Language (GLSL).
	*/
	KM_CLASS(RESOURCE)
    class KITE_FUNC_EXPORT KShader : public KResource{
		KMETA_KSHADER_BODY();
    public:

		//! Default constructors
		/*!
			This constructor creates an invalid shader.
			shader type will detected from file extension after loading. (*.vert or *.frag or *.geom)
		*/
        KShader(const std::string &Name);

		//! Destructor
		/// If a shader object is deleted while it is attached to a program object,
		/// it will be flagged for deletion, and deletion will not occur until 
		/// detach it from all KShaderProgram objects to which it is attached.
        ~KShader();

		bool loadString(const std::string &Code, ShaderType Type);

		inline ShaderType getShaderType() const { return _kshtype; }

		inline const std::string &getCode() const { return _kcode; }

		/// note: this function need an active opengl context.
		bool inite() override;

		//! Method to compile a shader and display any problems if compilation fails
		/*!
		note: this function will called by shader program in the link stage.
		note: this function need an active opengl context.

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

		bool _saveStream(KOStream &Stream, const std::string &Address) override;

		/// reloading new shader will not delete last shader (in opengl)
		/// so carefull about reloading a new shader with an in-use object.
		/// shader type will change according to file type
		bool _loadStream(KIStream &Stream, const std::string &Address) override;

		ShaderType _kshtype;
		U32 _kglid;
		std::string _kcode;
    };
}

#endif // KSHADER_H
