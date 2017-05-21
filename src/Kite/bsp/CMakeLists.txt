set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/bsp)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/bsp)

# all source files
set(BSP_SRC
    ${SRCROOT}/kbspstructs.cpp
    ${SRCROOT}/kbsptypes.cpp
    ${SRCROOT}/kspatialhash.cpp
    ${SRCROOT}/kboxcollidercom.cpp
    ${SRCROOT}/kbspsys.cpp
    PARENT_SCOPE
)

# all header files
set(BSP_INC
    ${INCROOT}/kbspstructs.h
    ${INCROOT}/kbsptypes.h
    ${INCROOT}/kspatialhash.h
    ${INCROOT}/kbspsys.h
    ${INCROOT}/kboxcollidercom.h
    PARENT_SCOPE
)

