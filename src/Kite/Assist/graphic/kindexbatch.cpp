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
#include "Kite/Assist/graphic/kindexbatch.h"
#include "Kite/Core/graphic/krender.h"
#include "Kite/Core/graphic/kgraphicdef.h"
#include "Kite/core/graphic/ktexturemanager.h"

namespace Kite{
	const KCamera KIndexBatch::_kdefcam;
	KIndexBatch::KIndexBatch(const std::vector<KIndexBatchObject *> &Objects,
		const KBatchConfig Config) :
		_kcam(&_kdefcam),
		_kvboInd(KBT_INDEX),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(0),
		_kisize(0)
	{

		KDEBUG_ASSERT_T(!Objects.empty());

		// store vertex and index offset of all objects
		for (U32 i = 0; i < Objects.size(); i++){
			_kvsize += Objects[i]->getVertexSize();
			_kisize += Objects[i]->getIndexSize();
		}

		KDEBUG_ASSERT_T(_kvsize > 0);
		KDEBUG_ASSERT_T(_kisize > 0);

		_kobj.reserve(Objects.size());
		// initialize index
		std::vector<U16> ind(_kisize, 0);

		// initialize vertex
		std::vector<KVector2F32> ve(_kvsize, KVector2F32(0,0));

		// initialize color
		std::vector<KColor> co(_kvsize, KColor());

		// bind vao then inite buffers
		_kvao.bind();

		// index buffer
		_kvboInd.bind();
		_kvboInd.fill(&ind[0], sizeof(U16) * _kisize, _kconfig.index);
		_kvboInd.setUpdateHandle(_updateInd);

		// position buffer
		_kvboXY.bind();
		_kvboXY.fill(&ve[0], sizeof(KVector2F32) * _kvsize, _kconfig.position);
		_kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboXY.setUpdateHandle(_updatePos);

		// uv buffer
		_kvboUV.bind();
		_kvboUV.fill(&ve[0], sizeof(KVector2F32) * _kvsize, _kconfig.uv);
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboUV.setUpdateHandle(_updateUV);

		// color buffer
		_kvboCol.bind();
		_kvboCol.fill(&co[0], sizeof(KColor) * _kvsize, _kconfig.color);
		_kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KColor), KBUFFER_OFFSET(0));
		_kvboCol.setUpdateHandle(_updateCol);

		// finish. unbind vao.
		_kvao.unbindVertexArray();
	}

	KIndexBatch::KIndexBatch(U32 VertexSize, U32 IndexSize, const KBatchConfig Config) :
		_kcam(&_kdefcam),
		_kvboInd(KBT_INDEX),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(VertexSize),
		_kisize(IndexSize) 
	{

		KDEBUG_ASSERT_T(IndexSize > 0);
		KDEBUG_ASSERT_T(VertexSize > 0);

		// initialize index
		std::vector<U16> ind(_kisize, 0);

		// initialize vertex
		std::vector<KVector2F32> ve(_kvsize, KVector2F32(0, 0));

		// initialize color
		std::vector<KColor> co(_kvsize, KColor());

		// bind vao then inite buffers
		_kvao.bind();

		// index buffer
		_kvboInd.bind();
		_kvboInd.fill(&ind[0], sizeof(U16)* IndexSize, _kconfig.index);
		_kvboInd.setUpdateHandle(_updateInd);

		// position buffer
		_kvboXY.bind();
		_kvboXY.fill(&ve[0], sizeof(KVector2F32)* VertexSize, _kconfig.position);
		_kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboXY.setUpdateHandle(_updatePos);

		// uv buffer
		_kvboUV.bind();
		_kvboUV.fill(&ve[0], sizeof(KVector2F32)* VertexSize, _kconfig.uv);
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboUV.setUpdateHandle(_updateUV);

		// color buffer
		_kvboCol.bind();
		_kvboCol.fill(&co[0], sizeof(KColor)* VertexSize, _kconfig.color);
		_kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KColor), KBUFFER_OFFSET(0));
		_kvboCol.setUpdateHandle(_updateCol);

		// finish. unbind vao.
		_kvao.unbindVertexArray();
	}

	void KIndexBatch::draw(const KIndexBatchObject *Object, const KBatchUpdate &Update) {
		if (!Object)
			return;

		if (Object->getIndexSize() > _kisize || Object->getVertexSize() > _kvsize) {
			KDEBUG_PRINT("object size is greater than buffer size.");
			return;
		}

		if (!Object->getVisible()) 
			return;

		// update vertex attributes
		_ksender.arraySize = 1;
		_ksender.firstObject = (const void *)&Object;
		if (Update.index) {
			_kvboInd.update(0, sizeof(U16) * Object->getIndexSize(), false, (void *)this);
		}
		if (Update.position) {
			_kvboXY.update(0, sizeof(KVector2F32) * Object->getVertexSize(), false, (void *)this);
		} 
		if (Update.uv) {
			_kvboUV.update(0, sizeof(KVector2F32) * Object->getVertexSize(), false, (void *)this);
		} 
		if (Update.color) {
			_kvboCol.update(0, sizeof(KColor) * Object->getVertexSize(), false, (void *)this);
		} 

		_kvao.bind();

		// bind shader
		if (Object->getShader())
			Object->getShader()->bind();

		// bind texture
		if (Object->getTexture())
			Object->getTexture()->bind();

		// draw objects
		KRender::draw(Object->getIndexSize(), (U16 *)0, Object->getGeoType());
	}

	void KIndexBatch::draw(const std::vector<KIndexBatchObject *> &Objects, const KBatchUpdate &Update) {
		if (Objects.empty())
			return;
		
		bool needDraw = true;
		U32 counter = 0;
		

		// update buffers and draw
		while (needDraw) {

			U32 vsize = 0, isize = 0;
			U32 firstInList = counter;
			_kobj.clear();

			for (counter; counter < Objects.size(); counter++) {

				if (!Objects.at(counter)->getVisible())
					continue;

				if ((vsize + Objects.at(counter)->getVertexSize()) <= _kvsize &&
					(isize + Objects.at(counter)->getIndexSize()) <= _kisize) {
					vsize += Objects.at(counter)->getVertexSize();
					isize += Objects.at(counter)->getIndexSize();
					_kobj.push_back(Objects.at(counter));
					//printf("obj: %u\n", _kobj.size());

				} else {
					break;
				}
			}

			if (vsize == 0 || isize == 0) {
				KDEBUG_PRINT("object size is greater than buffer size.");
				return;
			}

			// draw completed
			if (counter >= Objects.size())
				needDraw = false;

			_draw(_kobj, vsize, isize, Update);
		}
	}

	void KIndexBatch::_draw(const std::vector<KIndexBatchObject *> &Objects, U32 VSize, U32 ISize, const KBatchUpdate &Update) {
		// update vertex attributes
		_ksender.arraySize = Objects.size();
		_ksender.firstObject = (const void *)&Objects[0];
		if (Update.index) {
			_kvboInd.update(0, sizeof(U16)* ISize, false, (void *)this);
		}
		if (Update.position) {
			_kvboXY.update(0, sizeof(KVector2F32)* VSize, false, (void *)this);
		}
		if (Update.uv) {
			_kvboUV.update(0, sizeof(KVector2F32)* VSize, false, (void *)this);
		}
		if (Update.color) {
			_kvboCol.update(0, sizeof(KColor)* VSize, false, (void *)this);
		}

		// catch objects state (shader, texture, ...)
		// and render same object with single draw call
		Internal::KCatchDraw currentCatch;
		Internal::KCatchDraw tempCatch;

		U32 iter = 0, groupOfst = 0, groupSize = 0;
		bool completed = false;

		// fill catch with first object in array
		currentCatch.objIndex = 0;
		if (Objects.at(0)->getTexture()) {
			currentCatch.lastTexId = Objects.at(0)->getTexture()->getGLID();
		} else {
			currentCatch.lastTexId = 0;
		}
		if (Objects.at(0)->getShader())
			currentCatch.lastShdId = Objects.at(0)->getShader()->getGLID();
		currentCatch.lastGeo = Objects.at(0)->getGeoType();

		_kvao.bind();

		// compare objects
		while (!completed) {

			// iterate over all objects and draw same objects with one draw call
			for (iter = iter; iter < Objects.size(); iter++) {
				tempCatch.objIndex = iter;
				if (Objects.at(iter)->getTexture()) {
					tempCatch.lastTexId = Objects.at(iter)->getTexture()->getGLID();
				} else {
					tempCatch.lastTexId = 0;
				}
				if (Objects.at(iter)->getShader())
					tempCatch.lastShdId = Objects.at(iter)->getShader()->getGLID();
				currentCatch.lastGeo = Objects.at(iter)->getGeoType();
				if (tempCatch == currentCatch) {
					groupSize += Objects.at(iter)->getIndexSize();
					continue;
				}
				break;
			}

			// draw same object(s) in group
			// bind shader
			if (Objects.at(currentCatch.objIndex)->getShader())
				Objects.at(currentCatch.objIndex)->getShader()->bind();

			// bind texture
			if (Objects.at(currentCatch.objIndex)->getTexture()) {
				Objects.at(currentCatch.objIndex)->getTexture()->bind();
			} else {
				KTexture::unbindTexture();
			}

			// draw objects
			KRender::draw(groupSize, (U16 *)(sizeof(U16) * groupOfst), currentCatch.lastGeo);

			//printf("gs: %u\n gf: %u \n", groupSize, groupOfst);

			// reset catch
			currentCatch = tempCatch;
			groupOfst = groupSize;
			groupSize = 0;

			// draw completed
			if (iter >= Objects.size())
				completed = true;
		}
	}

	void KIndexBatch::_updateInd(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KIndexBatch *clObject = (KIndexBatch *)Sender;
		const KIndexBatchObject **obj = (const KIndexBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		U16 *ind = (U16 *)Data;
		const KIndexBatchObject *otmp;
		U16 itmp;

		// retrieve index, size and offset
		U32 iofst = 0;
		U32 vofst = 0;

		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			for (U32 j = 0; j < otmp->getIndexSize(); j++) {
				itmp = otmp->getIndex()[j];

				// update color
				ind[iofst + j] = itmp + vofst;
			}

			// move offset to next object
			iofst += otmp->getIndexSize();
			vofst += otmp->getVertexSize();
		}
	}

	void KIndexBatch::_updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KIndexBatch *clObject = (KIndexBatch *)Sender;
		const KIndexBatchObject **obj = (const KIndexBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		const KTransform *pmat;
		const KTransform *mmat;
		KVector2F32 *pos = (KVector2F32 *)Data;
		const KIndexBatchObject *otmp;
		const KVertex *vtmp;

		// projection matrix
		pmat = clObject->getCamera().getTransform();

		U32 ofst = 0;
		for (U32 i = 0; i < size; i++){
			otmp = obj[i];
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
		KIndexBatch *clObject = (KIndexBatch *)Sender;
		const KIndexBatchObject **obj = (const KIndexBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KVector2F32 *uv = (KVector2F32 *)Data;
		const KIndexBatchObject *otmp;
		const KVertex *vtmp;

		U32 ofst = 0;
		for (U32 i = 0; i < size; i++){
			otmp = obj[i];
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
		KIndexBatch *clObject = (KIndexBatch *)Sender;
		const KIndexBatchObject **obj = (const KIndexBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KColor *col = (KColor *)Data;
		const KIndexBatchObject *otmp;
		const KVertex *vtmp;

		// retrieve index, size and offset
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++){
			otmp = obj[i];
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