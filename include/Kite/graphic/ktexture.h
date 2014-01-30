#ifndef KTEXTURE_H
#define KTEXTURE_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/system/kvector2.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kimage.h"

namespace Kite{
    class KTexture : public KNonCopyable{
    public:
        KTexture();
        ~KTexture();

        // create texture and load image pixels
        void create(const KImage &Image, KTextureFilterTypes Filter, KTextureWrapTypes Wrap);

        // update whole or piece of texture with image
        // image size + position must equal or smaller than texture size
        void update(const KImage &Image, U32 XPos, U32 YPos);

        // bind the texture and ready for render
        // (handle autimatic by internal render system)
        void bind();

        inline KTextureFilterTypes getFilter() const {return _kfilter;}
        inline KTextureWrapTypes getWrap() const {return _kwrap;}
        inline U32 getID() const {return _ktexId;}

        void setFilter(KTextureFilterTypes Filter);
        void setWrap(KTextureWrapTypes Wrap);

    private:
        //U64 _kuid; // unique texture id (use in state catch)
        U32 _ktexId; // ogl texture name
        KTextureFilterTypes _kfilter; // texture interpolation
        KTextureWrapTypes _kwrap; // texture wrapping
        KVector2U32 _ksize; // size of texture
        static U32 _klastTexId;
    };
}

#endif // KTEXTURE_H

/* Remark
 * always "update" an existing texture is faster than "re-create"
 * update() overload with ImageArea param is slow because pixels data will copy row-by-row into texture
 */
