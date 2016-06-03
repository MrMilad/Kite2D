
cmake_minimum_required(VERSION 3.0)

# define a macro that helps defining an option
macro(kite_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set multiconfig 
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) 

# project name
project(Kite2D)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# include the OS detection file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/osdetect.cmake)

# setup version numbers
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)

# add an option for choosing the build type (shared or static)
kite_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Kite2D as shared libraries, FALSE to build it as static libraries")

# add an option for choosing the build type (shared or static)
kite_option(BUILD_EDITOR_VER TRUE BOOL "TRUE to build Kite2D as editor version, FALSE to build it as normal version")

# add the Kite2D header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# define KITE_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DKITE_STATIC)
endif()

# define KITE_CPP_VER11 c++11 flag
add_definitions(-DKITE_CPP_VER11)

# define KITE_EDITOR editor version (install editor hooks in main loop)
if (BUILD_EDITOR_VER)
	add_definitions(-DKITE_EDITOR)
endif()

# set some msvc compiler flags
if(COMPILER_MSVC)
	# remove SL security warnings with Visual C++
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)

	# support unicode
	add_definitions(-DUNICODE)

	# Here it is setting the Visual Studio warning level to 3
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

# define an option for choosing between static and dynamic C runtime (Windows only)
if(WINDOWS)
    kite_option(KITE_USE_STATIC_STD_LIBS FALSE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")
    
    # for VC++, we can apply it globally by modifying the compiler flags
    if(COMPILER_MSVC AND KITE_USE_STATIC_STD_LIBS)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

# Kite2D
add_subdirectory(src)

