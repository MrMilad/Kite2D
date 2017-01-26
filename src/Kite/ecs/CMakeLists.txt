set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/ecs)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/ecs)

# all source files
set(ECS_SRC
    ${SRCROOT}/kecsstructs.cpp
    ${SRCROOT}/kecstypes.cpp
    ${SRCROOT}/ksystem.cpp
    ${SRCROOT}/kcomponent.cpp
    ${SRCROOT}/kentity.cpp
    ${SRCROOT}/kscene.cpp
    ${SRCROOT}/kprefab.cpp
    ${SRCROOT}/kresourcemanager.cpp
    ${SRCROOT}/kresource.cpp
    ${SRCROOT}/kmessage.cpp
    ${SRCROOT}/kmessenger.cpp
    PARENT_SCOPE
)

# all header files
set(ECS_INC
    ${INCROOT}/kecs.h
    ${INCROOT}/kprefab.h
    ${INCROOT}/ksystem.h
    ${INCROOT}/kcomponent.h
    ${INCROOT}/kentity.h
    ${INCROOT}/kscene.h
    ${INCROOT}/kscenemanager.h
    ${INCROOT}/kmessage.h
    ${INCROOT}/kmessenger.h
    ${INCROOT}/klistener.h
    ${INCROOT}/kecsstructs.h
    ${INCROOT}/kecstypes.h
    ${INCROOT}/kcfstorage.h
    ${INCROOT}/kresourcemanager.h
    ${INCROOT}/kresource.h
    PARENT_SCOPE
)

