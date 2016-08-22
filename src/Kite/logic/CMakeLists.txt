set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/logic)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/logic)

# all source files
set(LOG_SRC
    ${SRCROOT}/klogiccom.cpp
    ${SRCROOT}/klogicsys.cpp
    ${SRCROOT}/kscript.cpp
    ${SRCROOT}/klogicinstancecom.cpp
    PARENT_SCOPE
)

# all header files
set(LOG_INC
    ${INCROOT}/klogic.h
    ${INCROOT}/klogiccom.h
    ${INCROOT}/klogicsys.h
    ${INCROOT}/kscript.h
    ${INCROOT}/klogicinstancecom.h
    PARENT_SCOPE
)

