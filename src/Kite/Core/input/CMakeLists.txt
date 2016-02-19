set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/Core/input)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/Core/input)

# all source files
set(INP_SRC
    #${SRCROOT}/kjoystick.cpp
    ${SRCROOT}/kkeyboard.cpp
    ${SRCROOT}/kmouse.cpp
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
    PARENT_SCOPE
)

