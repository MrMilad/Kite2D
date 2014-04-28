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
#include "Kite/Core/math/kvector3.h"
#include "kite/Assist/graphic/kcamera.h"

namespace Kite{
    KBatch::KBatch(const std::vector<KQuad> &Quads):
        _kquads(&Quads),
		_kvboIndx(KBT_INDEX),
		_kvboXY(KBT_VERTEX),
		_kvboUVC(KBT_VERTEX)
    {
        // using KVBPack4 for uv and color (static) buffer
        KVBPack4 uvc[4];
        std::vector<KVBPack4> uvcArray;
		uvcArray.reserve(_kquads->size() * 4);

        // using KVBPack5 for position (stream) buffer
		KVBPack5 pos[4];
        std::vector<KVBPack5> posArray;
        posArray.reserve(_kquads->size() * 4);

		// initialize index
		U16 *index = new U16[_kquads->size() * 6];
		for (U32 i = 0; i < _kquads->size(); i++){
			U16 andc = i * 6;
			U16 value = i * 4;
			index[andc] = value;
			index[++andc] = value + 1;
			index[++andc] = value + 2;
			index[++andc] = value + 2;
			index[++andc] = value + 1;
			index[++andc] = value + 3;
		}

        // initialize position and uv/color arrays
        for (U32 i = 0; i < _kquads->size(); i++){
            const KQuad *temp = &_kquads->at(i);

            // buttom left
			// position
            pos[0].x = temp->getQuadAttribute()->bottomLeft.x;
            pos[0].y = temp->getQuadAttribute()->bottomLeft.y;
			//uv
            uvc[0].u = temp->getQuadAttribute()->bottomLeftUV.x;
            uvc[0].v = temp->getQuadAttribute()->topRightUV.y;

            // top left
			// position
            pos[1].x = temp->getQuadAttribute()->bottomLeft.x;
            pos[1].y = temp->getQuadAttribute()->topRight.y;
			//uv
            uvc[1].u = temp->getQuadAttribute()->bottomLeftUV.x;
            uvc[1].v = temp->getQuadAttribute()->bottomLeftUV.y;

			// buttom right
			// position
            pos[2].x = temp->getQuadAttribute()->topRight.x;
            pos[2].y = temp->getQuadAttribute()->bottomLeft.y;
			//uv
            uvc[2].u = temp->getQuadAttribute()->topRightUV.x;
            uvc[2].v = temp->getQuadAttribute()->topRightUV.y;

            // top right
			// position
            pos[3].x = temp->getQuadAttribute()->topRight.x;
            pos[3].y = temp->getQuadAttribute()->topRight.y;
			//uv
            uvc[3].u = temp->getQuadAttribute()->topRightUV.x;
            uvc[3].v = temp->getQuadAttribute()->bottomLeftUV.y;

            // color
            uvc[0].r = uvc[1].r = uvc[2].r = uvc[3].r = temp->getQuadAttribute()->color.r / 255.0f;
            uvc[0].g = uvc[1].g = uvc[2].g = uvc[3].g = temp->getQuadAttribute()->color.g / 255.0f;
            uvc[0].b = uvc[1].b = uvc[2].b = uvc[3].b = temp->getQuadAttribute()->color.b / 255.0f;
            uvc[0].a = uvc[1].a = uvc[2].a = uvc[3].a = temp->getQuadAttribute()->color.a / 255.0f;

            // copy position data to position array
            posArray.insert(posArray.end(), pos, pos + 4);

            // copy uv and color data to uv/color array
            uvcArray.insert(uvcArray.end(), uvc, uvc + 4);
        }

        // bind vao then inite buffers
        _kvao.bind();

		// index buffer
		_kvboIndx.bind();
		_kvboIndx.fill(index, sizeof(U16)* _kquads->size() * 6, KVB_STATIC);
		delete[] index;

        // position buffer
        _kvboXY.bind();
		_kvboXY.fill(&posArray[0], sizeof(KVBPack5)* posArray.size(), KVB_STREAM);
        _kvao.enableAttribute(0);
        _kvao.setAttribute(0, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVBPack5), KBUFFER_OFFSET(0));
		_kvboXY.setUpdateHandle(_update);

