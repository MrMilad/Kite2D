set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/meta)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/meta)

# all source files
set(MET_SRC
    ${SRCROOT}/kmetaclass.cpp
    ${SRCROOT}/kmetaenum.cpp
    ${SRCROOT}/kmetamanager.cpp
    ${SRCROOT}/kmetaobject.cpp
    ${SRCROOT}/kmetapod.cpp
    PARENT_SCOPE
)

# all header files
set(MET_INC
    ${INCROOT}/kmetaclass.h
    ${INCROOT}/kmetadef.h
    ${INCROOT}/kmetaenum.h
    ${INCROOT}/kmetamanager.h
    ${INCROOT}/kmetapod.h
    ${INCROOT}/kmetaobject.h
    ${INCROOT}/kmetatypes.h
    PARENT_SCOPE
)

