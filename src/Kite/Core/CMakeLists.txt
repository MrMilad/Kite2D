set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/core)

# all source files
set(COR_SRC
    ${SRCROOT}/kcomponent.cpp
    ${SRCROOT}/kentity.cpp
    ${SRCROOT}/kmessage.cpp
    ${SRCROOT}/kmessenger.cpp
    ${SRCROOT}/kcoreinstance.cpp
    ${SRCROOT}/kcoreutil.cpp
    ${SRCROOT}/tp/mmhash3.cpp
    PARENT_SCOPE
)

# all header files
set(COR_INC
    ${INCROOT}/kcomponent.h
    ${INCROOT}/kentity.h
    ${INCROOT}/kmessage.h
    ${INCROOT}/kmessenger.h
    ${INCROOT}/kcommanager.h
    ${INCROOT}/kmessagehandler.h
    ${INCROOT}/kcoreinstance.h
    ${INCROOT}/knoncopyable.h
    ${INCROOT}/krefvariant.h
    ${INCROOT}/kcore.h
    ${INCROOT}/kcoredef.h
    ${INCROOT}/kcorestructs.h
    ${INCROOT}/kcoretypes.h
    ${INCROOT}/kcoreutil.h
    ${INCROOT}/kvariant.h
    ${INCROOT}/kvariantbase.h
    PARENT_SCOPE
)

