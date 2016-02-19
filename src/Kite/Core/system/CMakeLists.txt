set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/Core/system)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/Core/system)

# all source files
set(SYS_SRC
    ${SRCROOT}/kcoreinstance.cpp
    ${SRCROOT}/ksystemutil.cpp
    ${SRCROOT}/tp/mmhash3.cpp
    PARENT_SCOPE
)

# all header files
set(SYS_INC
    ${INCROOT}/kcoreinstance.h
    ${INCROOT}/ksystemutil.h
    ${INCROOT}/knoncopyable.h
    ${INCROOT}/krefvariant.h
    ${INCROOT}/ksystem.h
    ${INCROOT}/ksystemdef.h
    ${INCROOT}/ksystemstructs.h
    ${INCROOT}/ksystemtypes.h
    ${INCROOT}/ksystemutil.h
    ${INCROOT}/kvariant.h
    ${INCROOT}/kvariantbase.h
    PARENT_SCOPE
)

