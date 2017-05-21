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
#include "Kite/graphic/krendersys.h"
#include "Kite/math/ktransformcom.h"
#include "Kite/engine/kengine.h"
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/kgraphicdef.h"
#include "Kite/graphic/krenderable.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/graphic/kgcullingsys.h"
#include "Kite/graphic/korthomapcom.h"
#include "Kite/graphic/kvertexarray.h"
#include "Kite/graphic/kvertexbuffer.h"
#include "Kite/graphic/kframebuffer.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/graphic/kcameracom.h"	
#include "src/Kite/graphic/glcall.h"
#include <luaintf/LuaIntf.h>

namespace Kite{
	KRenderSys::KRenderSys():
		_kcullSys(nullptr)
	{
		KD_ASSERT(Internal::initeGLEW());

		// frame buffer object will be initialized in initFrameBuffer()
		// we inite it only when a reneder to texture operation need 
		_kfbo = new KFrameBuffer();

		// we dont need depth test (objects are always in a sorted order)
		DGL_CALL(glDisable(GL_DEPTH_TEST));

		// we need scissor for clear only part of the screen (camera viewport)
		DGL_CALL(glEnable(GL_SCISSOR_TEST));

		// enable blend
		DGL_CALL(glEnable(GL_BLEND));
		DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// update lists
		_kupdata.objects.reserve(_kconfig.objectSize);
		_kupdata.matrix.reserve(_kconfig.objectSize);

		// set color to black (gl default)
		_klastState.lastColor = KColor::fromName(Colors::BLACK);

		// inite vao
		_kvao = new KVertexArray();

		// inite fbo in first time
		_kfbo = new KFrameBuffer();

		// inite index buffer
		_kvboInd = new KVertexBuffer(BufferTarget::INDEX);
		std::vector<U16> ind(_kconfig.indexSize, 0);
		_initeQuadIndex(&ind);

		// inite vertex buffer
		_kvboVer = new KVertexBuffer(BufferTarget::VERTEX);
		std::vector<KGLVertex> vert(_kconfig.vertexSize, KGLVertex());

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
		_kvboInd->fill(&ind[0], sizeof(U16) * _kconfig.indexSize, VBufferType::STATIC);
		//_kvboInd->setUpdateHandle(_updateInd);

		// fill vertex buffer
		_kvboVer->bind();
		_kvboVer->fill(&vert[0], sizeof(KGLVertex) * _kconfig.vertexSize, VBufferType::STREAM);

		// set pos attr
		_kvao->enableAttribute(0);
		_kvao->setAttribute(0, AttributeCount::COMPONENT_2, AttributeType::FLOAT, true, sizeof(KGLVertex), KBUFFER_OFFSET(0));

		// set uv attr
		_kvao->enableAttribute(1);
		_kvao->setAttribute(1, AttributeCount::COMPONENT_2, AttributeType::FLOAT,
			true, sizeof(KGLVertex), KBUFFER_OFFSET(sizeof(F32) * 2));

		// set color attr
		_kvao->enableAttribute(2);
		_kvao->setAttribute(2, AttributeCount::COMPONENT_4, AttributeType::UNSIGNED_BYTE,
			false, sizeof(KGLVertex), KBUFFER_OFFSET(sizeof(F32) * 4));

		// set texture array index attr
		_kvao->enableAttribute(3);
		_kvao->setAttribute(3, AttributeCount::COMPONENT_1, AttributeType::UNSIGNED_SHORT,
			false, sizeof(KGLVertex), KBUFFER_OFFSET((sizeof(F32) * 4) + (sizeof(U8) * 4)));

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
	}

	KRenderSys::~KRenderSys() {
		// delete buffers
		delete _kvboVer;
		delete _kvboInd;
		delete _kvboPnt;
		delete _kvao;
		delete _kfbo;
		delete _kcullSys;
	}

	void KRenderSys::reset(KEngine *Engine) {
		_kcullSys = static_cast<KGCullingSys *>(Engine->getSystem(System::GCULLING));
	}

