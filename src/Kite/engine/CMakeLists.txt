set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/engine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/engine)

# all source files
set(ENG_SRC
    ${SRCROOT}/kengine.cpp
    PARENT_SCOPE
)

# all header files
set(ENG_INC
    ${INCROOT}/kengine.h
    PARENT_SCOPE
)

