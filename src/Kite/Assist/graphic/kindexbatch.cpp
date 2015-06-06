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
#include "Kite/Assist/graphic/kindexbatch.h"
#include "Kite/Core/graphic/krender.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include "Kite/core/graphic/ktexturemanager.h"

namespace Kite{
	KIndexBatch::KIndexBatch(const std::vector<KIndexBatchObject *> &Objects,
		const KBatchConfig Config) :
		_kobjects(&Objects),
		_kvboInd(KBT_INDEX),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_krange(0, 0),
		_kconfig(Config)
	{

		KDEBUG_ASSERT_T(!Objects.empty());

		// catching vertex and index offsets
		U32 voff = 0;
		U32 ioff = 0;
		// preserve enough size
		_kvoffset.reserve(Objects.size() + 1);
		_kioffset.reserve(Objects.size() + 1);
		// first offset is always 0
		_kvoffset.push_back(0);
		_kioffset.push_back(0);
		// store vertex and index offset of all objects
		for (U32 i = 0; i < Objects.size(); i++){
			voff += Objects[i]->getVertexSize();
			_kvoffset.push_back(voff);

			ioff += Objects[i]->getIndexSize();
			_kioffset.push_back(ioff);
		}

		// index pre-buffer
		std::vector<U16> indArray;
		indArray.reserve(ioff);

		// position pre-buffer
		std::vector<KVector2F32> posArray;
		posArray.reserve(voff);

		// uv pre-buffer
		std::vector<KVector2F32> uvArray;
		uvArray.reserve(voff);

		// Color pre-buffer
		std::vector<KColor> colArray;
		colArray.reserve(voff);

		// initialize pre-buffers
		const KVertex *vtmp;
		const U16 *itmp;
		U32 icount = 0;
		for (U32 i = 0; i < Objects.size(); i++){

			// vertex data(pos, uv, color)
			vtmp = Objects[i]->getVertex();
			for (U32 j = 0; j < Objects[i]->getVertexSize(); j++){
				posArray.push_back(vtmp[j].pos);
				uvArray.push_back(vtmp[j].uv);
				colArray.push_back(vtmp[j].color);
			}

			// index data
			itmp = Objects[i]->getIndex();
			for (U32 j = 0; j < Objects[i]->getIndexSize(); j++){
				indArray.push_back(itmp[j] + icount);
			}
			icount += Objects[i]->getVertexSize();
		}

		// bind vao then inite buffers
		_kvao.bind();

		// index buffer
		_kvboInd.bind();
		_kvboInd.fill(&indArray[0], sizeof(U16)* indArray.size(), KVB_STATIC);

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

	U32 KIndexBatch::getSize() const{
		U32 size = _kvoffset.size();
		if (size == 0){
			return 0;
		}
		else if (size == 1){
			return _kobjects->at(0)->getVertexSize();
		}
		return _kvoffset[size - 1];
	}

	void KIndexBatch::updatePosition(){
		updatePosition(0, _kobjects->size());
	}

	void KIndexBatch::updatePosition(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _kvoffset[FirstIndex + Size] - _kvoffset[FirstIndex];

		_kvboXY.update(sizeof(KVector2F32)* _kvoffset[FirstIndex],
			sizeof(KVector2F32)* size, false, (void *)this);
	}

	void KIndexBatch::updateUV(){
		updateUV(0, _kobjects->size());
	}

	void KIndexBatch::updateUV(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _kvoffset[FirstIndex + Size] - _kvoffset[FirstIndex];

		_kvboUV.update(sizeof(KVector2F32)* _kvoffset[FirstIndex],
			sizeof(KVector2F32)* size, false, (void *)this);
	}

	void KIndexBatch::updateColor(){
		updateColor(0, _kobjects->size());
	}

	void KIndexBatch::updateColor(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());

		_krange.x = FirstIndex;
		_krange.y = Size;
		U32 size = _kvoffset[FirstIndex + Size] - _kvoffset[FirstIndex];

		_kvboCol.update(sizeof(KColor)* _kvoffset[FirstIndex],
			sizeof(KColor)* size, false, (void *)this);
	}

