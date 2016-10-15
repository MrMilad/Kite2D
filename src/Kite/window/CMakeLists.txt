set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/window)

# all source files
set(WIN_SRC
    ${SRCROOT}/sdlcall.cpp
    ${SRCROOT}/sdlcall.h
    ${SRCROOT}/kglwindow.cpp
    ${SRCROOT}/kwindowstructs.cpp
    PARENT_SCOPE
)

# all header files
set(WIN_INC
    ${INCROOT}/kglwindow.h
    ${INCROOT}/kwindow.h
    ${INCROOT}/kwindowstructs.h
    ${INCROOT}/kwindowtypes.h
    PARENT_SCOPE
)

