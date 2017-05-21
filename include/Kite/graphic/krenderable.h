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
#ifndef KRENDERABLE_H
#define KRENDERABLE_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/graphic/kgraphictypes.h"
#include "krenderable.khgen.h"

KMETA
namespace Kite{
	class KRenderSys;
	KM_CLASS(INTERFACE)
	class KITE_FUNC_EXPORT KRenderable{
		KM_INFO(KI_NAME = "Renderable");
		KRENDERABLE_BODY();
		friend class KRenderSys;
	public:
		virtual ~KRenderable() {};

	protected:
		virtual const std::vector<KGLVertex> *getVertex() const = 0;
		virtual const U32 getIndexSize() const = 0;
		virtual const std::vector<KPointSprite> *getPoint() const = 0;

		virtual KSharedResource getShaderProg() const = 0;
		virtual KSharedResource getATextureArray() const = 0;

		virtual GLPrimitive getGeoType() const = 0;
		virtual bool isVisible() const = 0;
		virtual bool isIndexed() const = 0;
		virtual bool isPoint() const = 0;
    };

	KRENDERABLE_SOURCE();
}

#endif // KRENDERABLE_H