	bool KRenderSys::update(F64 Delta) {
		// update all active camera(s) (sorted)
		for (auto camit = _kcamList.begin(); camit != _kcamList.end(); ++camit) {
			auto cam = (*camit);

			// skip inactive camera
			if (!cam->getOwnerNode()->isActive()) continue;

			_kupdata.clear();

			// check active
			if (entity->isActive()) {
				// check render target (texture or screen)
				if (cam->getRenderTexture()) {
					_kfbo->bind();
					auto tarray = static_cast<KAtlasTextureArray *>(cam->getRenderTexture().get());
					_initeFrameBuffer(tarray, cam->getRenderTextureIndex());
				} else {
					_kfbo->unbind();
					DGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
				}

				// update viewport and scissor
				if (_klastState.lastViewSize != cam->_ksize || _klastState.lastViewPos != cam->_kposition) {
					DGL_CALL(glViewport(cam->_kposition.x, cam->_kposition.y, cam->_ksize.x, cam->_ksize.y));
					_klastState.lastViewSize = cam->_ksize;
					_klastState.lastViewPos = cam->_kposition;

					DGL_CALL(glScissor(cam->_kposition.x, cam->_kposition.y, cam->_ksize.x, cam->_ksize.y));
				}

				// update clear color
				if (cam->_kclearCol != _klastState.lastColor) {
					DGL_CALL(glClearColor((GLclampf)(cam->_kclearCol.getGLR()),
											(GLclampf)(cam->_kclearCol.getGLG()),
											(GLclampf)(cam->_kclearCol.getGLB()),
											(GLclampf)(cam->_kclearCol.getGLA())));
					_klastState.lastColor = cam->_kclearCol;
				}

				// clear scene 
				if (cam->_kclearView) {
					DGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
				}

				U32 indSize = 0;
				U32 verSize = 0;

				KRenderable *material = nullptr;
				ED_STATIC std::vector<std::pair<KEntity *, KRenderable *>> objList;
				objList.reserve(_kconfig.objectSize);

				// update and draw renderables
				// culling is enabled
				if (_kconfig.culling) {
					const U8 filter = (U8)GCullingObjectsFilter::TILE 
						| (U8)GCullingObjectsFilter::DYNAMIC 
						| (U8)GCullingObjectsFilter::STATIC;
					_kcullSys->queryObjects(cam, (GCullingObjectsFilter)filter, EManager, objList);

				} else {
					// culling is disabled
					_fillRenderList(EManager, objList);
				}

				// sort objects
				if (_kconfig.zSorting) {
					// sort using a lambda expression 
					std::sort(objList.begin(), objList.end(), [](const std::pair<KEntity *, KRenderable *> &A,
																const std::pair<KEntity *, KRenderable *> &B) {
						return B.first->getZOrder() < A.first->getZOrder();
					});
				}
				
				// render objects list
				for (auto oit = objList.begin(); oit != objList.end(); ++oit){
					auto ent = oit->first;

					// inite materials
					if (oit->second->getMatNeedUpdate()) {
						if (!_initeMaterials(oit->second)) {
							KD_FPRINT("cant init material. entity name: %s", ent->getName().c_str());
							return false;
						}
					}

					// inite state
					if (oit == objList.begin()) {
						_checkState(oit->second);
					}

					// check material (render shared materials in a single batch)
					if (_checkState(oit->second)) {

						// check available buffer size
						if ((indSize + oit->second->getIndexSize()) < _kconfig.indexSize) {
							// compute (parent * child * camera) matrix
							// catch object with its matrix
							auto trcom = (KTransformCom *)ent->getComponent(CTypes::Transform, "");
							_kupdata.matrix.push_back((*cam).getRatioMatrix(trcom->getRatioIndex()));
							_computeParentsTransform(EManager, ent, &_kupdata.matrix.back());
							_kupdata.objects.push_back(oit->second);
							material = oit->second;

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
						_kvboVer->update(0, sizeof(KGLVertex) * verSize, false, (void *)&_kupdata);

						// bind quad vao
						_kvao->bind();

						// bind materials
						material->getShaderProg()->bind();
						auto atlas = material->getATextureArray();
						if (atlas) {
							if (atlas->isInite()) {
								atlas->bind();
							} else {
								KAtlasTextureArray::unbindTextureArray();
							}
						} else {
							KAtlasTextureArray::unbindTextureArray();
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
		return true;
	}

	void KRenderSys::nodeAdded(KNode *Node) {
		// we store only camera components because we render objects using culling system
		if (Node->hasComponent(Component::CAMERA)) {
			componentAdded(Node->getComponent(Component::CAMERA));
		}
	}

	void KRenderSys::nodeRemoved(KNode *Node) {
		if (Node->hasComponent(Component::CAMERA)) {
			componentRemoved(Node->getComponent(Component::CAMERA));
		}
	}

	void KRenderSys::componentAdded(KComponent *Com) {
		auto cam = static_cast<KCameraCom *>(Com);
		cam->_krindex = _kcamList.size();
		_kcamList.push_back(cam);

		// sort list by camera depth
		std::sort(_kcamList.begin(), _kcamList.end(),
			[](const KCameraCom *left, const KCameraCom *right) {
			return left->getDepth() < right->getDepth();
		});
	}

	void KRenderSys::componentRemoved(KComponent *Com) {
		auto cam = static_cast<KCameraCom *>(Com);

		// swap and remove
		_kcamList.back()->_krindex = cam->_krindex;
		_kcamList[cam->_krindex] = _kcamList.back();
		_kcamList.pop_back();
	}

	KRenderSys::~KRenderSys() {
		// delete buffers
		delete _kvboVer;
		delete _kvboInd;
		delete _kvboPnt;
		delete _kvao;
		delete _kfbo;
		delete _kcullSys;
	}

	bool KRenderSys::_checkState(KRenderable *Rendeable){
		U32 textId = 0;
		auto tarraye = Rendeable->getATextureArray();
		if (tarraye) {
			if (tarraye->isInite()) {
				textId = tarraye->getGLID();
			}
		}

		auto shader = Rendeable->getShaderProg();
		if (shader->getGLID() != _klastState.lastShdId ||
			Rendeable->getGeoType() != _klastState.lastGeo ||
			textId != _klastState.lastTexId)
		{
			// reset last state
			_klastState.lastShdId = shader->getGLID();
			_klastState.lastShdId = shader->getGLID();
			_klastState.lastTexId = textId;
			_klastState.lastGeo = Rendeable->getGeoType();
			return false;
		}

		return true;
	}

	void KRenderSys::_initeFrameBuffer(KAtlasTextureArray *TextureArray, U32 Index) {
		TextureArray->inite();
		TextureArray->bind();

		// attach render texture
		if (_klastState.lastFBOTextId != TextureArray->getGLID() || _klastState.lastFBOTexInd != Index) {
			_kfbo->attachTextureArray(TextureArray, Index);

			GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
			DGL_CALL(glDrawBuffers(1, DrawBuffers));

			_klastState.lastFBOTextId = TextureArray->getGLID();
			_klastState.lastFBOTexInd = Index;
		}
		DGL_CALL(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
		//DGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, TextureArray->getGLID()));
	}

	void KRenderSys::_fillRenderList(KEntityManager *Eman, std::vector<std::pair<KEntity *, KRenderable *>> &Output) {
		Output.resize(0);
		// culling is disabled
		ED_STATIC auto render = Eman->getComponentStorage<KRenderCom>(CTypes::RenderInstance);

		// inite objects list
		for (auto rit = render->begin(); rit != render->end(); ++rit) {
			auto ent = Eman->getEntity(rit->getOwnerHandle());

			// catch renderable components
			if (ent->isActive()) {
				auto com = ent->getComponent(rit->getRenderable());
				com->updateRes();

				auto renderable = dynamic_cast<KRenderable *>(com);
				if (renderable->isVisible() && renderable->getIndexSize() > 0) {
					Output.push_back({ ent ,renderable });
				}
			}
		}
	}

	bool KRenderSys::_initeMaterials(KRenderable *Com) {
		// all renderables must have shader program
		auto shader = Com->getShaderProg();
		if (!shader) {
			return false;
		}
		
		if (!shader->isInite()) {
			shader->bindAttribute(KVATTRIB_XY, "in_pos");
			shader->bindAttribute(KVATTRIB_UV, "in_uv");
			shader->bindAttribute(KVATTRIB_RGBA, "in_col");
			shader->bindAttribute(KVATTRIB_ARRAYINDEX, "in_tindex");

			shader->inite();
			shader->link();
		}

		auto tarraye = Com->getATextureArray();
		if (tarraye) {
			tarraye->inite();
		}
		Com->_kmatNeedUpdate = false;
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
		KGLVertex *ver = (KGLVertex *)Data;
		U32 matIndex = 0;
		U32 offset = 0;

		const auto cend = udata->objects.cend();
		for (auto it = udata->objects.begin(); it != cend; ++it) {
			U16 vindex = 0;
			
			memcpy(ver + offset, &(*it)->getVertex()->operator[](0), sizeof(KGLVertex) * (*it)->getVertex()->size());
			for (SIZE i = 0; i < (*it)->getVertex()->size(); ++i) {
				(ver + offset)[i].pos = KTransform::transformPoint(udata->matrix[matIndex], (ver + offset)[i].pos);
			}

			offset += (*it)->getVertex()->size();
			++matIndex;
		}
	}

	KRENDERSYS_SOURCE();

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