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
#include "Kite/Core/graphic/kframebuffer.h"
#include "src/Kite/Core/graphic/glcall.h"

namespace Kite{
    U32 KFrameBuffer::_klastBufId = 0;
    KFrameBuffer::KFrameBuffer():
		KCoreInstance(KCI_FBO),
        _kbufId(0)
    {
        // generate framebuffer
        DGL_CALL(glGenFramebuffers(1, &_kbufId));
    }

    KFrameBuffer::~KFrameBuffer(){
        DGL_CALL(glDeleteFramebuffers(1, &_kbufId));
        if (_klastBufId == _kbufId){
            _klastBufId = 0;
        }
    }

    void KFrameBuffer::attachTexture(const KTexture *Texture){
		// save currently bound buffer then bind our buffer temporary
		bind();

        // attach the texture to FBO color attachment point
        DGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                        GL_TEXTURE_2D, Texture->getGLID(), 0));

        // check status
        KDEBUG_ASSERT_T(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

    void KFrameBuffer::bind(){
        if (_klastBufId != _kbufId){
            DGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _kbufId));
            _klastBufId = _kbufId;
        }
    }

    void KFrameBuffer::unbind(){
        if (_klastBufId == _kbufId){
            DGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            _klastBufId = 0;
        }
    }

    void KFrameBuffer::unbindFrameBuffer(){
        DGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        _klastBufId = 0;
    }

	U64 KFrameBuffer::getInstanceSize() const{
		return sizeof(KFrameBuffer);
	}
}
