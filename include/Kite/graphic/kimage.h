#ifndef KIMAGE_H
#define KIMAGE_H

#include "Kite/system/knoncopyable.h"
#include "Kite/system/ksystemdef.h"
#include "Kite/system/kvector2.h"
#include "Kite/graphic/kgraphicstructs.h"
#include <string>
#include <vector>

namespace Kite{
    class KITE_FUNC_EXPORT KImage{
    public:
        KImage();
        ~KImage();

        void create(U32 Width, U32 Height, const KColor &Color);
        void create(U32 Width, U32 Height, const U8 *Pixels);

        bool load(const std::string &FileName); // Load from file
        bool load(const void *Data, std::size_t Size); // Load from memory
        bool save(const std::string &FileName);
        void makeColorMask(const KColor& Color, U8 Alpha = 0);
        void flipH();
        void flipV();

        void setPixel(KVector2U32 Position, const KColor &Color);
        KColor getPixel(KVector2U32 Position) const;
        inline KVector2U32 getSize() const {return _ksize;}
        inline const U8 *getPixelsData() const {return &_kpixels[0];}
    private:
        std::vector<U8> _kpixels;
        KVector2U32 _ksize;
    };
}

#endif // KIMAGE_H
