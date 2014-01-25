#ifndef KGLRENDER_H
#define KGLRENDER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/graphic/kgraphictypes.h"
#include <cstring>

namespace Kite{
    class KITE_FUNC_EXPORT KGLRender : KNonCopyable{
    public:
        KGLRender();
        virtual ~KGLRender();

        virtual void pushStates(); // re-call initialize.
        virtual void popStates();

        virtual bool initialize() = 0;
        virtual std::string getRendererName() = 0;
        virtual KRendererFlags getRendererFlag() = 0;

    protected:
        static bool init(); // init glew
    };
}

#endif // KGLRENDER_H
