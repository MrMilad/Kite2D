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
#ifndef KCOREDEF_H
#define KCOREDEF_H

#include <cstdio>
#include <iostream>
#include <cstdint>
#include <limits>

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
#if !defined(KITE_DEV_DEBUG) 
    #define KITE_RELEASE
#endif

/// debug macros
#if defined(KITE_DEV_DEBUG)
	#if defined (KITE_EDITOR)
namespace Kite {
	enum class msgType : short{
		MSG_DEBUG = 0,
		MSG_BREAK,
		MSG_ASSERT,
		MSG_LUA
	};
	typedef void(*printCallback)(const std::string &, msgType);
	extern printCallback pcallback;
	extern char *buffer;
	extern KITE_FUNC_EXPORT void defaultPrint(const std::string &Text, msgType MType);
	extern KITE_FUNC_EXPORT void setEditorPrintCallback(printCallback Callback);
}

#define KD_BREAK() sprintf(buffer, "%s()", __func__);\
(Kite::pcallback)(std::string(Kite::buffer), Kite::msgType::MSG_BREAK)

#define KD_FPRINT(fmt, ...) sprintf(buffer, "%s(): " fmt, __func__, __VA_ARGS__);\
(Kite::pcallback)(std::string(Kite::buffer), Kite::msgType::MSG_DEBUG)

#define KD_PRINT(exp) sprintf(buffer, "%s(): %s", __func__, exp);\
(Kite::pcallback)(std::string(Kite::buffer), Kite::msgType::MSG_DEBUG)

#define KD_ASSERT(expr) if (!(expr)) {sprintf(buffer, "%s()", __func__);\
(Kite::pcallback)(std::string(Kite::buffer), Kite::msgType::MSG_ASSERT);}

	#else
		#include <assert.h>
		#define KD_BREAK() fprintf(stderr, "BREAK: %s:%d:%s()\n", __FILE__, __LINE__, __func__); assert(0)
		#define KD_FPRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)
		#define KD_PRINT(exp) fprintf(stderr, "DEBUG: %s:%d:%s(): %s\n", __FILE__, __LINE__, __func__, exp)
		#define KD_ASSERT(expr) assert(expr)
	#endif

#else /// release state. (without debug output and break)
    #define KD_BREAK()
    #define KD_FPRINT(fmt, ...)
    #define KD_PRINT(exp)
	#define KD_ASSERT(expr)
#endif

/// using sse
#if defined(KITE_USE_SSE)
    #ifndef __SSE__
        #define __SSE__
    #endif
#endif

/// Kite2D version
#define K2D_VER_MAJ 1
#define K2D_VER_MIN 1
#define K2D_VER_BUILD 0

/// max audio buffer (ogg vorbis)
#define KOGG_BUFF_SIZE 4096
#define KTEXT_BUFF_SIZE 4096
#define KBSERIAL_CHUNK_SIZE 262144	// (256kb)
#define KCFSTORAGE_CHUNK_SIZE 256	// 256 item 

/// Kite2D hash seed
#define KHASH_SEED 13711010

#ifndef DWORD
/// 64 bits integer types
typedef unsigned short WORD;
typedef unsigned long DWORD;
#endif

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

	// int pointer
	typedef uintptr_t UPTR;

	// 8 bits integer types
	typedef int8_t I8;
	typedef uint8_t U8;
	const I8 MAX_I8 = std::numeric_limits<I8>::max();
	const I8 MIN_I8 = std::numeric_limits<I8>::min();
	const U8 MAX_U8 = std::numeric_limits<U8>::max();
	const U8 MIN_U8 = std::numeric_limits<U8>::min();

	// 16 bits integer types
	typedef int16_t I16;
	typedef uint16_t U16;
	const I16 MAX_I16 = std::numeric_limits<I16>::max();
	const I16 MIN_I16 = std::numeric_limits<I16>::min();
	const U16 MAX_U16 = std::numeric_limits<U16>::max();
	const U16 MIN_U16 = std::numeric_limits<U16>::min();


	// 32 bits integer types
	typedef int32_t I32;
	typedef uint32_t U32;
	const I32 MAX_I32 = std::numeric_limits<I32>::max();
	const I32 MIN_I32 = std::numeric_limits<I32>::min();
	const U32 MAX_U32 = std::numeric_limits<U32>::max();
	const U32 MIN_U32 = std::numeric_limits<U32>::min();

	// 64 bits integer types
	typedef int64_t I64;
	typedef uint64_t U64;
	const I64 MAX_I64 = std::numeric_limits<I64>::max();
	const I64 MIN_I64 = std::numeric_limits<I64>::min();
	const U64 MAX_U64 = std::numeric_limits<U64>::max();
	const U64 MIN_U64 = std::numeric_limits<U64>::min();

	// 32 bits floating types
	typedef float F32;
	const F32 MAX_F32 = std::numeric_limits<F32>::max();
	const F32 MIN_F32 = -std::numeric_limits<F32>::min();

	// 64 bits floating types
	typedef double F64;
	const F64 MAX_F64 = std::numeric_limits<F64>::max();
	const F64 MIN_F64 = -std::numeric_limits<F64>::min();

	// size_t
	typedef size_t SIZE;
	const SIZE MAX_PTR_SIZE = std::numeric_limits<SIZE>::max();
	const SIZE MIN_PTR_SIZE = std::numeric_limits<SIZE>::min();

    // glfw window handle
    typedef void *KWindowHandle;

    // OpenGL Context is HGLRC
    typedef void *KContextHandle;


	//KM_CLASS(POD)
	struct KFileInfo {
		//KMETA_KFILEINFO_BODY();

		std::string name;
		std::string path;
		std::string fullPath;
	};
}
#endif // KCOREDEF_H
