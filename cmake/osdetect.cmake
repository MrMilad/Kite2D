# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS 1)

    # detect the architecture (note: this test won't work for cross-compilation)
    include(CheckTypeSize)
    check_type_size(void* SIZEOF_VOID_PTR)
    if("${SIZEOF_VOID_PTR}" STREQUAL "4")
        set(ARCH_32BITS 1)
    elseif("${SIZEOF_VOID_PTR}" STREQUAL "8")
        set(ARCH_64BITS 1)
    else()
        message(FATAL_ERROR "Unsupported architecture")
        return()
    endif()
else()
    message(FATAL_ERROR "Unsupported operating system")
    return()
endif()

# detect the compiler and its version
# we only support MSVC at current version
if(MSVC)
    set(COMPILER_MSVC 1)
else()
    message(FATAL_ERROR "Unsupported compiler")
    return()
endif()
