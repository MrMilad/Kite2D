set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/stream)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/stream)

# all source files
set(STR_SRC
    ${SRCROOT}/tp/miniz.c
    ${SRCROOT}/zipio.cpp
    ${SRCROOT}/kiostream.cpp
    ${SRCROOT}/karchivestream.cpp
    ${SRCROOT}/kfilestream.cpp
    ${SRCROOT}/kstreamtypes.cpp
    ${SRCROOT}/kstreamstructs.cpp
    PARENT_SCOPE
)

# all header files
set(STR_INC
    ${SRCROOT}/zipio.h
    ${INCROOT}/kiostream.h
    ${INCROOT}/karchivestream.h
    ${INCROOT}/kfilestream.h
    ${INCROOT}/kstreamtypes.h
    ${INCROOT}/kstreamstructs.h
    PARENT_SCOPE
)

