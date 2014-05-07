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
#ifndef KINDEXBATCH_H
#define KINDEXBATCH_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kshader.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/kvertexarray.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Assist/graphic/kdrawable.h"
#include "Kite/Assist/graphic/kindexbatchobject.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KIndexBatch : public KDrawable{
	public:
		KIndexBatch(const std::vector<KIndexBatchObject *> &Objects, const KBatchConfig Config);

		inline void setShader(const KShader *Shader) { _kshader = Shader; }
		inline const KShader *getShader() const { return _kshader; }

		inline void setTexture(const KTexture *Texture) { _ktexture = Texture; }
		inline const KTexture *getTexture() const { return _ktexture; }

		inline void setGeoType(KGeoPrimitiveTypes GeoType) { _kgtype = GeoType; }
		inline KGeoPrimitiveTypes getGeoType() const { return _kgtype; }

		U32 getSize() const;

		/// update positions
		void updatePosition();
		void updatePosition(U32 FirstIndex, U32 Size);

		/// update uv
		void updateUV();
		void updateUV(U32 FirstIndex, U32 Size);

		/// update colors
		void updateColor();
		void updateColor(U32 FirstIndex, U32 Size);

		/// draw all objects
		void draw();

		/// draw a part of objects
		void draw(U32 FirstIndex, U32 Size);

	private:
		static void _updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender);
		const std::vector<KIndexBatchObject *> *_kobjects;
		KVertexArray _kvao;
		KVertexBuffer _kvboInd;		/// index
		KVertexBuffer _kvboXY;		/// xy (position)
		KVertexBuffer _kvboUV;		/// uv (texture uv)
		KVertexBuffer _kvboCol;		/// rgba (color)
		std::vector<U32> _kvoffset;	/// vertex offset
		std::vector<U32> _kioffset;	/// index offset
		KVector2U32 _krange;
		KBatchConfig _kconfig;
		const KShader *_kshader;
		const KTexture *_ktexture;
		KGeoPrimitiveTypes _kgtype;
	};
}

#endif // KINDEXBATCH_H
