set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/core)

# all source files
set(COR_SRC
    ${SRCROOT}/kfistream.cpp
    ${SRCROOT}/klistener.cpp
    ${SRCROOT}/ksystem.cpp
    ${SRCROOT}/kcomponent.cpp
    ${SRCROOT}/kentity.cpp
    ${SRCROOT}/kentitymanager.cpp
    ${SRCROOT}/kmessage.cpp
    ${SRCROOT}/kmessenger.cpp
    #${SRCROOT}/kcoreinstance.cpp
    ${SRCROOT}/kcoreutil.cpp
    ${SRCROOT}/tp/mmhash3.cpp
    PARENT_SCOPE
)

# all header files
set(COR_INC
    ${INCROOT}/kcore.h
    ${INCROOT}/kistream.h
    ${INCROOT}/kostream.h
    ${INCROOT}/kfistream.h
    ${INCROOT}/kdelegate.h
    ${INCROOT}/kany.h
    ${INCROOT}/kvariant.h
    ${INCROOT}/ksystem.h
    ${INCROOT}/kcomponent.h
    ${INCROOT}/kentity.h
    ${INCROOT}/kentitymanager.h
    ${INCROOT}/kresource.h
    ${INCROOT}/kresourcemanager.h
    ${INCROOT}/knoncopyable.h
    ${INCROOT}/kmessage.h
    ${INCROOT}/kmessenger.h
    ${INCROOT}/klistener.h
    #${INCROOT}/kcoreinstance.h
    ${INCROOT}/kcoredef.h
    ${INCROOT}/kcorestructs.h
    ${INCROOT}/kcoretypes.h
    ${INCROOT}/kcoreutil.h
    PARENT_SCOPE
)

