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
#include "Kite/math/ktransformcom.h"
#include "Kite/graphic/krendersys.h"
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/kgraphicdef.h"
#include "Kite/graphic/krenderable.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/graphic/kcameracom.h"	
#include "src/Kite/graphic/glcall.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	bool KRenderSys::update(F32 Delta, KEntityManager *EManager, KResourceManager *RManager) {
		STATIC_OUT_EDITOR const bool isregist = (EManager->isRegisteredComponent(CTypes::Camera) 
												 && EManager->isRegisteredComponent(CTypes::RenderMaterial));
		
		if (isregist) {
			// sort camera(s) (based depth)
			auto continer = EManager->getComponentStorage<KCameraCom>(CTypes::Camera);
			STATIC_OUT_EDITOR std::vector<std::pair<U32, U32>> sortedIndex(KRENDER_CAMERA_SIZE); // reserve 10 pre-defined camera
			sortedIndex.clear();

			// collect depth of all camera(s)
			for (auto i = 0; i < continer->size(); ++i) {
				sortedIndex.push_back({ i, continer->at(i)._kdepth });
			}

			// sort depth list
			std::sort(sortedIndex.begin(), sortedIndex.end(),
					  [](const std::pair<U32, U32> &left, const std::pair<U32, U32> &right) {
				return left.second < right.second;
			});

			// update all active camera(s) (sorted)
			for (auto sit = sortedIndex.begin(); sit != sortedIndex.end(); ++sit) {
				auto it = &continer->at(sit->first);
				_kupdata.clear();
				auto ehandle = it->getOwnerHandle();
				auto entity = EManager->getEntity(ehandle);
				if (entity->isActive()) {

					// update camera
					if (it->getNeedUpdate()) {
						this->_computeCamera(&(*it));
					}

					// update viewport and scissor
					if (_klastState.lastViewSize != it->_ksize || _klastState.lastViewPos != it->_kposition) {
						DGL_CALL(glViewport(it->_kposition.x, it->_kposition.y, it->_ksize.x, it->_ksize.y));
						_klastState.lastViewSize = it->_ksize;
						_klastState.lastViewPos = it->_kposition;

						DGL_CALL(glScissor(it->_kposition.x, it->_kposition.y, it->_ksize.x, it->_ksize.y));
					}

					// update clear color
					if (it->_kclearCol != _klastState.lastColor) {
						DGL_CALL(glClearColor((GLclampf)(it->_kclearCol.getGLR()),
											  (GLclampf)(it->_kclearCol.getGLG()),
											  (GLclampf)(it->_kclearCol.getGLB()),
											  (GLclampf)(it->_kclearCol.getGLA())));
						_klastState.lastColor = it->_kclearCol;
					}

					// clear scene 
					if (it->_kclearView) {
						DGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
					}

					// update and draw renderables
					STATIC_OUT_EDITOR auto render = EManager->getComponentStorage<KRenderCom>(CTypes::RenderMaterial);
					U32 indSize = 0;
					U32 verSize = 0;

					KRenderCom *material = nullptr;
					STATIC_OUT_EDITOR std::vector<std::pair<KRenderCom *, KRenderable *>> objList(KRENDER_BUFF_SIZE);
					objList.resize(0);

					// inite objects list
					for (auto rit = render->begin(); rit != render->end(); ++rit) {
						auto ent = EManager->getEntity(rit->getOwnerHandle());

						// inite materials
						if (rit->getNeedUpdate()) {
							if (!_initeMaterials(RManager, &(*rit))) {
								KD_FPRINT("cant init material. entity name: %s", ent->getName().c_str());
								return false;
							}
						}

						// catch renderable components
						if (EManager->getEntity(rit->getOwnerHandle())->isActive()) {
							if (auto quadcom = (KQuadCom *)ent->getComponent(CTypes::Quad, "")) {
								if (quadcom->isVisible()) {
									objList.push_back({ &(*rit), quadcom });
								}

								// } else if (catch another renderable ...){
							}
						}
					}
						
					// render objects list
					for (auto oit = objList.begin(); oit != objList.end(); ++oit){
						auto ent = EManager->getEntity(oit->first->getOwnerHandle());
						// inite state
						if (oit == objList.begin()) {
							_checkState(oit->first, oit->second);
						}

						// check material (render shared materials in a single batch)
						if (_checkState(oit->first, oit->second)) {

							// check available buffer size
							if ((indSize + oit->second->getIndexSize()) < KRENDER_IBUFF_SIZE) {

								// compute (parent * child * camera) matrix
								// catch object with its matrix
								_kupdata.matrix.push_back((*it).getMatrix());
								_computeParentsTransform(EManager, ent, &_kupdata.matrix.back());
								_kupdata.objects.push_back(oit->second);
								material = oit->first;

								// increase buffers size
								indSize += oit->second->getIndexSize();
								verSize += oit->second->getVertex()->size();

								// check next object if any
								if (oit != --objList.end()) {
									continue;
								}
							} 
						} else {
							--oit;
						}

						// render 
						if (!_kupdata.objects.empty()) {
							// update vetex
							_kvboVer->update(0, sizeof(Internal::KGLVertex) * verSize, false, (void *)&_kupdata);

							// bind quad vao
							_kvao->bind();

							// bind materials
							material->_kshprogptr->bind();
							if (material->_ktextureptr) {
								if (material->_ktextureptr->getTexture()){
									material->_ktextureptr->getTexture()->bind();
								}
							}

							// draw 
							DGL_CALL(glDrawElements(GL_TRIANGLES, indSize, GL_UNSIGNED_SHORT, (U16 *)0));

							// clear list after render
							_kupdata.clear();
							indSize = 0;
							verSize = 0;
						}
						
					}
				}
			}
		}
		return true;
	}

	bool KRenderSys::inite(void *Data) {
		if (!Internal::initeGLEW()) {
			return false;
		}

		// we dont need depth test (objects are always in a sorted order)
		DGL_CALL(glDisable(GL_DEPTH_TEST));

		// we need scissor for clear only part of the screen (camera viewport)
		DGL_CALL(glEnable(GL_SCISSOR_TEST));

		// enable blend
		DGL_CALL(glEnable(GL_BLEND));
		DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// update lists
		_kupdata.objects.reserve(KRENDER_BUFF_SIZE);
		_kupdata.matrix.reserve(KRENDER_BUFF_SIZE);

		// set color to black (gl default)
		_klastState.lastColor = KColor::fromName(Colors::BLACK);

		// inite vao
		_kvao = new KVertexArray();

		// inite index buffer
		_kvboInd = new KVertexBuffer(BufferTarget::INDEX);
		std::vector<U16> ind(KRENDER_IBUFF_SIZE, 0);
		_initeQuadIndex(&ind);

		// inite vertex buffer
		_kvboVer = new KVertexBuffer(BufferTarget::VERTEX);
		std::vector<Internal::KGLVertex> vert(KRENDER_VBUFF_SIZE, Internal::KGLVertex());

		// inite point
		_kvboPnt = nullptr;
		/*std::vector<KPointSprite> po;
		if (_kpsprite) {
			po.resize(_kvsize, KPointSprite());
			_updatePar(&po[0], 0, 0, this);
		}*/

		// bind vao then inite buffers
		_kvao->bind();

		// fill index buffer
		_kvboInd->bind();
		_kvboInd->fill(&ind[0], sizeof(U16) * KRENDER_IBUFF_SIZE, VBufferType::STREAM);
		//_kvboInd->setUpdateHandle(_updateInd);

		// fill vertex buffer
		_kvboVer->bind();
		_kvboVer->fill(&vert[0], sizeof(Internal::KGLVertex) * KRENDER_VBUFF_SIZE, VBufferType::STREAM);

		// set pos attr
		_kvao->enableAttribute(0);
		_kvao->setAttribute(0, AttributeCount::COMPONENT_2, AttributeType::FLOAT, false, sizeof(Internal::KGLVertex), KBUFFER_OFFSET(0));

		// set uv attr
		_kvao->enableAttribute(1);
		_kvao->setAttribute(1, AttributeCount::COMPONENT_2, AttributeType::FLOAT,
							false, sizeof(Internal::KGLVertex), KBUFFER_OFFSET(sizeof(KVector2F32)));

		// set color attr
		_kvao->enableAttribute(2);
		_kvao->setAttribute(2, AttributeCount::COMPONENT_4, AttributeType::FLOAT,
							false, sizeof(Internal::KGLVertex), KBUFFER_OFFSET(sizeof(F32) * 4));
		_kvboVer->setUpdateHandle(_updateVer);

		// point sprite buffer
		/*if (_kpsprite) {
			_kvboPnt.bind();
			_kvboPnt.fill(&po[0], sizeof(KPointSprite)* _kvsize, (KVertexBufferTypes)_kconfig.point);
			// point sprite
			_kvao.enableAttribute(3);
			_kvao.setAttribute(3, KAC_3COMPONENT, KAT_FLOAT, false, sizeof(KPointSprite), KBUFFER_OFFSET(0));
			_kvboPnt.setUpdateHandle(_updatePar);
		}*/

		// finish. unbind vao.
		_kvao->unbindVertexArray();
		
		setInite(true);
		return true;
	}

	void KRenderSys::destroy() {
		// delete buffers
		delete _kvboVer;
		delete _kvboInd;
		delete _kvboPnt;
		delete _kvao;
		setInite(false);
	}

	bool KRenderSys::_checkState(KRenderCom *Com, KRenderable *Rendeable){
		U32 textId = 0;
		if (Com->_ktextureptr) {
			if (Com->_ktextureptr->getTexture()) {
				textId = Com->_ktextureptr->getTexture()->getGLID();
			}
		}
		if (Com->_kshprogptr->getGLID() != _klastState.lastShdId ||
			Rendeable->getGeoType() != _klastState.lastGeo ||
			textId != _klastState.lastTexId)
		{
			// reset last state
			_klastState.lastShdId = Com->_kshprogptr->getGLID();
			_klastState.lastShdId = Com->_kshprogptr->getGLID();
			_klastState.lastTexId = textId;
			_klastState.lastGeo = Rendeable->getGeoType();
			return false;
		}

		return true;
	}

	bool KRenderSys::_initeMaterials(KResourceManager*RMan, KRenderCom *Com) {
		// fetch resources to the component
		Com->_kshprogptr = (KShaderProgram *)RMan->get(Com->_kshprog.str);

		// all renderables must have shader program
		if (!Com->_kshprogptr) {
			return false;
		}

		if (!Com->_kshprogptr->isInite()) {
			Com->_kshprogptr->bindAttribute(KVATTRIB_XY, "in_pos");
			Com->_kshprogptr->bindAttribute(KVATTRIB_UV, "in_uv");
			Com->_kshprogptr->bindAttribute(KVATTRIB_RGBA, "in_col");

			Com->_kshprogptr->inite();
			Com->_kshprogptr->link();
		}

		if (!Com->_ktexture.str.empty()) {
			Com->_ktextureptr = (KAtlasTexture *)RMan->get(Com->_ktexture.str);

			if (Com->_ktextureptr) {
				if (Com->_ktextureptr->getTexture()) {
					Com->_ktextureptr->getTexture()->inite();
				}
			}
		}

		Com->setNeedUpdate(false);
		return true;
	}

	void KRenderSys::_initeQuadIndex(std::vector<U16> *Buffer) {
		auto trueSize = Buffer->size() / 6;
		for (U32 i = 0; i < trueSize; ++i) {
			auto offset = i * 6;
			auto value = i * 4;
			(*Buffer)[offset] = value;
			(*Buffer)[offset + 1] = value + 1;
			(*Buffer)[offset + 2] = value + 2;
			(*Buffer)[offset + 3] = value + 2;
			(*Buffer)[offset + 4] = value + 1;
			(*Buffer)[offset + 5] = value + 3;
		}
	}

	void KRenderSys::_computeCamera(KCameraCom *Camera) {
		F32 flipy = 1.0f; 
		if (Camera->_kflipy) flipy = -1.0f;
		F32 flipx = 1.0f; 
		if (Camera->_kflipx) flipx = -1.0f;
		// rotation components
		F32 angle = Camera->_krotation * KMATH_PIsub180;
		F32 cosx = (F32)(std::cos(angle));
		F32 sinx = (F32)(std::sin(angle));
		//F32 tx = -(_kviewport.right + _kviewport.left) / (_kviewport.right - _kviewport.left);
		//F32 ty = -(_kviewport.top + _kviewport.bottom) / (_kviewport.top - _kviewport.bottom);
		F32 tx = -Camera->_kcenter.x * cosx - Camera->_kcenter.y * sinx + Camera->_kcenter.x;
		F32 ty = Camera->_kcenter.x * sinx - Camera->_kcenter.y * cosx + Camera->_kcenter.y;

		// projection components
		F32 a = (2.0f * flipx) / (Camera->_ksize.x / Camera->_kzoom);
		F32 b = (2.0f * flipy) / (Camera->_ksize.y / Camera->_kzoom); // -2.f for microsoft windows coordinate system
		F32 c = -a * Camera->_kcenter.x;
		F32 d = -b * Camera->_kcenter.y;

		// rebuild the projection matrix
		Camera->_kmatrix = KMatrix3(a * cosx, a * sinx, a * tx + c,
										  -b * sinx, b * cosx, b * ty + d,
										  0.0f, 0.0f, 1.0f);

		/*_kmatrix = KMatrix3(	a,		0.0f,	tx,
		0.0f,	b,		ty,
		0.0f,	0.0f,	1.0f);*/

		Camera->setNeedUpdate(false);
	}

	void KRenderSys::_computeParentsTransform(KEntityManager *Eman, KEntity *Entity, KMatrix3 *Matrix) {
		auto trcom = (KTransformCom *)Entity->getComponent(CTypes::Transform, "");
		if (Entity->getParentHandle() != Eman->getRoot()) {
			_computeParentsTransform(Eman, Eman->getEntity(Entity->getParentHandle()), Matrix);
		}
		if (trcom) {
			(*Matrix) *= trcom->getMatrix();
		}
	}

	/*void KRenderSys::_updateInd(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		updateData *udata = (updateData *)Sender;
		U16 *ind = (U16 *)Data;
		U32 offset = 0;

		for (auto it = udata->objects.begin(); it < udata->objects.end(); ++it) {
			memcpy(&ind[offset], &(*(*it)->getIndex())[0], sizeof(U16) * (*it)->getIndex()->size());
			offset += (*it)->getIndex()->size();
		}
	}*/

	void KRenderSys::_updateVer(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		updateData *udata = (updateData *)Sender;
		Internal::KGLVertex *ver = (Internal::KGLVertex *)Data;
		U32 matIndex = 0;
		U32 offset = 0;

		const auto cend = udata->objects.cend();
		for (auto it = udata->objects.begin(); it != cend; ++it) {
			U16 vindex = 0;

			// reverse render
			if ((*it)->isReverse()) {
				const auto crend = (*it)->getVertex()->crend();
				for (auto vit = (*it)->getVertex()->crbegin(); vit != crend; ++vit) {
					ver[offset + vindex].pos = KTransform::transformPoint(udata->matrix[matIndex], vit->pos);
					ver[offset + vindex].r = vit->color.r;
					ver[offset + vindex].g = vit->color.g;
					ver[offset + vindex].b = vit->color.b;
					ver[offset + vindex].a = vit->color.a;

					// update uv
					ver[offset + vindex].uv = vit->uv;;

					++vindex;
				}

			// normal render
			} else {
				const auto cend = (*it)->getVertex()->cend();
				for (auto vit = (*it)->getVertex()->cbegin(); vit != cend; ++vit) {
					ver[offset + vindex].pos = KTransform::transformPoint(udata->matrix[matIndex], (*vit).pos);
					ver[offset + vindex].r = (*vit).color.r;
					ver[offset + vindex].g = (*vit).color.g;
					ver[offset + vindex].b = (*vit).color.b;
					ver[offset + vindex].a = (*vit).color.a;

					// update uv
					ver[offset + vindex].uv = vit->uv;

					++vindex;
				}
			}

			offset += (*it)->getVertex()->size();
			++matIndex;
		}
	}

	KMETA_KRENDERSYS_SOURCE();

	/*void KRenderSys::_updatePar(void *Data, U32 Offset, U32 DataSize, void *Sender) {
		KRenderSys *clObject = (KRenderSys *)Sender;
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
	}*/
}