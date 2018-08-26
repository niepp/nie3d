#pragma once
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// define supported target platform macro
#define PLATFORM_UNKNOWN			0
#define PLATFORM_IOS				1
#define PLATFORM_ANDROID			2
#define PLATFORM_WIN32				3
#define PLATFORM_LINUX				4
#define PLATFORM_MAC				5

// define compiler macro
#define COMPILER_NONE				0
#define COMPILER_MSVC 				1
#define COMPILER_GNUC 				2
#define COMPILER_GCCE 				3

// define architecture macro
#define ARCHITECTURE_32 			1
#define ARCHITECTURE_64 			2

// check compiler
#define COMPILER			 		COMPILER_NONE
#if defined( __GCCE__ )
#	undef COMPILER
#	define COMPILER				COMPILER_GCCE
#	define COMP_VER				_MSC_VER
#	ifdef BUILD_MESSAGE
#		pragma message("COMPILER = COMPILER_GCCE")
#	endif
//#	include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE 
#elif defined( _MSC_VER )
#	undef COMPILER
#	define COMPILER			COMPILER_MSVC
#	define COMP_VER			_MSC_VER
#	ifdef BUILD_MESSAGE
#		pragma message("COMPILER = COMPILER_MSVC")
#	endif
#elif defined( __GNUC__ )
#	undef COMPILER
#	define COMPILER			COMPILER_GNUC
#	define COMP_VER			(((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#	ifdef BUILD_MESSAGE
#		pragma message("COMPILER = COMPILER_GNUC")
#	endif
#endif

// check platform
#define PLATFORM				PLATFORM_UNKNOWN
// win32
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WINDOWS)
#	undef  PLATFORM
#	define PLATFORM	PLATFORM_WIN32
#	ifdef BUILD_MESSAGE
#		pragma message("PLATFORM = PLATFORM_WIN32")
#	endif
#endif

// linux / android
#if defined(LINUX) || defined(linux) || defined(__linux__)
#	undef  PLATFORM
#	if defined(ANDROID) || defined(__ANDROID__)
#		define PLATFORM	PLATFORM_ANDROID
#		ifdef BUILD_MESSAGE
#			pragma message("PLATFORM = PLATFORM_ANDROID")
#		endif
#	else
#		define PLATFORM	PLATFORM_LINUX
#		ifdef BUILD_MESSAGE
#			pragma message("PLATFORM = PLATFORM_LINUX")
#		endif
#	endif
#endif

// mac / ios
#if defined(__APPLE_CC__) || defined(APPLE) || defined(__MACH__)
#	undef  PLATFORM
#	if (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000)
#		define PLATFORM	PLATFORM_IOS
#		ifdef BUILD_MESSAGE
#			pragma message("PLATFORM = PLATFORM_IOS")
#		endif
#	else 
#		define PLATFORM	PLATFORM_MAC
#		ifdef BUILD_MESSAGE
#			pragma message("PLATFORM = PLATFORM_MAC")
#		endif
#	endif
#endif


// check architecture type
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#	define ARCH_TYPE ARCHITECTURE_64
#	ifdef BUILD_MESSAGE
#		pragma message("ARCH_TYPE = ARCHITECTURE_64")
#	endif
#else
#	define ARCH_TYPE ARCHITECTURE_32
#	ifdef BUILD_MESSAGE
#		pragma message("ARCH_TYPE = ARCHITECTURE_32")
#	endif
#endif



//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if (PLATFORM == PLATFORM_UNKNOWN)
#error  "Cannot recognize the target platform, Abort!"
#endif

// check user set compiler
#if (COMPILER == COMPILER_NONE)
#error  "Cannot recognize the compiler, Abort!"
#endif

#endif //__PLATFORM_H__
