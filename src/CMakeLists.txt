# let CMake know about our additional libraries paths (on Windows)
if (WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers")
    include_directories(${CMAKE_INCLUDE_PATH})
    if(ARCH_32BITS)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib-vc/win32")
    elseif(ARCH_64BITS)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib-vc/win64")
    endif()
endif()

# external lib directories
link_directories(${CMAKE_LIBRARY_PATH})

# add the Kite sources path
include_directories(${PROJECT_SOURCE_DIR})

# add meta path
include_directories(${PROJECT_SOURCE_DIR}/include/KiteMeta)

# set the output directory for Kite libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the modules subdirectories

# meta
add_subdirectory(Kite/meta)
source_group("Header Files\\Meta" FILES ${MET_INC})
source_group("Source Files\\Meta" FILES ${MET_SRC})
LIST(APPEND ALL_FILES ${MET_SRC} ${MET_INC})

# core
add_subdirectory(Kite/core)
source_group("Header Files\\Core" FILES ${COR_INC})
source_group("Source Files\\Core" FILES ${COR_SRC})
LIST(APPEND ALL_FILES ${COR_SRC} ${COR_INC})

# ecs
add_subdirectory(Kite/ecs)
source_group("Header Files\\ECS" FILES ${ECS_INC})
source_group("Source Files\\ECS" FILES ${ECS_SRC})
LIST(APPEND ALL_FILES ${ECS_SRC} ${ECS_INC})

# graphic
add_subdirectory(Kite/graphic)
source_group("Header Files\\Graphic" FILES ${GFX_INC})
source_group("Source Files\\Graphic" FILES ${GFX_SRC})
LIST(APPEND ALL_FILES ${GFX_SRC} ${GFX_INC})

# serialization
add_subdirectory(Kite/serialization)
source_group("Header Files\\Serialization" FILES ${SER_INC})
source_group("Source Files\\Serialization" FILES ${SER_SRC})
source_group("Header Files\\Serialization\\types" FILES ${SER_INC_SUB})
LIST(APPEND ALL_FILES ${SER_SRC} ${SER_INC} ${SER_INC_SUB})

# math
add_subdirectory(Kite/math)
source_group("Header Files\\Math" FILES ${MAT_INC})
source_group("Source Files\\Math" FILES ${MAT_SRC})
LIST(APPEND ALL_FILES ${MAT_SRC} ${MAT_INC})

# window
add_subdirectory(Kite/window)
source_group("Header Files\\Window" FILES ${WIN_INC})
source_group("Source Files\\Window" FILES ${WIN_SRC})
LIST(APPEND ALL_FILES ${WIN_SRC} ${WIN_INC})

# input
# add_subdirectory(Kite/input)
# source_group("Header Files\\Input" FILES ${INP_INC})
# source_group("Source Files\\Input" FILES ${INP_SRC})
# LIST(APPEND ALL_FILES ${INP_SRC} ${INP_INC})

# logic
# add_subdirectory(Kite/logic)
# source_group("Header Files\\Logic" FILES ${LOG_INC})
# source_group("Source Files\\Logic" FILES ${LOG_SRC})
# LIST(APPEND ALL_FILES ${LOG_SRC} ${LOG_INC})

# audio
#add_subdirectory(Kite/audio)
#source_group("Header Files\\Audio" FILES ${AUD_INC})
#source_group("Source Files\\Audio" FILES ${AUD_SRC})
#LIST(APPEND ALL_FILES ${AUD_SRC} ${AUD_INC})

# memory
add_subdirectory(Kite/memory)
source_group("Header Files\\Memory" FILES ${MEM_INC})
source_group("Source Files\\Memory" FILES ${MEM_SRC})
LIST(APPEND ALL_FILES ${MEM_SRC} ${MEM_INC})

# engine
add_subdirectory(Kite/engine)
source_group("Header Files\\Engine" FILES ${ENG_INC})
source_group("Source Files\\Engine" FILES ${ENG_SRC})
LIST(APPEND ALL_FILES ${ENG_SRC} ${ENG_INC})

# add kite2D vc project
if (BUILD_SHARED_LIBS)
add_library(Kite2D SHARED ${ALL_FILES})
else()
add_library(Kite2D ${ALL_FILES})
endif()

# add external lib
if (WINDOWS)
     target_link_libraries (Kite2D "lua.lib")
     target_link_libraries (Kite2D "sdl2.lib")
     target_link_libraries (Kite2D "opengl32.lib")
     target_link_libraries (Kite2D  $<$<CONFIG:Release>:foonathan_memory.lib>)
     target_link_libraries (Kite2D  $<$<CONFIG:Debug>:debug/foonathan_memory>)
endif()

# set debug flags
set_property(TARGET Kite2D PROPERTY COMPILE_DEFINITIONS
$<$<CONFIG:Debug>:KITE_DEV_DEBUG>)

# set debug name
set_property(TARGET Kite2D PROPERTY OUTPUT_NAME_DEBUG Kite2Dd)

# create directory for KHParser output
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/include/KiteMeta)

# bulid and run KHParser before building Kite
add_executable(KHParser ${PROJECT_SOURCE_DIR}/src/KHParser/khparser.cpp ${PROJECT_SOURCE_DIR}/src/KHParser/mmhash3.cpp)

# run KHParser
add_custom_target(KHParserRun
  COMMAND KHParser -i ${PROJECT_SOURCE_DIR}/include -o ${PROJECT_SOURCE_DIR}/include/KiteMeta
  COMMENT "run KHParser ...."
)

add_dependencies(Kite2D KHParserRun)

# copy dll to test directory
if (WINDOWS AND BUILD_SHARED_LIBS)       
    ADD_CUSTOM_COMMAND (TARGET Kite2D POST_BUILD

    # copy debug version of kite2D dll
    COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/debug/Kite2Dd.dll ${PROJECT_BINARY_DIR}/src/Debug

    # copy debug version PDB of kite2D dll
    COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/debug/Kite2Dd.pdb ${PROJECT_BINARY_DIR}/src/Debug

    # copy Lua.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlibs/bin/win32/lua.dll ${PROJECT_BINARY_DIR}/src/Debug

    # copy sdl2.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlibs/bin/win32/sdl2.dll ${PROJECT_BINARY_DIR}/src/Debug

    # copy openal.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlibs/bin/win32/OpenAL32.dll ${PROJECT_BINARY_DIR}/src/Debug

    # Output Message
    COMMENT "Copying Kite2D binaries" VERBATIM)            
endif()

# build test project
add_executable(Test ${PROJECT_SOURCE_DIR}/src/Test/main.cpp)

#add_dependencies(Test Kite2D)

# add Kite2D lib
if (WINDOWS)
    #TARGET_LINK_LIBRARIES(Test "${PROJECT_BINARY_DIR}/lib/debug/Kite2Dd.lib")
     target_link_libraries (Test "${PROJECT_BINARY_DIR}/lib/debug/Kite2Dd.lib")
     target_link_libraries (Test "lua.lib")
     target_link_libraries (Test "sdl2.lib")
     target_link_libraries (Test  $<$<CONFIG:Debug>:debug/foonathan_memory>)
endif()
