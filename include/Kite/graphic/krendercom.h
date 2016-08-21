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
#ifndef KRENDERCOM_H
#define KRENDERCOM_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcomponent.h"
#include "Kite/meta/kmetadef.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/ktexture.h"
#include <cstring>
#include "krendercom.khgen.h"

KMETA
namespace Kite{
	KM_CLASS(COMPONENT)
	class KITE_FUNC_EXPORT KRenderCom : public KComponent{
		friend class KRenderSys;
		KM_INFO(KI_CTYPE = "Render");
		KMETA_KRENDERCOM_BODY();
    public:
		// array rendering (without index)
		KRenderCom(const std::string &Name = "");

		void attached(KEntity *Entity) override;

		void deattached(KEntity *Entity) override;

		RecieveTypes onMessage(KMessage *Message, MessageScope Scope) override;

		KM_PRO_SET(KP_NAME = "shaderProgram")
		void setShader(const KStringID &ShaderProgram);

		KM_PRO_GET(KP_NAME = "shaderProgram", KP_TYPE = KStringID, KP_CM = "name of the shader program", KP_RES = "KShaderProgram")
		inline const KStringID &getShader() const { return _kshprog; }

		KM_PRO_SET(KP_NAME = "texture")
		void setTexture(const KStringID &Texture);

		KM_PRO_GET(KP_NAME = "texture", KP_TYPE = KStringID, KP_CM = "name of the texture", KP_RES = "KTexture")
		inline const KStringID &getTexture() const { return _ktexture; }

		KM_PRO_SET(KP_NAME = "visible")
		inline void setVisible(bool Visible) { _isvisible = Visible; }

		KM_PRO_GET(KP_NAME = "visible", KP_TYPE = bool, KP_CM = "object visibility")
		inline bool getVisible() const { return _isvisible; }

		// shuld defined in particle component
		//inline void setRelativeTransform(bool Relative) { _krelTrans = Relative; }
		//inline bool getRelativeTransform() const { return _krelTrans; }
		//inline void setReverseRender(bool Reverse) { _kreverse = Reverse; }
		//inline bool getReverseRender() const { return _kreverse; }

    private:
		KM_VAR() bool _isvisible;
		KM_VAR() KStringID _kshprog;
		KM_VAR() KStringID _ktexture;
		KShaderProgram *_kshprogptr;
		KTexture *_ktextureptr;
    };
}

#endif // KRENDERCOM_H
