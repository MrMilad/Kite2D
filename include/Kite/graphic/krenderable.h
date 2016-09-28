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
#include "Kite/graphic/kgraphictypes.h"

namespace Kite{
	class KShaderProgram;
	class KAtlasTextureArray;
	class KITE_FUNC_EXPORT KRenderable{
		friend class KRenderSys;
		friend class KGCullingSys;
	public:
		KRenderable() :
			_kmatNeedUpdate(true),
			_ksahder(nullptr),
			_katlas(nullptr)
		{}

	protected:
		virtual void getBoundingRect(KRectF32 &Output) const = 0;
		virtual const std::vector<KGLVertex> *getVertex() const = 0;
		virtual const U32 getIndexSize() const = 0;
		virtual const std::vector<KPointSprite> *getPoint() const = 0;

		virtual const KStringID &getShader() const = 0;
		virtual const KStringID &getAtlasTextureArray() const = 0;

		virtual GLPrimitive getGeoType() const = 0;
		virtual bool isVisible() const = 0;
		virtual bool isIndexed() const = 0;
		virtual bool isPoint() const = 0;

		inline bool getMatNeedUpdate() const { return _kmatNeedUpdate; }
		inline void matNeedUpdate() { _kmatNeedUpdate = true; }
	private:
		bool _kmatNeedUpdate;
		KShaderProgram *_ksahder;
		KAtlasTextureArray *_katlas;
    };
}

#endif // KRENDERABLE_H
