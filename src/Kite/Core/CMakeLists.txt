set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/core)

# all source files
set(COR_SRC
    ${SRCROOT}/kcoredef.cpp
    ${SRCROOT}/kcorestructs.cpp
    ${SRCROOT}/kcoretypes.cpp
    #${SRCROOT}/kcoreinstance.cpp
    ${SRCROOT}/kcoreutil.cpp
    ${SRCROOT}/tp/mmhash3.cpp
    PARENT_SCOPE
)

# all header files
set(COR_INC
    ${INCROOT}/kcore.h
    ${INCROOT}/kdelegate.h
    ${INCROOT}/kany.h
    ${INCROOT}/kcoredef.h
    ${INCROOT}/kcorestructs.h
    ${INCROOT}/kcoretypes.h
    ${INCROOT}/kcoreutil.h
    PARENT_SCOPE
)

