#ifndef KDRAWABLE_H
#define KDRAWABLE_H

#include "Kite/system/ksystemdef.h"
#include "Kite/graphic/kglrender.h"

namespace Kite{
    class KITE_FUNC_EXPORT KDrawable{
        friend class KGLRender;
    public:
        virtual ~KDrawable();
    protected:
        virtual void draw() = 0;
    };
}

#endif // KDRAWABLE_H
