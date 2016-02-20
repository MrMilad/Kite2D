set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/memory)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/memory)

# all source files
set(MEM_SRC
    ${SRCROOT}/kfreeliststorage.cpp
    ${SRCROOT}/klinearstorage.cpp
    ${SRCROOT}/kphysicalstorage.cpp
    ${SRCROOT}/kpoolstorage.cpp
    ${SRCROOT}/kproxystorage.cpp
    ${SRCROOT}/kstackstorage.cpp
    PARENT_SCOPE
)

# all header files
set(MEM_INC
    ${INCROOT}/kbasestorage.h
    ${INCROOT}/kfreeliststorage.h
    ${INCROOT}/klinearstorage.h
    ${INCROOT}/kphysicalstorage.h
    ${INCROOT}/kpoolstorage.h
    ${INCROOT}/kproxystorage.h
    ${INCROOT}/kstackstorage.h
    ${INCROOT}/kstdpoolallocator.h
    PARENT_SCOPE
)