	void KIndexBatch::draw(){
		draw(0, _kobjects->size());
	}

	void KIndexBatch::draw(U32 FirstIndex, U32 Size){
		Internal::KCatchDraw currentCatch;
		Internal::KCatchDraw lastCatch;

		KDEBUG_ASSERT_T(FirstIndex + Size <= _kobjects->size());
		if (_kvisible && _kobjects->size() > 0){

			// bind our vao
			_kvao.bind();

			U32 iter = FirstIndex, groupIndex = FirstIndex, groupSize = 0;
			bool completed = false;

			// fill catch with first object in array
			lastCatch.objIndex = FirstIndex;
			if (_kobjects->at(FirstIndex)->getTexture())
				lastCatch.lastTexId = _kobjects->at(FirstIndex)->getTexture()->getGLID();
			if (_kobjects->at(FirstIndex)->getShader())
				lastCatch.lastShdId = _kobjects->at(FirstIndex)->getShader()->getGLID();
			lastCatch.lastGeo = _kobjects->at(FirstIndex)->getGeoType();

			while (!completed){

				// iterate over all objects and draw same objects with one draw call
				for (iter; iter < (FirstIndex + Size); iter++){
					currentCatch.objIndex = iter;
					if (_kobjects->at(iter)->getTexture())
						currentCatch.lastTexId = _kobjects->at(iter)->getTexture()->getGLID();
					if (_kobjects->at(iter)->getShader())
						currentCatch.lastShdId = _kobjects->at(iter)->getShader()->getGLID();
					currentCatch.lastGeo = _kobjects->at(iter)->getGeoType();
					if (lastCatch == currentCatch){
						++groupSize;
						continue;
					}
					break;
				}

				// draw same object(s) in group
				// bind shader
				if (_kobjects->at(lastCatch.objIndex)->getShader())
					_kobjects->at(lastCatch.objIndex)->getShader()->bind();

				// bind texture
				if (_kobjects->at(lastCatch.objIndex)->getTexture())
					_kobjects->at(lastCatch.objIndex)->getTexture()->bind();

				// calculate vertex size
				U32 count = _kioffset[groupIndex + groupSize] - _kioffset[groupIndex];

				// draw objects
				KRender::draw(count, (U16 *)(sizeof(U16)* _kioffset[groupIndex]), lastCatch.lastGeo);

				// reset catch
				lastCatch = currentCatch;
				groupSize = 0;
				groupIndex = iter;

				// draw completed
				if (iter >= (FirstIndex + Size))
					completed = true;
			}
		}
	}

	void KIndexBatch::_updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KIndexBatch *obj = (KIndexBatch *)Sender;
		const KTransform *pmat;
		const KTransform *mmat;
		KVector2F32 *pos = (KVector2F32 *)Data;
		const KIndexBatchObject *otmp;
		const KVertex *vtmp;

		// projection matrix
		pmat = obj->getCamera().getTransform();

		// retrieve index, size and offset
		U32 index = obj->_krange.x;
		U32 size = obj->_krange.y;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++){
			otmp = obj->_kobjects->at(index + i);
			mmat = &otmp->getModelViewTransform();
			for (U32 j = 0; j < otmp->getVertexSize(); j++){
				vtmp = &otmp->getVertex()[j];

				// multiply model-matrix (object) with projection-matrix (camera)
				// then update position
				pos[ofst + j] = pmat->transformPoint(mmat->transformPoint(vtmp->pos));
			}

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
	}

	void KIndexBatch::_updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KIndexBatch *obj = (KIndexBatch *)Sender;
		KVector2F32 *uv = (KVector2F32 *)Data;
		const KIndexBatchObject *otmp;
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

	void KIndexBatch::_updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KIndexBatch *obj = (KIndexBatch *)Sender;
		KColor *col = (KColor *)Data;
		const KIndexBatchObject *otmp;
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