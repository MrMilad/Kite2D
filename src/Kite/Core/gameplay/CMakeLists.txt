set(INCROOT ${PROJECT_SOURCE_DIR}/include/Kite/Core/gameplay)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Kite/Core/gameplay)

# all source files
set(GAM_SRC
    ${SRCROOT}/kcomponent.cpp
    ${SRCROOT}/kentity.cpp
    ${SRCROOT}/kmessage.cpp
    ${SRCROOT}/kmessenger.cpp
    PARENT_SCOPE
)

# all header files
set(GAM_INC
    ${INCROOT}/kcomponent.h
    ${INCROOT}/kentity.h
    ${INCROOT}/kmessage.h
    ${INCROOT}/kmessenger.h
    ${INCROOT}/kcommanager.h
    ${INCROOT}/kgameplaystructs.h
    ${INCROOT}/kgameplaytypes.h
    ${INCROOT}/kmessagehandler.h
    PARENT_SCOPE
)

