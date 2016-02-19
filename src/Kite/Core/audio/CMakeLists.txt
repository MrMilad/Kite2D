set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/Core/audio)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/Core/audio)

# all source files
set(AUD_SRC
    ${SRCROOT}/alcall.cpp
    ${SRCROOT}/alcall.h
    ${SRCROOT}/audioformatio.h
    ${SRCROOT}/klistener.cpp
    ${SRCROOT}/ksoundbuffer.cpp
    ${SRCROOT}/ksoundsource.cpp
    ${SRCROOT}/kstaticsource.cpp
    ${SRCROOT}/kstreamsource.cpp
    ${SRCROOT}/oggio.cpp
    ${SRCROOT}/oggio.h
    ${SRCROOT}/soundio.cpp
    ${SRCROOT}/soundio.h
    PARENT_SCOPE
)

# all header files
set(AUD_INC
    ${INCROOT}/kaudio.h
    ${INCROOT}/kaudiostructs.h
    ${INCROOT}/kaudiotypes.h
    ${INCROOT}/klistener.h
    ${INCROOT}/ksoundbuffer.h
    ${INCROOT}/ksoundsource.h
    ${INCROOT}/kstaticsource.h
    ${INCROOT}/kstreamsource.h
    PARENT_SCOPE
)

