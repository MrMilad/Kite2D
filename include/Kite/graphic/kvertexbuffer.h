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
        KVertexBuffer();
        ~KVertexBuffer();

        // create buffer
        void create(const KVertex *VertexArray, U32 ArrayLenght, KVertexBufferTypes BufferType);

        // update buffer
        void update(KMapAccessTypes AccessType);

        // bind buffer
        // (handle autimatic by internal render system)
        void bind();

        // set update handle for updating buffer
        inline void setUpdateHandle(KCallVBUpdate *UpdateHandle) {_kupdateHnd = UpdateHandle;}

        inline KVertexBufferTypes getType() const {return _kbufType;}
        inline U32 getLength() const {return _karrLen;}
        inline U32 getID() const {return _kbufId;}

    private:
        KVertexBufferTypes _kbufType;
        U32 _karrLen;
        U32 _kbufId;
        KCallVBUpdate *_kupdateHnd;
        static U32 _klastBufId;
    };
}

#endif // KVERTEXBUFFER_H