        // uv/color buffer
        _kvboUVC.bind();
		_kvboUVC.fill(&uvcArray[0], sizeof(KVBPack4)* uvcArray.size(), KVB_STATIC);
		// uv
		_kvao.enableAttribute(1);
		_kvao.setAttribute(1, KAC_2COMPONENT, KAT_FLOAT, false, sizeof(KVBPack4), KBUFFER_OFFSET(0));
        // rgba
        _kvao.enableAttribute(2);
		_kvao.setAttribute(2, KAC_4COMPONENT, KAT_FLOAT, false, sizeof(KVBPack4), KBUFFER_OFFSET(sizeof(F32)* 2));

        // finish. unbind vao.
        _kvao.unbindVertexArray();
    }

    void KBatch::update(){
        // update position buffer
		update(0, _kquads->size());
    }

	void KBatch::update(U32 FirstIndex, U32 Size){
		KDEBUG_ASSERT_T(FirstIndex + Size <= _kquads->size());
		// update position buffer
		_kvboXY.update(FirstIndex * sizeof(KVBPack5) * 4, Size * sizeof(KVBPack5)* 4, false, (void *)this);
	}

    void KBatch::draw(KRender &Renderer){
		draw(Renderer, 0, _kquads->size());
    }

	void KBatch::draw(KRender &Renderer, U32 FirstIndex, U32 Size){
		if (_kvisible){
			// bind shader
			if (_kshader)
				_kshader->bind();

			// bind texture
			if (_ktexture)
				_ktexture->bind();

			// bind our vao
			_kvao.bind();

			KDEBUG_ASSERT_T(FirstIndex + Size <= _kquads->size());
			Renderer.draw(Size * 6, (U16 *)(sizeof(U16) * 6 * FirstIndex), KGP_TRIANGLES);
		}
	}

    void KBatch::_update(void *Data, U32 Offset, U32 DataSize, void *Sender){
        KBatch *obj = (KBatch *)Sender;
		const F32 *mmat = 0;
		const F32 *pmat = 0;
		KVBPack5 *pos = (KVBPack5 *)Data;
		KVector3F32 vec(0,0,1);
		U32 pind;

		// projection matrix
		pmat = obj->getCamera().getMatrix().getArray();

        // update data
		U32 index = Offset / sizeof(KVBPack5) / 4;
		U32 size = DataSize / sizeof(KVBPack5) / 4;
		for (U32 i = 0; i < size; i++){
			const KQuadAttrib *temp = obj->_kquads->at(index + i).getQuadAttribute();
			// model-view matrix
			mmat = obj->_kquads->at(index + i).getMatrix().getArray();

			// pos index
			pind = i * 4;

			// vertex 1
			vec.x = temp->bottomLeft.x;
			vec.y = temp->bottomLeft.y;
            vec = vec * mmat;
			vec = vec * pmat;
            pos[pind].x = vec.x;
            pos[pind].y = vec.y;

			// vertex 2
			vec.x = temp->bottomLeft.x;
			vec.y = temp->topRight.y;
			vec = vec * mmat;
			vec = vec * pmat;
            pos[pind + 1].x = vec.x;
            pos[pind + 1].y = vec.y;

			// vertex 3
			vec.x = temp->topRight.x;
			vec.y = temp->bottomLeft.y;
			vec = vec * mmat;
			vec = vec * pmat;
            pos[pind + 2].x = vec.x;
            pos[pind + 2].y = vec.y;

			// vertex 4
			vec.x = temp->topRight.x;
			vec.y = temp->topRight.y;
			vec = vec * mmat;
			vec = vec * pmat;
            pos[pind + 3].x = vec.x;
            pos[pind + 3].y = vec.y;
        }
    }
}
