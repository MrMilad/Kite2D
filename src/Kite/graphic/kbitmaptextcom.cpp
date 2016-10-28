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
#include "Kite/graphic/kbitmaptextcom.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/meta/kmetamanager.h"
#include "Kite/meta/kmetaclass.h"
#include "Kite/meta/kmetatypes.h"
#include "Kite/serialization/types/kstdvector.h"
#include "Kite/serialization/types/kstdstring.h"
#include <luaintf/LuaIntf.h>
#include <ctype.h>

namespace Kite{
	KBitmapTextCom::KBitmapTextCom(const std::string &Name) :
		KComponent(Name),
		_kisVisible(true),
		_ktindex(0),
		_kwidth(0),
		_kheight(0),
		_kmid(0),
		_kspace(15),
		_kpgraph(30),
		_kindexsize(0),
		_katarray(nullptr),
		_kshprog(nullptr),
		_kneedRedraw(true),
		_kvertex(40)
	{
		addDependency(CTypes::RenderInstance);
	}

	void KBitmapTextCom::attached(KEntity *Owner) {
		auto renderable = (KRenderCom *)Owner->getComponent(CTypes::RenderInstance);
		renderable->setRenderable(getType());
	}

	void KBitmapTextCom::deattached(KEntity *Owner) {}

	RecieveTypes KBitmapTextCom::onMessage(KMessage *Message, MessageScope Scope) {
		return RecieveTypes::IGNORED;
	}

	bool KBitmapTextCom::updateRes() {
		if (!getResNeedUpdate()) {
			return true;
		}

		// load resources
		if (getRMan()) {
			_kshprog = (KShaderProgram *)getRMan()->get(_kshprogName.str);
			_katarray = (KAtlasTextureArray *)getRMan()->get(_ktextureArrayName.str);
			if (_kneedRedraw) {
				setText(_ktext);
			}

			resUpdated();
			return true;
		}

		return false;
	}

	void KBitmapTextCom::setText(const std::string &Text) {
		std::string digit;
		_ktext = Text;
		_kvertex.clear();
		_kindexsize = 0;
		_kwidth = 0;
		_kheight = 0;
		if (_katarray) {
			KColor col;
			bool fsharp = false;
			F32 width = 0, height = 0;
			auto atex = _katarray->getItem(_ktindex);
			if (atex) {
				if (atex->getItemsSize() < (_kmarker.atlasID + _kmarker.lenght)) {
					KD_PRINT("glyph marker is out of range");
					return;
				}
				for (auto it = Text.begin(); it != Text.end(); ++it) {
					// parse color index command
					if ((*it) == '#' && (it + 1) != Text.end()) {

						// move to next char
						++it;

						// we will ignore double sharp
						if ((*it) != '#' && (it + 1) != Text.end()) {
							digit.clear();
							// retrieve index
							for (it; it != Text.end(); ++it) {

								// reach end of command
								if ((*it) == '>') {

									// change current color
									I32 cindex = std::stoi(digit);
									if (cindex >= 0 && cindex < _kcolors.size()) {
										col = _kcolors[cindex];
									}
									++it;
									break;
								}

								// non digit character
								if (!isdigit((*it))) break;

								// catch number
								digit.push_back((*it));
							}

							// reach end of text
							if (it == Text.end()) break;
						}
					}

					// next paragraph
					if ((*it) == '\n') {
						width = 0.0f;
						height += _kpgraph;

					} else if ((*it) == ' ') {
						width += _kspace;

					} else if ((*it) == '\0') {
						continue;

					} else if ((*it) == '\t') {
						width += (_kspace * 3);

					} else if ((*it) >= _kmarker.character && (*it) <= (_kmarker.character + _kmarker.lenght)) {
						KGLVertex temp;
						temp.r = col.getR(); temp.g = col.getG(); temp.b = col.getB(); temp.a = col.getA();

						auto atemp = atex->getItem(((*it) - _kmarker.character) + _kmarker.atlasID);
						temp.pos = KVector2F32(width, height);
						temp.uv = KVector2F32(atemp->blu, atemp->trv);
						_kvertex.push_back(temp);

						// vertex 1
						temp.pos = KVector2F32(width, atemp->height + height);
						temp.uv = KVector2F32(atemp->blu, atemp->blv);
						_kvertex.push_back(temp);

						// vertex 2
						temp.pos = KVector2F32(width + atemp->width, height);
						temp.uv = KVector2F32(atemp->tru, atemp->trv);
						_kvertex.push_back(temp);

						// vertex 3
						temp.pos = KVector2F32(width + atemp->width, atemp->height + height);
						temp.uv = KVector2F32(atemp->tru, atemp->blv);
						_kvertex.push_back(temp);

						width += atemp->width + _kmid;
						_kindexsize += 6;
					}

					_kwidth = width;
					_kheight = height + atex->getItem(_kmarker.atlasID)->height;
				}
			}

			_kneedRedraw = false;
		}
	}

	void KBitmapTextCom::setParagraphSize(U32 Size) {
		if (_kpgraph != Size) {
			_kpgraph = Size;
			_kneedRedraw = true;
		}
	}

	void KBitmapTextCom::setStrideSize(U32 Size) {
		if (_kmid != Size) {
			_kmid = Size;
			_kneedRedraw = true;
		}
	}

	void KBitmapTextCom::setSpaceSize(U32 Size) {
		if (_kspace != Size) {
			_kspace = Size;
			_kneedRedraw = true;
		}
	}

	void KBitmapTextCom::setShader(const KStringID &ShaderProgram) {
		if (_kshprogName.hash != ShaderProgram.hash) {
			_kshprogName = ShaderProgram;
			resNeedUpdate();
			matNeedUpdate();
		}
	}

	void KBitmapTextCom::setAtlasTextureArraye(const KStringID &TextureArrayName) {
		if (_ktextureArrayName.hash != TextureArrayName.hash) {
			_ktextureArrayName = TextureArrayName;
			resNeedUpdate();
			matNeedUpdate();
		}
	}

	void KBitmapTextCom::setTextureArrayIndex(U16 Index) {
		if (_ktindex != Index) {
			_ktindex = Index;
			_kneedRedraw = true;
		}
	}

	void KBitmapTextCom::setGlyphMarker(const KGlyphMarker &Marker) {
		if (_kmarker != Marker) {
			_kmarker = Marker;
			_kneedRedraw = true;
		}
	}

	U32 KBitmapTextCom::addColor(const KColor &Color) {
		_kcolors.push_back(Color);
		return _kcolors.size() - 1;
	}

	void KBitmapTextCom::clearColors() {
		_kcolors.clear();
		_kneedRedraw = true;
	}

	void KBitmapTextCom::getBoundingRect(KRectF32 &Output) const {
		Output.bottom = 0;
		Output.left = 0;
		Output.top = _kheight;
		Output.right = _kwidth;
	}

	KMETA_KBITMAPTEXTCOM_SOURCE();
}