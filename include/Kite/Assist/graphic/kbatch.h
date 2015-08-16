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
#ifndef KBATCH_H
#define KBATCH_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kvertexarray.h"
#include "Kite/Core/graphic/kvertexbuffer.h"
#include "Kite/Assist/graphic/kcamera.h"
#include "Kite/Assist/graphic/kbatchobject.h"
#include <vector>

namespace Kite{
	class KITE_FUNC_EXPORT KBatch : KNonCopyable{
	public:
		KBatch(const std::vector<KBatchObject *> &Objects, const KBatchConfig Config, bool PointSprite);
		KBatch(U32 VertexSize, U32 IndexSize, const KBatchConfig Config, bool PointSprite);

		inline U32 getVertexSize() const { return _kvsize; }
		inline U32 getIndexSize() const { return _kisize; }

		inline void setCamera(const KCamera &Camera) { _kcam = &Camera; }
		inline const KCamera &getCamera() const { return *_kcam; }

		/// draw a single object
		void draw(const KBatchObject *Object, const KBatchUpdate &Update);

		/// draw a vector of objects
		void draw(const std::vector<KBatchObject *> &Objects, const KBatchUpdate &Update);

	private:
		void _draw(const std::vector<const KBatchObject *> &Objects, U32 VSize, U32 ISize, const KBatchUpdate &Update);
		static void _updateInd(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateVer(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updatePar(void *Data, U32 Offset, U32 DataSize, void *Sender);
		
		std::vector<const KBatchObject *> _kobj;
		Internal::KUpdateSender _ksender;
		const KCamera *_kcam;
		static const KCamera _kdefcam;
		KVertexArray _kvao;
		KVertexBuffer _kvboInd;		/// index
		KVertexBuffer _kvboVer;		/// xy (position)
		KVertexBuffer _kvboPnt;		/// point sprite: point size and texture size (usage: Particle)
		KBatchConfig _kconfig;
		U32 _kvsize;
		U32 _kisize;
		bool _kpsprite;
	};
}

#endif // KBATCH_H
