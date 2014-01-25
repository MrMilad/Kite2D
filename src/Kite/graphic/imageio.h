#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "Kite/system/knoncopyable.h"
#include "Kite/system/kvector2.h"
#include <string>
#include <vector>

namespace Kite{
namespace Internal{
    class ImageIO : KNonCopyable{
    public:
        static ImageIO *CreateInstance();
        static void DestroyInstance();

        bool readFromFile(const std::string &FileName, std::vector<U8> &Pixels, KVector2U32 &Size);
        bool readFromMemory(const void *Data, std::size_t DataSize,
                            std::vector<U8> &Pixels, KVector2U32 &Size);

        bool writeToFile(const std::string &FileName, const std::vector<U8> &Pixels,
                         const KVector2U32& Size);
    private:
        ImageIO();
        ~ImageIO();

        // Convert a string to lower case
        std::string toLower(std::string str);
        static ImageIO *_kinstance;
    };
}
}

#endif // IMAGEIO_H
