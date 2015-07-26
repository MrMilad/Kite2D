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
#include "Kite/Assist/graphic/karraybatch.h"
#include "Kite/Core/graphic/krender.h"
#include "Kite/Core/graphic/kgraphicdef.h"

namespace Kite{
	const KCamera KArrayBatch::_kdefcam;
	KArrayBatch::KArrayBatch(U32 VertexSize, const KBatchConfig Config, bool PointSprite) :
		_kcam(&_kdefcam),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_kvboPnt(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(VertexSize),
		_kpstride(PointSprite)
	{
		KDEBUG_ASSERT_T(VertexSize > 0);

		// initialize vertex
		std::vector<KVector2F32> ve(_kvsize, KVector2F32(0, 0));

		// initialize color
		std::vector<KColor> co(_kvsize, KColor());

		// initialize particle
		std::vector<KPointSprite> par;
		if (_kpstride) {
			par.resize(VertexSize);
		}

		// bind vao then inite buffers
		_kvao.bind();

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

		// particle buffer
		if (_kpstride) {
			_kvboPnt.bind();
			_kvboPnt.fill(&par[0], sizeof(KPointSprite)* VertexSize, _kconfig.point);
			// point size
			_kvao.enableAttribute(3);
			_kvao.setAttribute(3, KAC_1COMPONENT, KAT_FLOAT, false, sizeof(F32), KBUFFER_OFFSET(0));
			// texture size
			_kvao.enableAttribute(4);
			_kvao.setAttribute(4, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(sizeof(F32)));
			_kvboPnt.setUpdateHandle(_updateCol);
		}

		// finish. unbind vao.
		_kvao.unbindVertexArray();
	}

	KArrayBatch::KArrayBatch(const std::vector<KArrayBatchObject *> &Objects,
							 const KBatchConfig Config, bool PointSprite) :
		_kcam(&_kdefcam),
		_kvboXY(KBT_VERTEX),
		_kvboUV(KBT_VERTEX),
		_kvboCol(KBT_VERTEX),
		_kvboPnt(KBT_VERTEX),
		_kconfig(Config),
		_kvsize(0),
		_kpstride(PointSprite)
    {

		KDEBUG_ASSERT_T(!Objects.empty());

		// store vertex and index offset of all objects
		for (U32 i = 0; i < Objects.size(); i++) {
			_kvsize += Objects[i]->getVertexSize();
		}

		KDEBUG_ASSERT_T(_kvsize > 0);

		_kobj.reserve(Objects.size());

		// initialize vertex
		std::vector<KVector2F32> ve(_kvsize, KVector2F32(0, 0));

		// initialize color
		std::vector<KColor> co(_kvsize, KColor());

		// initialize particle
		std::vector<KPointSprite> par;
		if (_kpstride) {
			par.resize(_kvsize, KPointSprite());
		}

		// bind vao then inite buffers
		_kvao.bind();

		// position buffer
		_kvboXY.bind();
		_kvboXY.fill(&ve[0], sizeof(KVector2F32)* _kvsize, _kconfig.position);
		_kvao.enableAttribute(0);
		_kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboXY.setUpdateHandle(_updatePos);

		// uv buffer
		_kvboUV.bind();
		_kvboUV.fill(&ve[0], sizeof(KVector2F32)* _kvsize, _kconfig.uv);
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVector2F32), KBUFFER_OFFSET(0));
		_kvboUV.setUpdateHandle(_updateUV);

