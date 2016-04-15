set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/serialization)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/serialization)

# all source files
set(SER_SRC
    #${SRCROOT}/kbaseserial.cpp
    ${SRCROOT}/kbinaryserial.cpp
    PARENT_SCOPE
)

# all header files
set(SER_INC
    ${INCROOT}/kbaseserial.h
    ${INCROOT}/kbinaryserial.h
    ${INCROOT}/kserialization.h
    PARENT_SCOPE
)

# sub header files
set(SER_INC_SUB
    ${INCROOT}/types/kstdpair.h
    ${INCROOT}/types/kstdlist.h
    ${INCROOT}/types/kstdvector.h
    ${INCROOT}/types/kstdstring.h
    ${INCROOT}/types/kstdumap.h
    ${INCROOT}/types/kstddeque.h
    PARENT_SCOPE
)
