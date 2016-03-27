set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/input)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/input)

# all source files
set(INP_SRC
    ${SRCROOT}/kinputtypes.cpp
    #${SRCROOT}/kjoystick.cpp
    ${SRCROOT}/kkeyboard.cpp
    ${SRCROOT}/kmouse.cpp
    ${SRCROOT}/kinputcom.cpp
    ${SRCROOT}/kinputsys.cpp
    PARENT_SCOPE
)

# all header files
set(INP_INC
    #${INCROOT}/kinput.h
    ${INCROOT}/kinputstructs.h
    ${INCROOT}/kinputtypes.h
    #${INCROOT}/kjoystick.h
    ${INCROOT}/kkeyboard.h
    ${INCROOT}/kmouse.h
    ${INCROOT}/kinputcom.h
    ${INCROOT}/kinputsys.h
    PARENT_SCOPE
)

