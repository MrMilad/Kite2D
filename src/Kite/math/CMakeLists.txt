set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/math)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/math)

# all source files
set(MAT_SRC
    ${SRCROOT}/kmathtypes.cpp
    ${SRCROOT}/kmathstructs.cpp
    #${SRCROOT}/kgeometric.cpp
    #${SRCROOT}/kinterpolate.cpp
    ${SRCROOT}/kmatrix3.cpp
    ${SRCROOT}/ktransform.cpp
    ${SRCROOT}/ktransformcom.cpp
    ${SRCROOT}/ktransformsys.cpp
    PARENT_SCOPE
)

# all header files
set(MAT_INC
    ${INCROOT}/kmath.h
    #${INCROOT}/kgeometric.h
    #${INCROOT}/kinterpolate.h
    #${INCROOT}/ktween.h
    ${INCROOT}/kmathdef.h
    ${INCROOT}/kmathstructs.h
    ${INCROOT}/kmathtypes.h
    ${INCROOT}/kmatrix3.h
    ${INCROOT}/ktransform.h
    ${INCROOT}/ktransformcom.h
    ${INCROOT}/ktransformsys.h
    PARENT_SCOPE
)

