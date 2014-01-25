#ifndef KGL2DRENDER_H
#define KGL2DRENDER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/graphic/kgraphicdef.h"
#include "Kite/graphic/kglrender.h"
#include "Kite/graphic/kcamera.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kvertexbuffer.h"

namespace Kite{
    class KITE_FUNC_EXPORT KGL2DRender : public KGLRender{
    public:
        KGL2DRender();
        KGL2DRender(const KCamera &Camera);
        ~KGL2DRender();

        virtual bool initialize();

        void clear();
        void drawBuffer(KVertexBuffer &Buffer);
        void drawBuffer(KVertexBuffer &Buffer,
                        U32 FirstIndex, U32 Size,
                        KGeoPrimitiveTypes Primitive);

        void setClearColor(const KColor &Color);
        void setPointsOption(const KPointOption &Option);
        inline void setCamera(const KCamera &Camera) {_kcurrentCam = &Camera;}
        void updateCamera();
//        void setBlendMode(KBlendMode BlendMode);
//        void setTexture(const KTexture *Texture);
//        void setSheader(const KSheader Sheader);

        //void unbindBuffers();

        virtual std::string getRendererName();
        virtual KRendererFlags getRendererFlag();

    private:
        const KCamera _kdefaultCam;
        const KCamera *_kcurrentCam;
        KGeoPrimitiveTypes _kgeoType;
        Kite::Internal::KCatchState _kcatch;
    };
}

#endif // KGL2DRENDER_H
