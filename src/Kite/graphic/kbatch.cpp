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
#include "Kite/Core/graphic/kbatch.h"
#include "Kite/Core/graphic/krender.h"
#include "Kite/Core/graphic/kgraphicdef.h"

namespace Kite{
	const KCamera KBatch::_kdefcam;
	KBatch::KBatch(const std::vector<KBatchObject *> &Objects,
		const KBatchConfig Config, bool PointSprite) :
		_kcam(&_kdefcam),
		_kvboInd(KBT_INDEX),
		_kvboVer(KBT_VERTEX),
		_kvboPnt(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(0),
		_kisize(0),
		_kpsprite(PointSprite)
	{

		KDEBUG_ASSERT_T(!Objects.empty());

		// store vertex and index offset of all objects
		for (size_t i = 0; i < Objects.size(); i++){
			_kvsize += Objects[i]->getVertexSize();
			_kisize += Objects[i]->getIndexSize();
		}

		// inite internal vector
		_kobj.reserve(Objects.size());

		// update pre buffers
		_ksender.arraySize = Objects.size();
		_ksender.firstObject = (const void *)&Objects[0];

		// inite index
		std::vector<U16> ind(_kisize, 0);
		if (_kisize > 0) {
			_updateInd(&ind[0], 0, 0, this);
		}

		// inite vertex
		std::vector<KVertex> vert(_kvsize, KVertex());
		_updateVer(&vert[0], 0, 0, this);

		// inite point
		std::vector<KPointSprite> po;
		if (_kpsprite) {
			po.resize(_kvsize, KPointSprite());
			_updatePar(&po[0], 0, 0, this);
		}

		// bind vao then inite buffers
		_kvao.bind();

		// index buffer
		if (_kisize > 0) {
			_kvboInd.bind();
			_kvboInd.fill(&ind[0], sizeof(U16)* _kisize, (KVertexBufferTypes)_kconfig.index);
			_kvboInd.setUpdateHandle(_updateInd);
		}

		// vertex buffer
		_kvboVer.bind();
		_kvboVer.fill(&vert[0], sizeof(KVertex)* _kvsize, (KVertexBufferTypes)_kconfig.vertex);

		// pos
		_kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(0));

		// uv
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(sizeof(KVector2F32)));

