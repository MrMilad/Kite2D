#ifndef KCAMERA_H
#define KCAMERA_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/kvector2.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/ktransform.h"
#include <cmath>

namespace Kite{
    class KITE_FUNC_EXPORT KCamera{
    public:
        KCamera();
        KCamera(const KRectF32 &Viewport);

        inline const KRectF32 &getViewport() const {return _kviewport;}
        inline const KVector2F32 &getCenter() const {return _kcenter;}
        inline F32 getRotation() const {return _krotation;}
        inline F32 getZoom() const {return _kzoom;}

        inline const KTransform &getTransform() const {return _ktransform;}
        KTransform getInverseTransform() const;

        inline void setViewport(const KRectF32 &Viewport) {_kviewport = Viewport;}
        inline void setCenter(const KVector2F32 Center) {_kcenter = Center; _kneedUpdateIn = true;}
        void setRotation(F32 Angle);
        void setZoom(F32 Factor);

        KVector2F32 convertPoint(const KVector2I32 &Point);

    private:
        KRectF32 _kviewport;
        KVector2F32 _kcenter;
        F32 _krotation;
        F32 _kzoom;
        //mutable bool _kneedUpdateTr;
        mutable bool _kneedUpdateIn;
        mutable KTransform _ktransform;
        mutable KTransform _ktransformIn;
    };
}
#endif // KCAMERA_H
