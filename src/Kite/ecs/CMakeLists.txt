set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/ecs)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/ecs)

# all source files
set(ECS_SRC
    ${SRCROOT}/kecsstructs.cpp
    ${SRCROOT}/kecstypes.cpp
    ${SRCROOT}/ksystem.cpp
    ${SRCROOT}/kcomponent.cpp
    ${SRCROOT}/kresourcemanager.cpp
    ${SRCROOT}/kresource.cpp
    ${SRCROOT}/knode.cpp
    PARENT_SCOPE
)

# all header files
set(ECS_INC
    ${INCROOT}/kecs.h
    ${INCROOT}/ksystem.h
    ${INCROOT}/kcomponent.h
    ${INCROOT}/kecsstructs.h
    ${INCROOT}/kecstypes.h
    ${INCROOT}/kresourcemanager.h
    ${INCROOT}/kresource.h
    ${INCROOT}/knode.h
    PARENT_SCOPE
)

