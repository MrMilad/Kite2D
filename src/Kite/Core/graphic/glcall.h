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
#ifndef GLCALL_H
#define GLCALL_H

/*! \file glcall.h */

//#define GLEW_STATIC
#include "extlibs/headers/gl/glew.h"
#include "Kite/core/system/ksystemdef.h"
#include "Kite/core/graphic/kgraphictypes.h"

/*! \namespace Kite
	\brief Public namespace.
*/
namespace Kite{

/*! \namespace Kite::Internal
	\brief Private namespace.
*/
namespace Internal{

	//! Initialize GLEW library.
	/*!
		\return Return false if there is an error.
	*/
    extern bool initeGLEW();

	//! Checking last OpenGL error.
	/*!
		\return Return false if there is an error.
	*/
    extern bool checkLastGLErr();

	//! The GLBindGuard class is the utility class for managing binding opengl objects.
	/*!
		This class helps prevent the loss of the last OpenGL bind() when we do a temporary bind().
		all kite graphic classes use this class internally and the user does not need to manage it.
	*/
    class GLBindGuard{
    public:
		//! Constructs an GLBindGuard object with a type and a ID.
		/*!
			\param Type An enum that specifies the type of OpenGL object.
			\param ObjectID Object ID.
		*/
        GLBindGuard(KGLBindGuardTypes Type = KBG_NONE, GLint ObjectID = 0);

		//! Destroys the GLBindGuard object.
		/*!
			re-bind opengl stored object in destructor.
		*/
        ~GLBindGuard();

    private:
        KGLBindGuardTypes _ktypes; //!< OpenGL object type
        GLuint _kglobject; //!< OpenGL object ID
    };

	/*! \def DGL_CALL
	\brief OpenGL debug macro.
	\param call Any OpenGL function in debug mode
	*/
	#if defined(KITE_DEV_DEBUG)
	#define DGL_CALL(call) call; KDEBUG_ASSERT_T(Kite::Internal::checkLastGLErr())
	#else
	#define DGL_CALL(call) call;
	#endif
}
}

#endif // GLCALL_H
