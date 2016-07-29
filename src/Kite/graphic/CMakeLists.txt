set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/graphic)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/graphic)

# all source files
set(GFX_SRC
    ${SRCROOT}/tp/glew.c
    ${SRCROOT}/glcall.cpp
    ${SRCROOT}/imageio.cpp
    ${SRCROOT}/kframebuffer.cpp
    ${SRCROOT}/kgraphicstructs.cpp
    ${SRCROOT}/kimage.cpp
    ${SRCROOT}/kshader.cpp
    ${SRCROOT}/kshaderprogram.cpp
    ${SRCROOT}/ktexture.cpp
    ${SRCROOT}/kvertexarray.cpp
    ${SRCROOT}/kvertexbuffer.cpp
    ${SRCROOT}/kgraphictypes.cpp
    PARENT_SCOPE
)

# all header files
set(GFX_INC
    ${SRCROOT}/glcall.h
    ${SRCROOT}/imageio.h
    ${INCROOT}/kframebuffer.h
    ${INCROOT}/kgraphicstructs.h
    ${INCROOT}/kgraphictypes.h
    ${INCROOT}/kimage.h
    ${INCROOT}/kshader.h
    ${INCROOT}/kshaderprogram.h
    ${INCROOT}/ktexture.h
    ${INCROOT}/kvertexarray.h
    ${INCROOT}/kvertexbuffer.h
    PARENT_SCOPE
)

