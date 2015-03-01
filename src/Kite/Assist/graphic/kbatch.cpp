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
#include "Kite/Assist/graphic/kbatch.h"
#include "Kite/Core/graphic/krender.h"
#include "Kite/Core/graphic/kgraphicdef.h"

namespace Kite{
	KBatch::KBatch(const std::vector<KBatchObject *> &Objects,
		const KBatchConfig Config) :
		_kobjects(&Objects),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_krange(0,0),
		_kconfig(Config),
		_ktexture(0),
		_kshader(0),
		_kgtype(KGP_POINTS)
    {

		KDEBUG_ASSERT_T(!Objects.empty());

		// catching vertex offset 
		U32 offset = 0;
		_koffset.reserve(Objects.size() + 1);
		// first offset is always 0
		_koffset.push_back(0);
		// store vertex offset of all objects
		for (U32 i = 0; i < Objects.size(); i++){
			offset += Objects[i]->getVertexSize();
			_koffset.push_back(offset);
		}

		// position pre-buffer
		std::vector<KVector2F32> posArray;
		posArray.reserve(offset);

        // uv pre-buffer
		std::vector<KVector2F32> uvArray;
		uvArray.reserve(offset);

		// Color pre-buffer
		std::vector<KColor> colArray;
		colArray.reserve(offset);

        // initialize pre-buffers with vertex data (pos,uv,color)
        const KVertex *vtmp;
		for (U32 i = 0; i < Objects.size(); i++){
			vtmp = Objects[i]->getVertex();
			for (U32 j = 0; j < Objects[i]->getVertexSize(); j++){
				posArray.push_back(vtmp[j].pos);
				uvArray.push_back(vtmp[j].uv);
				colArray.push_back(vtmp[j].color);
            }
        }

        // bind vao then inite buffers
        _kvao.bind();

        // position buffer
        _kvboXY.bind();
		_kvboXY.fill(&posArray[0], sizeof(KVector2F32)* posArray.size(), _kconfig.position);
        _kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboXY.setUpdateHandle(_updatePos);

        // uv buffer
        _kvboUV.bind();
		_kvboUV.fill(&uvArray[0], sizeof(KVector2F32)* uvArray.size(), _kconfig.uv);
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboUV.setUpdateHandle(_updateUV);

		// color buffer
		_kvboCol.bind();
		_kvboCol.fill(&colArray[0], sizeof(KColor)* colArray.size(), _kconfig.color);
        _kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KColor), KBUFFER_OFFSET(0));
		_kvboCol.setUpdateHandle(_updateCol);

        // finish. unbind vao.
        _kvao.unbindVertexArray();
    }

    U32 KBatch::getSize() const{
		U32 size = _koffset.size();
		if (size == 0){
			return 0;
		}
		else if (size == 1){
			return _kobjects->at(0)->getVertexSize();
		}
		return _koffset[size - 1];
    }

    void KBatch::updatePosition(){
        updatePosition(0, _kobjects->size());
    }

	void KBatch::updatePosition(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _koffset[FirstIndex + Size] - _koffset[FirstIndex];

        _kvboXY.update(sizeof(KVector2F32)* _koffset[FirstIndex],
			sizeof(KVector2F32)* size, false, (void *)this);
	}

	void KBatch::updateUV(){
        updateUV(0, _kobjects->size());
	}

	void KBatch::updateUV(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _koffset[FirstIndex + Size] - _koffset[FirstIndex];

		_kvboUV.update(sizeof(KVector2F32)* _koffset[FirstIndex],
			sizeof(KVector2F32)* size, false, (void *)this);
	}

	void KBatch::updateColor(){
        updateColor(0, _kobjects->size());
	}

	void KBatch::updateColor(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _koffset[FirstIndex + Size] - _koffset[FirstIndex];

		_kvboCol.update(sizeof(KColor)* _koffset[FirstIndex],
			sizeof(KColor)* size, false, (void *)this);
	}

    void KBatch::draw(){
        draw(0, _kobjects->size());
    }

    void KBatch::draw(U32 FirstIndex, U32 Size){
		if (_kvisible){
			// bind shader
			if (_kshader)
				_kshader->bind();

			// bind texture
			if (_ktexture)
				_ktexture->bind();

			// bind our vao
			_kvao.bind();

            KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

			// calculate size
			U32 size = _koffset[FirstIndex + Size] - _koffset[FirstIndex];

			KRender::draw(_koffset[FirstIndex], size, _kgtype);
		}
	}

    void KBatch::_updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender){
        KBatch *obj = (KBatch *)Sender;
		const KTransform *pmat;
		const KTransform *mmat;
		KVector2F32 *pos = (KVector2F32 *)Data;
        const KBatchObject *otmp;
        const KVertex *vtmp;

		// projection matrix
		pmat = obj->getCamera().getTransform();

        // retrieve index, size and offset
		U32 index = obj->_krange.x;
		U32 size = obj->_krange.y;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++){
            otmp = obj->_kobjects->at(index + i);
			mmat = otmp->getTransform();
			for (U32 j = 0; j < otmp->getVertexSize(); j++){
				vtmp = &otmp->getVertex()[j];

                // multiply model-matrix with projection-matrix (camera)
				// then update position
				pos[ofst + j] = pmat->transformPoint(mmat->transformPoint(vtmp->pos));
            }

			// move offset to next object
			ofst += otmp->getVertexSize();
        }
    }

	void KBatch::_updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KBatch *obj = (KBatch *)Sender;
		KVector2F32 *uv = (KVector2F32 *)Data;
        const KBatchObject *otmp;
        const KVertex *vtmp;

		// retrieve index, size and offset
		U32 index = obj->_krange.x;
		U32 size = obj->_krange.y;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++){
            otmp = obj->_kobjects->at(index + i);
			for (U32 j = 0; j < otmp->getVertexSize(); j++){
				vtmp = &otmp->getVertex()[j];

                // update uv
				uv[ofst + j] = vtmp->uv;
            }

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
	}

	void KBatch::_updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KBatch *obj = (KBatch *)Sender;
		KColor *col = (KColor *)Data;
        const KBatchObject *otmp;
        const KVertex *vtmp;

		// retrieve index, size and offset
		U32 index = obj->_krange.x;
		U32 size = obj->_krange.y;
		U32 ofst = 0;

        for (U32 i = 0; i < size; i++){
            otmp = obj->_kobjects->at(index + i);
			for (U32 j = 0; j < otmp->getVertexSize(); j++){
				vtmp = &otmp->getVertex()[j];

                // update color
				col[ofst + j] = vtmp->color;
            }

			// move offset to next object
			ofst += otmp->getVertexSize();
        }
	}
}