		// color buffer
		_kvboCol.bind();
		_kvboCol.fill(&co[0], sizeof(KColor)* _kvsize, _kconfig.color);
		_kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KColor), KBUFFER_OFFSET(0));
		_kvboCol.setUpdateHandle(_updateCol);

		// point sprite buffer
		if (_kpstride) {
			_kvboPnt.bind();
			_kvboPnt.fill(&par[0], sizeof(KPointSprite)* _kvsize, _kconfig.point);
			// point sprite
			_kvao.enableAttribute(3);
			_kvao.setAttribute(3, KAC_3COMPONENT, KAT_FLOAT, false, sizeof(KPointSprite), KBUFFER_OFFSET(0));
			_kvboPnt.setUpdateHandle(_updatePar);
		}


		// finish. unbind vao.
		_kvao.unbindVertexArray();
    }

	void KArrayBatch::draw(const KArrayBatchObject *Object, const KBatchUpdate &Update) {
		if (!Object)
			return;

		if (Object->getVertexSize() > _kvsize) {
			KDEBUG_PRINT("object size is greater than buffer size.");
			return;
		}

		if (!Object->getVisible())
			return;

		// update vertex attributes
		_ksender.arraySize = 1;
		_ksender.firstObject = (const void *)&Object;
		if (Update.position) {
			_kvboXY.update(0, sizeof(KVector2F32) * Object->getVertexSize(), false, (void *)this);
		}
		if (Update.uv) {
			_kvboUV.update(0, sizeof(KVector2F32) * Object->getVertexSize(), false, (void *)this);
		}
		if (Update.color) {
			_kvboCol.update(0, sizeof(KColor) * Object->getVertexSize(), false, (void *)this);
		}
		if (_kpstride && Update.particle) {
			_kvboPnt.update(0, sizeof(KPointSprite) * Object->getPointSize(), false, (void *)this);
		}

		_kvao.bind();

		// bind shader
		if (Object->getShader())
			Object->getShader()->bind();

		// bind texture
		if (Object->getTexture())
			Object->getTexture()->bind();

		// draw objects
		KRender::draw(0, Object->getVertexSize(), Object->getGeoType());
	}

	void KArrayBatch::draw(const std::vector<KArrayBatchObject *> &Objects, const KBatchUpdate &Update) {
		if (Objects.empty())
			return;

		bool needDraw = true;
		U32 counter = 0;


		// update buffers and draw
		while (needDraw) {

			U32 vsize = 0;
			U32 firstInList = counter;
			_kobj.clear();

			for (counter; counter < Objects.size(); counter++) {

				if (!Objects.at(counter)->getVisible())
					continue;

				if ((vsize + Objects[counter]->getVertexSize()) <= _kvsize) {
					vsize += Objects[counter]->getVertexSize();
					_kobj.push_back(Objects[counter]);

				} else {
					break;
				}
			}

			if (vsize == 0) {
				//KDEBUG_PRINT("object size is greater than buffer size.");
				return;
			}

			// draw completed
			if (counter >= Objects.size())
				needDraw = false;

			_draw(_kobj, vsize, Update);
		}
	}

	void KArrayBatch::_draw(const std::vector<const KArrayBatchObject *> &Objects, U32 VSize, const KBatchUpdate &Update) {
		// update vertex attributes
		_ksender.arraySize = Objects.size();
		_ksender.firstObject = (const void *)&Objects[0];
		if (Update.position) {
			_kvboXY.update(0, sizeof(KVector2F32) * VSize, false, (void *)this);
		}
		if (Update.uv) {
			_kvboUV.update(0, sizeof(KVector2F32) * VSize, false, (void *)this);
		}
		if (Update.color) {
			_kvboCol.update(0, sizeof(KColor) * VSize, false, (void *)this);
		}
		if (_kpstride && Update.particle) {
			_kvboPnt.update(0, sizeof(KPointSprite) * VSize, false, (void *)this);
		}

		// catch objects state (shader, texture, ...)
		// and render same object with single draw call
		Internal::KCatchDraw currentCatch;
		Internal::KCatchDraw tempCatch;

		U32 iter = 0, groupSize = 0;
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
				if (Objects[iter]->getTexture()) {
					tempCatch.lastTexId = Objects[iter]->getTexture()->getGLID();
				} else {
					tempCatch.lastTexId = 0;
				}
				if (Objects[iter]->getShader())
					tempCatch.lastShdId = Objects[iter]->getShader()->getGLID();
				tempCatch.lastGeo = Objects[iter]->getGeoType();
				if (tempCatch == currentCatch) {
					groupSize += Objects[iter]->getVertexSize();
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
			KRender::draw(0, groupSize, currentCatch.lastGeo);

			// reset catch
			currentCatch = tempCatch;
			groupSize = 0;

			// draw completed
			if (iter >= Objects.size())
				completed = true;
		}
	}

    void KArrayBatch::_updatePos(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KArrayBatch *clObject = (KArrayBatch *)Sender;
		const KArrayBatchObject **obj = (const KArrayBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		const KTransform *pmat;
		const KTransform *mmat;
		KVector2F32 *pos = (KVector2F32 *)Data;
		const KArrayBatchObject *otmp;
		const KVertex *vtmp;

		// projection matrix
		pmat = clObject->getCamera().getTransform();

		U32 ofst = 0;
		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			mmat = &otmp->getModelViewTransform();
			for (U32 j = 0; j < otmp->getVertexSize(); j++) {
				vtmp = &otmp->getVertex()[j];

				// multiply model-matrix (object) with projection-matrix (camera)
				// then update position
				if (otmp->getRelativeTransform()) {
					pos[ofst + j] = pmat->transformPoint(mmat->transformPoint(vtmp->pos));
				} else {
					pos[ofst + j] = pmat->transformPoint(vtmp->pos);
				}
			}

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
    }

	void KArrayBatch::_updateUV(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KArrayBatch *clObject = (KArrayBatch *)Sender;
		const KArrayBatchObject **obj = (const KArrayBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KVector2F32 *uv = (KVector2F32 *)Data;
		const KArrayBatchObject *otmp;
		const KVertex *vtmp;

		U32 ofst = 0;
		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			for (U32 j = 0; j < otmp->getVertexSize(); j++) {
				vtmp = &otmp->getVertex()[j];

				// update uv
				uv[ofst + j] = vtmp->uv;
			}

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
	}

	void KArrayBatch::_updateCol(void *Data, U32 Offset, U32 DataSize, void *Sender){
		KArrayBatch *clObject = (KArrayBatch *)Sender;
		const KArrayBatchObject **obj = (const KArrayBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KColor *col = (KColor *)Data;
		const KArrayBatchObject *otmp;
		const KVertex *vtmp;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			for (U32 j = 0; j < otmp->getVertexSize(); j++) {
				vtmp = &otmp->getVertex()[j];

				// update color
				col[ofst + j] = vtmp->color;
			}

			// move offset to next object
			ofst += otmp->getVertexSize();
		}
	}

	void KArrayBatch::_updatePar(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KArrayBatch *clObject = (KArrayBatch *)Sender;
		const KArrayBatchObject **obj = (const KArrayBatchObject **)clObject->_ksender.firstObject;
		U32 size = clObject->_ksender.arraySize;
		KPointSprite *par = (KPointSprite *)Data;
		const KArrayBatchObject *otmp;
		const KPointSprite *vtmp;
		U32 ofst = 0;

		for (U32 i = 0; i < size; i++) {
			otmp = obj[i];
			for (U32 j = 0; j < otmp->getPointSize(); j++) {
				vtmp = &otmp->getPoint()[j];

				// update point
				par[ofst + j] = *vtmp;
			}

			// move offset to next object
			ofst += otmp->getPointSize();
		}
	}
}
