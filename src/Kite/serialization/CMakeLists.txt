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
    PARENT_SCOPE
)

# sub header files
set(SER_INC_SUB
    ${INCROOT}/types/kpod.h
    #${INCROOT}/types/kpolymorphism.h
    ${INCROOT}/types/kstdlist.h
    ${INCROOT}/types/kstdvector.h
    #${INCROOT}/types/kstdsmartptr.h
    ${INCROOT}/types/kstdstring.h
    PARENT_SCOPE
)
