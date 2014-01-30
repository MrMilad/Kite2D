#ifndef KGRAPHICTYPES_H
#define KGRAPHICTYPES_H
namespace Kite{

    enum KRendererFlags{
        KITE2D_FIXED_2D = 0
    };

    enum KBlendMode{
        KBLEND_ALPHA,    // Pixel = Source * Source.a + Dest * (1 - Source.a)
        KBLEND_ADD,      // Pixel = Source + Dest
        KBLEND_MULTIPLY, // Pixel = Source * Dest
        KBLEND_NONE      // Pixel = Source
    };

    enum KVertexBufferTypes{
        KVBUFFER_STATIC = 0,
        KVBUFFER_DYNAMIC,
        KVBUFFER_STREAM
    };

    enum KMapAccessTypes{
        KMAP_ACCESS_READ = 0,
        KMAP_ACCESS_WRITE,
        KMAP_ACCESS_RW
    };

    enum KGeoPrimitiveTypes{
        KGL_POINTS = 0,
        KGL_LINES,
        KGL_LINE_STRIP,
        KGL_LINE_LOOP,
        KGL_TRIANGLES,
        KGL_TRIANGLE_STRIP,
        KGL_TRIANGLE_FAN,
        KGL_QUADS,
        KGL_QUAD_STRIP,
        KGL_POLYGON
    };

    enum KPointDrawTypes{
        KPD_DEFAULT,
        KPD_SMOOTH
    };

    /* Colors */
    enum KColors{
        KCOLORS_BLACK = 0x000000,           KCOLORS_WHITE = 0xFFFFFF,
        KCOLORS_RED = 0xff0000,             KCOLORS_YELLOW = 0xffff00,
        KCOLORS_BLUE = 0x0000ff,            KCOLORS_GREEN = 0x00ff00,
        KCOLORS_CRIMSON = 0xDC143C,         KCOLORS_PINK = 0xFFC0CB,
        KCOLORS_PALEVIOLETRED = 0xDB7093,   KCOLORS_LIMEGREEN = 0x32CD32,
        KCOLORS_LAVENDERBLUSH = 0xFFF0F5,   KCOLORS_HOTPINK = 0xFF69B4,
        KCOLORS_RASPBERRY = 0x872657,       KCOLORS_COBALTGREEN = 0x3D9140,
        KCOLORS_DEEPPINK = 0xFF1493,        KCOLORS_MAROON = 0xFF34B3,
        KCOLORS_VIOLET = 0xEE82EE,          KCOLORS_MEDIUMSEAGREEN = 0x3CB371,
        KCOLORS_DARKVIOLET = 0x9400D3,      KCOLORS_ORCHID = 0xDA70D6,
        KCOLORS_THISTLE = 0xD8BFD8,         KCOLORS_CYAN = 0x00FFFF,
        KCOLORS_PLUM = 0xDDA0DD,            KCOLORS_MAGENTA  = 0xFF00FF,
        KCOLORS_BLUEVIOLET = 0x8A2BE2,      KCOLORS_POWDERBLUE = 0xB0E0E6,
        KCOLORS_PURPLE = 0x800080,          KCOLORS_GHOSTWHITE = 0xF8F8FF,
        KCOLORS_INDIGO = 0x4B0082,          KCOLORS_DODGERBLUE = 0x1E90FF,
        KCOLORS_LAVENDER = 0xE6E6FA,        KCOLORS_NAVY = 0x000080,
        KCOLORS_MIDNIGHTBLUE = 0x191970,    KCOLORS_CORNFLOWERBLUE = 0x6495ED,
        KCOLORS_COBALT = 0x3D59AB,          KCOLORS_ROYALBLUE = 0x4169E1,
        KCOLORS_LIGHTSTEELBLUE = 0xB0C4DE,  KCOLORS_SLATEGRAY = 0x708090,
        KCOLORS_SKYBLUE = 0x87CEEB,         KCOLORS_PEACOCK = 0x33A1C9,
        KCOLORS_CADETBLUE = 0x5F9EA0,       KCOLORS_AZURE = 0xF0FFFF,
        KCOLORS_COLDGREY = 0x808A87,        KCOLORS_SPRINGGREEN = 0x00FF7F,
        KCOLORS_EMERALDGREEN = 0x00C957,    KCOLORS_MINT = 0xBDFCC9,
        KCOLORS_DARKSEAGREEN = 0x8FBC8F,    KCOLORS_PALEGREEN = 0x98FB98,
        KCOLORS_FORESTGREEN = 0x228B22,     KCOLORS_LAWNGREEN = 0x7CFC00,
        KCOLORS_GREENYELLOW = 0xADFF2F,     KCOLORS_MELON = 0xE3A869,
        KCOLORS_OLIVEDRAB = 0x6B8E23,       KCOLORS_IVORY = 0xFFFFF0,
        KCOLORS_BEIGE = 0xF5F5DC,           KCOLORS_ORANGE = 0xFF8000,
        KCOLORS_OLIVE = 0x808000,           KCOLORS_DARKKHAKI = 0xBDB76B,
        KCOLORS_KHAKI = 0xF0E68C,           KCOLORS_SIENNA = 0xA0522D,
        KCOLORS_PALEGOLDENROD = 0xEEE8AA,   KCOLORS_BANANA = 0xE3CF57,
        KCOLORS_GOLD = 0xFFD700,            KCOLORS_SNOW = 0xFFFAFA,
        KCOLORS_CADMIUMYELLOW = 0xFF9912,   KCOLORS_BRICK = 0x9C661F,
        KDCOLORS_ARKORANGE = 0xFF8C00,      KCOLORS_CARROT = 0xED9121,
        KCOLORS_FLESH = 0xFF7D40,           KCOLORS_CADMIUMORANGE = 0xFF6103,
        KCOLORS_CORAL = 0x8B5742,           KCOLORS_SALMON = 0xFA8072,
        KCOLORS_BROWN = 0xA52A2A,           KCOLORS_FIREBRICK = 0xB22222,
        KCOLORS_GRAY = 0x808080
    };

    enum KColorComponent{
        KC_R = 0,
        KC_G = 1,
        KC_B = 2,
        KC_A = 3
    };

    enum KRectComponent{
        KR_X = 0,
        KR_Y = 1,
        KR_W = 2,
        KR_H = 3
    };


    enum KFontCharSetTypes{
        KFONT_ANSI,
        KFONT_ARABIC,
        KFONT_HEBREW,
        KFONT_AUTO
    };

    enum KTextureFilterTypes{
        KTF_NEAREST = 0,
        KTF_LINEAR
    };

    enum KTextureWrapTypes{
        KTW_REPEAT = 0,
        KTW_MIRRORED_REPEAT,
        KTW_CLAMP_TO_EDGE,
        //KTW_CLAMP_TO_BORDER // border not supported (see KTexture::create())
    };

    namespace Internal{
    enum KGLBindGuardTypes{
        KBGUARD_NONE = 0,
        KBGUARD_TEXTURE,
        KBGUARD_BUFFER
    };

    enum KRenderMode{
        KRM_VBO,
        KRM_VARRAY,
        KRM_UNSET
    };
    }

}
#endif // KGRAPHICTYPES_H
