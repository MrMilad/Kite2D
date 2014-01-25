#ifndef KVERTEXBUFFER_H
#define KVERTEXBUFFER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"

namespace Kite{
    class KITE_FUNC_EXPORT KVertexBuffer : KNonCopyable{
    friend class KGL2DRender;
    public:
        KVertexBuffer(KVertexBufferTypes BufferType, const KVertex *VertexArray, U32 ArrayLenght);
        ~KVertexBuffer();

        inline void setType(KVertexBufferTypes BufferType) {_kbufType = BufferType;}
        inline void setVertexArray(const KVertex *VertexArray, U32 ArrayLenght){_kvertexPointer = VertexArray;
                                                                                _karrLen = ArrayLenght;}
        inline void setUpdateHandle(KCallVBUpdate *UpdateHandle) {_kupdateHnd = UpdateHandle;}

        inline KVertexBufferTypes getType() const {return _kbufType;}
        inline U32 getLength() const {return _karrLen;}
        inline U32 getID() const {return _kbufId;}

        // Bind Buffer (generate and fill buffer in first call)
        void bind();

        // Update Buffer
        void update(KMapAccessTypes AccessType);

    private:
        KVertexBufferTypes _kbufType;
        const KVertex *_kvertexPointer;
        U32 _karrLen;
        U32 _kbufId;
        bool _kfilled;
        KCallVBUpdate *_kupdateHnd;
        static U32 _klastBufId;
    };
}

#endif // KVERTEXBUFFER_H
