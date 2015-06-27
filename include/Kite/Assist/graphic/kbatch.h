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
#include "Kite/Assist/graphic/kdrawable.h"
#include "Kite/Assist/graphic/kbatchobject.h"
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KBatch : public KDrawable{
    public:
        KBatch(const std::vector<KBatchObject *> &Objects, const KBatchConfig Config);

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
		/// draw(0, 1) = draw first object 
        void draw(U32 FirstIndex, U32 Size);

    private:
        static void _updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender);
		static void _updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender);
        const std::vector<KBatchObject *> *_kobjects;
        KVertexArray _kvao;
		KVertexBuffer _kvboXY;		/// xy (position)
        KVertexBuffer _kvboUV;		/// uv (texture uv)
		KVertexBuffer _kvboCol;		/// rgba (color)
        std::vector<U32> _koffset;
		KVector2U32 _krange;
        KBatchConfig _kconfig;
    };
}

#endif // KBATCH_H