		// color
		_kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(sizeof(KVector2F32)* 2));
		_kvboVer.setUpdateHandle(_updateVer);

		// point sprite buffer
		if (_kpsprite) {
			_kvboPnt.bind();
			_kvboPnt.fill(&po[0], sizeof(KPointSprite)* _kvsize, (KVertexBufferTypes)_kconfig.point);
			// point sprite
			_kvao.enableAttribute(3);
			_kvao.setAttribute(3, KAC_3COMPONENT, KAT_FLOAT, false, sizeof(KPointSprite), KBUFFER_OFFSET(0));
			_kvboPnt.setUpdateHandle(_updatePar);
		}

		// finish. unbind vao.
		_kvao.unbindVertexArray();
	}

	KBatch::KBatch(U32 VertexSize, U32 IndexSize, const KBatchConfig Config, bool PointSprite) :
		_kcam(&_kdefcam),
		_kvboInd(KBT_INDEX),
		_kvboVer(KBT_VERTEX),
		_kvboPnt(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(VertexSize),
		_kisize(IndexSize),
		_kpsprite(PointSprite)
	{

		// inite index
		std::vector<U16> ind(_kisize, 0);

		// inite vertex
		std::vector<KVertex> vert(_kvsize, KVertex());

		// inite point
		std::vector<KPointSprite> po;
		if (_kpsprite) {
			po.resize(_kvsize, KPointSprite());
			_updatePar(&po[0], 0, 0, this);
		}

		// bind vao then inite buffers
		_kvao.bind();

		// index buffer
		if (_kisize > 0) {
			_kvboInd.bind();
			_kvboInd.fill(&ind[0], sizeof(U16)* _kisize, (KVertexBufferTypes)_kconfig.index);
			_kvboInd.setUpdateHandle(_updateInd);
		}

		// vertex buffer
		_kvboVer.bind();
		_kvboVer.fill(&vert[0], sizeof(KVertex)* _kvsize, (KVertexBufferTypes)_kconfig.vertex);

		// pos
		_kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(0));

		// uv
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(sizeof(KVector2F32)));

		// color
		_kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KVertex), KBUFFER_OFFSET(sizeof(KVector2F32)* 2));
		_kvboVer.setUpdateHandle(_updateVer);

		// point sprite buffer
		if (_kpsprite) {
			_kvboPnt.bind();
			_kvboPnt.fill(&po[0], sizeof(KPointSprite)* _kvsize, (KVertexBufferTypes)_kconfig.point);
			// point sprite
			_kvao.enableAttribute(3);
			_kvao.setAttribute(3, KAC_3COMPONENT, KAT_FLOAT, false, sizeof(KPointSprite), KBUFFER_OFFSET(0));
			_kvboPnt.setUpdateHandle(_updatePar);
		}

		// finish. unbind vao.
		_kvao.unbindVertexArray();
	}

	void KBatch::draw(const KBatchObject *Object, const KBatchUpdate &Update) {
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
		if (Object->getIndexed()) {
			_kvboInd.update(0, sizeof(U16) * Object->getIndexSize(), false, (void *)this);
		}
		if (Update.vertex) {
			_kvboVer.update(0, sizeof(KVector2F32) * Object->getVertexSize(), false, (void *)this);
		} 
		if (_kpsprite && Update.point && Object->getSpriteEnabled()) {
			_kvboPnt.update(0, sizeof(KPointSprite)* Object->getPointSize(), false, (void *)this);
		}

		_kvao.bind();

		// bind shader
		if (Object->getShader())
			Object->getShader()->bind();

		// bind texture
		if (Object->getTexture())
			Object->getTexture()->bind();

		// draw objects
		if (Object->getIndexed()) {
			KRender::draw(Object->getIndexSize(), (U16 *)0, Object->getGeoType());
		} else {
			KRender::draw(0, Object->getVertexSize(), Object->getGeoType());
		}
	}

	void KBatch::draw(const std::vector<KBatchObject *> &Objects, const KBatchUpdate &Update) {
		if (Objects.empty())
			return;
		
		bool needDraw = true, needFilter = true;
		bool indexed = Objects[0]->getIndexed();
		U32 counter = 0;
		U32 start = 0;
		
		while (needFilter) {

			U32 lenght = 0;
			// filtering objects by render types (insexed or array)
			for (counter; counter < Objects.size(); counter++) {
				if (indexed == Objects[counter]->getIndexed()) {
					++lenght;
					needDraw = true;
				} else {
					indexed = Objects[counter]->getIndexed();
					break;
				}
			}
			lenght += start;

			// draw filtered objects
			while (needDraw) {

				U32 vsize = 0, isize = 0;
				_kobj.clear();

				for (start; start < lenght; start++) {

					if (!Objects[start]->getVisible())
						continue;

					if ((vsize + Objects[start]->getVertexSize()) <= _kvsize &&
						(isize + Objects[start]->getIndexSize()) <= _kisize) {
						vsize += Objects[start]->getVertexSize();
						isize += Objects[start]->getIndexSize();
						_kobj.push_back(Objects[start]);

					} else {
						break;
					}
				}

				if (vsize == 0 || (isize == 0 && Objects[lenght - 1]->getIndexed())) {
					KDEBUG_PRINT("object size is greater than buffer size.");
					return;
				}

				// draw
				_draw(_kobj, vsize, isize, Update);

				// draw completed
				if (start >= lenght)
					needDraw = false;
			}

			// filtering completed
			if (counter >= Objects.size())
				needFilter = false;
		}

	}

	void KBatch::_draw(const std::vector<const KBatchObject *> &Objects, U32 VSize, U32 ISize, const KBatchUpdate &Update) {
		// update vertex attributes
		_ksender.arraySize = Objects.size();
		_ksender.firstObject = (const void *)&Objects[0];
		if (ISize > 0) {
			_kvboInd.update(0, sizeof(U16)* ISize, false, (void *)this);
		}
		if (Update.vertex) {
			_kvboVer.update(0, sizeof(KVector2F32)* VSize, false, (void *)this);
		}
		if (_kpsprite && Update.point) {
			_kvboPnt.update(0, sizeof(KPointSprite)* VSize, false, (void *)this);
		}

		// catch objects state (shader, texture, ...)
		// and render same object with single draw call
		Internal::KCatchDraw currentCatch;
		Internal::KCatchDraw tempCatch;

		U32 iter = 0, groupOfst = 0, groupSize = 0;
		bool completed = false;

		// fill catch with first object in array
		currentCatch.objIndex = 0;
		if (Objects[0]->getTexture()) {
			currentCatch.lastTexId = Objects[0]->getTexture()->getGLID();
		} else {
			currentCatch.lastTexId = 0;
		}
		if (Objects[0]->getShader())
			currentCatch.lastShdId = Objects[0]->getShader()->getGLID();
		currentCatch.lastGeo = Objects[0]->getGeoType();

		_kvao.bind();

		// compare objects
		while (!completed) {

			// iterate over all objects and draw same objects with one draw call
			for (iter = iter; iter < Objects.size(); iter++) {
				tempCatch.objIndex = iter;
				if (Objects[iter]->getTexture()) {
					tempCatch.lastTexId = Objects[iter]->getTexture()->getGLID();
				} else {
					tempCatch.lastTexId = 0;
				}
				if (Objects[iter]->getShader())
					tempCatch.lastShdId = Objects[iter]->getShader()->getGLID();
				tempCatch.lastGeo = Objects[iter]->getGeoType();
				if (tempCatch == currentCatch) {
					// indexed
					if (ISize > 0) {
						groupSize += Objects[iter]->getIndexSize();

					// array
					} else {
						groupSize += Objects[iter]->getVertexSize();
					}
					continue;
				}
				break;
			}

			// draw same object(s) in group
			// bind shader
			if (Objects[currentCatch.objIndex]->getShader())
				Objects[currentCatch.objIndex]->getShader()->bind();

			// bind texture
			if (Objects[currentCatch.objIndex]->getTexture()) {
				Objects[currentCatch.objIndex]->getTexture()->bind();
			} else {
				KTexture::unbindTexture();
			}

			// draw objects
			if (ISize > 0) {
				// indexed
				KRender::draw(groupSize, (U16 *)(sizeof(U16)* groupOfst), currentCatch.lastGeo);
			} else {
				// array
				KRender::draw(0, groupSize, currentCatch.lastGeo);
			}

			// reset catch
			currentCatch = tempCatch;
			groupOfst += groupSize;
			groupSize = 0;

			// draw completed
			if (iter >= Objects.size())
				completed = true;
		}
	}

	void KBatch::_updateInd(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KBatch *clObject = (KBatch *)Sender;
		const KBatchObject **obj = (const KBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		U16 *ind = (U16 *)Data;
		const KBatchObject *otmp;
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

	void KBatch::_updateVer(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KBatch *clObject = (KBatch *)Sender;
		const KBatchObject **obj = (const KBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		const KTransform *pmat;
		const KTransform *mmat;
		KVertex *ver = (KVertex *)Data;
		const KBatchObject *otmp;
		const KVertex *vtmp;

		// projection matrix
		pmat = clObject->getCamera().getTransform();

		U32 ofst = 0;
		for (U32 i = 0; i < size; i++){
			otmp = obj[i];
			mmat = &otmp->getModelViewTransform();
			for (U32 j = 0; j < otmp->getVertexSize(); j++){
				// reverse render
				if (otmp->getReverseRender()) {
					vtmp = &otmp->getVertex()[(otmp->getVertexSize() - 1) - j];
				} else {
					vtmp = &otmp->getVertex()[j];
				}

				// multiply model-matrix (object) with projection-matrix (camera)
				// then update position
				if (otmp->getRelativeTransform()) {
					ver[ofst + j].pos = pmat->transformPoint(mmat->transformPoint(vtmp->pos));
				} else {
					ver[ofst + j].pos = pmat->transformPoint(vtmp->pos);
				}

				// update uv
				ver[ofst + j].uv = vtmp->uv;

				// update color
				ver[ofst + j].color = vtmp->color;
			}

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
	}

	void KBatch::_updatePar(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KBatch *clObject = (KBatch *)Sender;
		const KBatchObject **obj = (const KBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KPointSprite *par = (KPointSprite *)Data;
		const KBatchObject *otmp;
		const KPointSprite *vtmp;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			for (U32 j = 0; j < otmp->getPointSize(); j++) {
				// reverse render
				if (otmp->getReverseRender()) {
					vtmp = &otmp->getPoint()[(otmp->getPointSize() - 1) - j];
				} else {
					vtmp = &otmp->getPoint()[j];
				}

				// update point
				par[ofst + j] = *vtmp;
			}

			// move offset to next object
			ofst += otmp->getPointSize();
		}
	}
}