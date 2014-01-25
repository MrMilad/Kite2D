#include "Kite/graphic/kvertexbuffer.h"
#include "Kite/graphic/glcall.h"

namespace Kite{

    U32 KVertexBuffer::_klastBufId = 0;
    KVertexBuffer::KVertexBuffer(KVertexBufferTypes BufferType, const KVertex *VertexArray, U32 ArrayLenght):
        _kbufType(BufferType),
        _kvertexPointer(VertexArray),
        _karrLen(ArrayLenght),
        _kbufId(0),
        _kfilled(false),
        _kupdateHnd(0)
    {}

    KVertexBuffer::~KVertexBuffer(){
        if (_kbufId != 0){
            DGL_CALL(glDeleteBuffers(1, &_kbufId));
            if (_klastBufId == _kbufId){
                _klastBufId = 0;
            }
        }
    }

    void KVertexBuffer::bind(){
        // generate buffer (generate in first call)
        if (_kbufId == 0)
            DGL_CALL(glGenBuffers(1, &_kbufId));

        // bind buffer
        DGL_CALL(glBindBuffer(GL_ARRAY_BUFFER_ARB, _kbufId));

        // fill buffer with vertex data.
        if (!_kfilled){
            // before filling buffer we check our data
            if (_kvertexPointer != 0 && _karrLen > 0){
                int dataHint[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
                DGL_CALL(glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(KVertex)*_karrLen,
                                      (const void *) &_kvertexPointer[0].x, dataHint[_kbufType]));
                _kfilled = true;
            }
        }
        _klastBufId = _kbufId;
    }

    void KVertexBuffer::update(KMapAccessTypes AccessType){
        if (_kupdateHnd){
            int accType[] = {GL_READ_ONLY_ARB, GL_WRITE_ONLY_ARB, GL_READ_WRITE_ARB};
            Internal::GLBindGuard bindGuard;
            if (_klastBufId != 0)// save currently binded buffer
                bindGuard = Internal::GLBindGuard(Internal::BGUARD_BUFFER, _klastBufId);
            bind();
            void *dataPtr = DGL_CALL(glMapBuffer(GL_ARRAY_BUFFER_ARB, accType[AccessType]));
            (*_kupdateHnd)((KVertex *)dataPtr, _karrLen);
            DGL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER_ARB));
        }else{
            KDEBUG_PRINT("update handle not set yet.");
        }
    }

}
