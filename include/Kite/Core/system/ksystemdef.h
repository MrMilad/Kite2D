/*
    Kite2D Game Framework.
    Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/
#ifndef KSYSTEMDEF_H
#define KSYSTEMDEF_H

#include <cstdio>
#include <iostream>
#include <cstdint>

/// identify the Little/Big endian
#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__hppa__)

    /// Big endian
    #define KITE_ENDIAN_BIG

#else

    /// Little endian
    #define KITE_ENDIAN_LITTLE
	#define MINIZ_LITTLE_ENDIAN 1

#endif

/// identify the operating system
#if defined(_WIN32) || defined(__WIN32__) || defined(__WIN64)

    /// Windows
    #define KITE_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

	/// 32 or 64 bit
	#if _WIN64
		#define KITE_ENV_64
		#define MINIZ_HAS_64BIT_REGISTERS 1
	#else
		#define KITE_ENV_32
	#endif

#elif defined(linux) || defined(__linux)

    /// Linux
    #define KITE_PLATFORM_LINUX

	/// 32 or 64 bit
	#if __x86_64__ || __ppc64__
		#define KITE_ENV_64
		#define MINIZ_HAS_64BIT_REGISTERS 1
	#else
		#define KITE_ENV_32
	#endif

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

    /// MacOS
    #define KITE_PLATFORM_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

    /// FreeBSD
    #define KITE_PLATFORM_FREEBSD

#else

    /// Unsupported system
    #error This operating system is not supported by Kite library

#endif

// cpp version
#if (__cplusplus > 199711L)
	#define KITE_CPP_VER11
#endif


/// import / export macros (static/shared(dynamic) 
#if !defined(KITE_STATIC)

    #if defined(KITE_PLATFORM_WINDOWS)

        /// Windows compilers import / export specific
        #define KITE_FUNC_EXPORT __declspec(dllexport)
        #define KITE_FUNC_IMPORT __declspec(dllimport)

        /// turn off C4251 warning (in Visual C++ compilers)
        #ifdef _MSC_VER

            #pragma warning(disable : 4251)

        #endif

    #else /// Linux, FreeBSD, Mac OS X

        #if __GNUC__ >= 4

            /// GCC 4 has special keywords for showing/hidding symbols,
            /// the same keyword is used for both importing and exporting
            #define KITE_FUNC_EXPORT __attribute__ ((__visibility__ ("default")))
            #define KITE_FUNC_IMPORT __attribute__ ((__visibility__ ("default")))

        #else

            /// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define KITE_FUNC_EXPORT
            #define KITE_FUNC_IMPORT

        #endif

    #endif

#else

    /// Static build doesn't need import/export macros
    #define KITE_FUNC_EXPORT
    #define KITE_FUNC_IMPORT

#endif

/// alignment macro
#if defined(_MSC_VER)
    #define K_ALIGNED(t,x) __declspec(align(x)) t
#elif defined(__GNUC__)
    #define K_ALIGNED(t,x) t __attribute__ ((aligned(x)))
#endif

/// default (without any debug flag): framework will compiled in release state.
#if !defined(KITE_DEV_DEBUG) || !defined(KITE_USER_DEBUG)
    #define KITE_RELEASE
#endif

/// debug macro(s)
#if defined(KITE_DEV_DEBUG) // break in source cod.
    #include <assert.h>
    #define KDEBUG_BREAK assert(0);
    #define KDEBUG_PRINT(x) printf (x); printf("\n     Line: %u \n     File: %s \n", __LINE__, __FILE__);
    #define KDEBUG_ASSERT(expr) assert(expr);
    #define KDEBUG_ASSERT_T(expr) assert(expr);
    #define KDEBUG_ASSERT_EQ(expr, eq) assert(expr != eq);
    #define KDEBUG_TEMP(expr) expr

#elif defined(KITE_USER_DEBUG) // print debug output (without break).
    #define KDEBUG_BREAK
    #define KDEBUG_PRINT(x) printf (x); printf("\n     Line: %u \n     File: %s \n", __LINE__, __FILE__);
    #define KDEBUG_ASSERT(expr) if (!(expr)) {KDEBUG_PRINT(#expr)}
    #define KDEBUG_ASSERT_T(expr) if (!(expr)) {KDEBUG_PRINT(#expr)}
    #define KDEBUG_ASSERT_EQ(expr, eq) if(expr != eq) {KDEBUG_PRINT(#expr)}
    #define KDEBUG_TEMP(expr) expr

#else /// release state. (without debug output and break)
    #define KDEBUG_PRINT(x)
    #define KDEBUG_BREAK
    #define KDEBUG_ASSERT(expr) expr;
    #define KDEBUG_ASSERT_T(expr)
    #define KDEBUG_ASSERT_EQ(expr, eq) expr
    #define KDEBUG_TEMP(expr)
#endif

/// using sse
#if defined(KITE_USE_SSE)
    #ifndef __SSE__
        #define __SSE__
    #endif
#endif

/// using DirectInput (only for joystick(s))
#if defined(KITE_USE_DIRECTINPUT)
    #define KDINPUT_ALLOW
#endif

#ifndef DWORD
/// 64 bits integer types
typedef unsigned short WORD;
typedef unsigned long DWORD;
#endif

/// max audio buffer (ogg vorbis)
#define KOGG_BUFF_SIZE 4096

namespace Kite{
	/*
    // 8 bits integer types
    typedef signed char I8;
    typedef unsigned char U8;

    // 16 bits integer types
    typedef signed short I16;
    typedef unsigned short U16;

    // 32 bits integer types
    typedef signed int I32;
    typedef unsigned int U32;

    typedef signed long int IL32;
    typedef unsigned long int UL32;

    // 64 bits integer types
    typedef signed long long int I64;
    typedef unsigned long long int U64;

    // 32 bits floating types
    typedef float F32;

    // 64 bits floating types
    typedef double F64;
	*/

	// 8 bits integer types
	typedef int8_t I8;
	typedef uint8_t U8;

	// 16 bits integer types
	typedef int16_t I16;
	typedef uint16_t U16;

	// 32 bits integer types
	typedef int32_t I32;
	typedef uint32_t U32;

	// 64 bits integer types
	typedef int64_t I64;
	typedef uint64_t U64;

	// 32 bits floating types
	typedef float F32;

	// 64 bits floating types
	typedef double F64;

    // glfw window handle
    typedef void *KWindowHandle;

    // OpenGL Context is HGLRC
    typedef void *KContextHandle;

}
#endif // KSYSTEMDEF_H
