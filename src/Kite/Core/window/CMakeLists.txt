set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/Core/window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/Core/window)

# all source files
set(WIN_SRC
    ${SRCROOT}/sdlcall.cpp
    ${SRCROOT}/sdlcall.h
    ${SRCROOT}/kglwindow.cpp
    ${SRCROOT}/kwindowutil.cpp
    PARENT_SCOPE
)

# all header files
set(WIN_INC
    ${INCROOT}/kglwindow.h
    ${INCROOT}/kwindow.h
    ${INCROOT}/kwindowstructs.h
    ${INCROOT}/kwindowtypes.h
    ${INCROOT}/kwindowutil.h
    PARENT_SCOPE
)

