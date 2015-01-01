#pragma once

#ifndef __TYPES_H__
#define __TYPES_H__

typedef bool				Bool;
typedef char				Int8;
typedef short				Int16;
typedef int					Int32;

typedef unsigned char		Uint8;
typedef unsigned short		Uint16;
typedef unsigned int		Uint32;
typedef unsigned int		Uint;
typedef float				Float;
typedef double				Double;

#ifdef __GNUC__
typedef long long			Int64;
typedef unsigned long long	Uint64;
#else
typedef __int64				Int64;
typedef unsigned __int64	Uint64;
#endif

#ifndef _WIN32
typedef long				HRESULT;
typedef unsigned long		HWND;

#define LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#endif

#ifdef _WIN32
#define LOAD_LIBRARY(a)			LoadLibrary(a)
#define CLOSE_LIBRARY			FreeLibrary
#define GET_PROC_ADDR			GetProcAddress
#define LIBRARY_EXT				".dll"
#else
#define LOAD_LIBRARY(a)			dlopen(a,RTLD_NOW)
#define CLOSE_LIBRARY			dlclose
#define GET_PROC_ADDR			dlsym
#define LIBRARY_EXT				".so"
#endif

const Int8      cMaxInt8	= 127;
const Uint8     cMaxUInt8	= 255;
const Int16     cMaxInt16	= 32767;
const Uint16    cMaxUInt16	= 0xffff;
const Int32     cMaxInt32	= 2147483647;
const Uint32    cMaxUInt32	= 0xffffffff;
const Float     cMaxFloat	= 3.402823466e+38F;
const Double    cMaxDouble	= 1.7976931348623158e+308;
const Int8      cMinInt8	= -127-1;
const Int16     cMinInt16	= -32767-1;
const Int32     cMinInt32	= -2147483647-1;
const Float     cMinFloat	= 1.175494351e-38F;
const Double    cMinDouble	= 2.2250738585072014e-308;
const Uint32    cInvalidIndex = 0xffffffff;
const Uint32    cMaxPath = 260;

#define Align(alignment)	__declspec(align(alignment))
#define UnrefParam(p)	(p)

#define MakeFourCC(ch0, ch1, ch2, ch3) \
	((Uint32)(Uint8)(ch0) | ((Uint32)(Uint8)(ch1) << 8) |   \
	((Uint32)(Uint8)(ch2) << 16) | ((Uint32)(Uint8)(ch3) << 24))

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN (64)
#endif

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN (260)
#endif

template <typename T, Uint32 n>
Uint32 ArraySize(T (&)[n])
{
	return n;
}

#endif //__TYPES_H__
