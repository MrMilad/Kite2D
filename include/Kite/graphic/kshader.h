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
#include "Kite/meta/kmetadef.h"
#include "Kite/ecs/kresourcemanager.h"
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
		KM_INFO(KI_NAME = "Shader");
		KSHADER_BODY();
    public:
        KShader(const std::string &Code, ShaderType Type);

		//! Destructor
		/// If a shader object is deleted while it is attached to a program object,
		/// it will be flagged for deletion, and deletion will not occur until 
		/// detach it from all KShaderProgram objects to which it is attached.
        ~KShader();

		KShader() = delete;
		KShader(const KShader &Copy) = delete;
		KShader &operator=(const KShader &Right) = delete;

		bool saveStream(KIOStream &Stream, const std::string &Address) override;

#ifdef KITE_EDITOR
		inline void setCode(const std::string &Code) { _kcode = Code; }
#endif

		KM_PRO_GET(KP_NAME = "type", KP_TYPE = ShaderType, KP_CM = "type of the shader")
		inline ShaderType getShaderType() const { return _kshtype; }

		KM_PRO_GET(KP_NAME = "code", KP_TYPE = std::string)
		inline const std::string &getCode() const { return _kcode; }

		//! Method to compile a shader and display any problems if compilation fails
		/*!
		note: this function will called by shader program in the link stage.
		note: this function need an active opengl context.

			\return True if compile was successful
		*/
		KM_FUN()
		bool compile();

		//! Get OpenGL ID of the shader
		/*!
			\return OpenGL ID of the shader
		*/
		KM_PRO_GET(KP_NAME = "glID", KP_TYPE = U32)
		inline U32 getGLID() const { return _kglid; }

		//! Get version of the shading language
		/*!
			\return A version or release number for the shading language
		*/
		KM_FUN(KP_NAME = "version")
        static std::string getShaderVersion();

    private:
		KShader(const std::string &Name, const std::string &Address);

		KM_FUN(KP_NAME = "__call")
		static KSharedResource luaConstruct(const std::string &Code, ShaderType Type);

		bool _loadStream(std::unique_ptr<KIOStream> Stream, KResourceManager *RManager) override;

		bool _kisCreated;
		ShaderType _kshtype;
		U32 _kglid;
		std::string _kcode;
    };
}

#endif // KSHADER_H
