// Copyright (C) Ellisys. All rights reserved.
//
// This code is provided "as is" without warranty of any kind,
// either expressed or implied.
//
// Please read the license agreement for more information.
//
#pragma once

#ifndef __STANDARDELLISYS_H__
#define __STANDARDELLISYS_H__

#ifdef _MSC_VER
#	ifndef __cplusplus
#		error Must use C++ to compile this code
#	endif
#endif

// FIX: We do not want to use the "secured" STL
#if _SECURE_SCL
#	error The secured STL is not supported. Please include this header file before any STL header.
#endif
#define _SECURE_SCL 0

// FIX
#if defined(_UNICODE) && !defined(UNICODE)
#	define UNICODE
#endif
#if defined(UNICODE) && !defined(_UNICODE)
#	define _UNICODE
#endif

#include <tchar.h>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <basetsd.h> // for DWORD_PTR

#ifdef _DEBUG
#	ifndef C_ASSERT
#		define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#	endif
#	ifndef ASSERT
#		if _MSC_VER >= 1300
#			include <crtdbg.h>
#			define THIS_FILE __FILE__
#			define ASSERT(expr) (void) ((expr) || (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL)) || (_CrtDbgBreak(), 0))
#		else
#			include <assert.h>
#			define ASSERT(expr) assert(expr)
#		endif
#	endif
#else
#	ifndef C_ASSERT
#		define C_ASSERT(e)
#	endif
#	ifndef ASSERT
#		define ASSERT(expr) (void) (0)
#	endif
#endif

// Use macro "HERE" with the following form:
// #pragma message(HERE "Warning: XXX")
#define LINE_STRING_1(x)  #x
#define LINE_STRING_2(x)  LINE_STRING_1(x)
#define LINE_STRING       LINE_STRING_2(__LINE__)
#define HERE              __FILE__ "(" LINE_STRING ") : "

C_ASSERT( sizeof(char) == 1 );
C_ASSERT( sizeof(short) == 2 );
C_ASSERT( sizeof(long) == 4 );
C_ASSERT( sizeof(int) == 4 );

C_ASSERT( sizeof(char) == sizeof(unsigned char) );
C_ASSERT( sizeof(char) == sizeof(signed char) );
C_ASSERT( sizeof(int) == sizeof(unsigned int) );
C_ASSERT( sizeof(int) == sizeof(signed int) );
C_ASSERT( sizeof(int) == sizeof(long) );
C_ASSERT( sizeof(long) == sizeof(unsigned long) );
C_ASSERT( sizeof(long) == sizeof(signed long) );

typedef unsigned char BYTE;
C_ASSERT( sizeof(BYTE) == 1 );

typedef signed char SBYTE;
C_ASSERT( sizeof(SBYTE) == 1 );

typedef unsigned short WORD;
C_ASSERT( sizeof(WORD) == 2 );

typedef signed short SWORD;
C_ASSERT( sizeof(SWORD) == 2 );

typedef signed int INT;
C_ASSERT( sizeof(INT) == 4 );

typedef unsigned int UINT;
C_ASSERT( sizeof(UINT) == 4 );

typedef unsigned long DWORD;
C_ASSERT( sizeof(DWORD) == 4 );

typedef signed long LONG;
C_ASSERT( sizeof(LONG) == 4 );

typedef unsigned long ULONG;
C_ASSERT( sizeof(ULONG) == 4 );

typedef unsigned __int64 DWORDLONG;
C_ASSERT( sizeof(DWORDLONG) == 8 );

typedef float FLOAT;
C_ASSERT( sizeof(FLOAT) == 4 );

typedef double DOUBLE;
C_ASSERT( sizeof(DOUBLE) == 8 );

C_ASSERT( sizeof(BYTE) == sizeof(SBYTE) );
C_ASSERT( sizeof(WORD) == sizeof(SWORD) );
C_ASSERT( sizeof(INT) == sizeof(UINT) );
C_ASSERT( sizeof(INT) == sizeof(LONG) );
C_ASSERT( sizeof(LONG) == sizeof(ULONG) );
C_ASSERT( sizeof(UINT) == sizeof(DWORD) );
C_ASSERT( sizeof(ULONG) == sizeof(DWORD) );

#ifndef ELLISYS_KEEP_EXISTING_BYTE_MACROS
// Undefine any previous definition of LOBYTE, HIBYTE, MAKEWORD, etc
#	undef LOBYTE
#	undef HIBYTE
#	undef LOWORD
#	undef HIWORD
#	undef MAKEWORD
#	undef MAKEDWORD
#endif // ELLISYS_KEEP_EXISTING_BYTE_MACROS

// The following macro is used to ensure a parameter can be converted to a DWORD value.
// It is unconditionally truncated to a DWORD value.
// This is because some Microsoft's headers may call our macros with LPCTSTR parameters.
#define ENSURE_DWORD(dw) ((DWORD_PTR) (((DWORD_PTR) (dw)) /*& 0xFFFFFFFF*/))

#ifndef LOBYTE
#	define LOBYTE(w) ((BYTE) (ENSURE_DWORD(w) & 0xFF))
#endif
C_ASSERT( LOBYTE(0) == 0 );
C_ASSERT( LOBYTE(0x12) == 0x12 );
C_ASSERT( LOBYTE(0xABCD) == 0xCD );
C_ASSERT( LOBYTE(0x123456) == 0x56 );
C_ASSERT( LOBYTE(0xFFFFFFFC) == 0xFC );
C_ASSERT( LOBYTE(-1) == (BYTE) -1 );

#ifndef HIBYTE
#	define HIBYTE(w) ((BYTE) ((ENSURE_DWORD(w) >> 8) & 0xFF))
#endif
C_ASSERT( HIBYTE(0) == 0 );
C_ASSERT( HIBYTE(0x12) == 0x00 );
C_ASSERT( HIBYTE(0xABCD) == 0xAB );
C_ASSERT( HIBYTE(0x123456) == 0x34 );
C_ASSERT( HIBYTE(0xFFFFFAFC) == 0xFA );
C_ASSERT( HIBYTE(-1) == (BYTE) -1 );

#ifndef LOWORD
#	define LOWORD(dw) ((WORD) (ENSURE_DWORD(dw) & 0xFFFF))
#endif
C_ASSERT( LOWORD(0) == 0 );
C_ASSERT( LOWORD(0x12) == 0x0012 );
C_ASSERT( LOWORD(0xABCD) == 0xABCD );
C_ASSERT( LOWORD(0x123456) == 0x3456 );
C_ASSERT( LOWORD(0xFFFFFFFC) == 0xFFFC );
C_ASSERT( LOWORD(-1) == (WORD) -1 );

#ifndef HIWORD
#	define HIWORD(dw) ((WORD) ((ENSURE_DWORD(dw) >> 16) & 0xFFFF))
#endif
C_ASSERT( HIWORD(0) == 0 );
C_ASSERT( HIWORD(0x12) == 0x0000 );
C_ASSERT( HIWORD(0xABCD) == 0x0000 );
C_ASSERT( HIWORD(0x123456) == 0x0012 );
C_ASSERT( HIWORD(0xF234FAFC) == 0xF234 );
C_ASSERT( HIWORD(-1) == (WORD) -1 );

#ifndef MAKEWORD
#	define MAKEWORD(lo, hi) ((WORD) (LOBYTE(lo) | (LOBYTE(hi) << 8)))
#endif
C_ASSERT( MAKEWORD(0, 0) == 0 );
C_ASSERT( MAKEWORD(0x34, 0x12) == 0x1234 );
C_ASSERT( MAKEWORD(0xABCD, 0x1234) == 0x34CD );
C_ASSERT( MAKEWORD(0, 0x1234) == 0x3400 );
C_ASSERT( MAKEWORD(0x5678, 0) == 0x0078 );
C_ASSERT( MAKEWORD(-1, -1) == (WORD) -1 );
C_ASSERT( MAKEWORD(LOBYTE(0xDECA), HIBYTE(0xCAFE)) == 0xCACA );

#ifndef MAKEDWORD
#	define MAKEDWORD(lo, hi) ((DWORD) (LOWORD(lo) | (LOWORD(hi) << 16)))
#endif
C_ASSERT( MAKEDWORD(0, 0) == 0 );
C_ASSERT( MAKEDWORD(0x34, 0x12) == 0x00120034 );
C_ASSERT( MAKEDWORD(0xABCD, 0x1234) == 0x1234ABCD );
C_ASSERT( MAKEDWORD(0, 0x1234) == 0x12340000 );
C_ASSERT( MAKEDWORD(0x5678, 0) == 0x00005678 );
C_ASSERT( MAKEDWORD(0xABCDEF5A, 0x12345678) == 0x5678EF5A);
C_ASSERT( MAKEDWORD(-1, -1) == (DWORD) -1 );
C_ASSERT( MAKEDWORD(-1, 0) == (DWORD) (WORD) -1 );
C_ASSERT( MAKEDWORD(LOWORD(0xBEBECACA), HIWORD(0xCAFEDECA)) == 0xCAFECACA );

#ifndef min
#	define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
C_ASSERT( min(0, 0) == 0 );
C_ASSERT( min(0, -1) == -1 );
C_ASSERT( min(-1, 0) == -1 );
C_ASSERT( min(-1, -1) == -1 );
C_ASSERT( min(0, 1) == 0 );
C_ASSERT( min(1, 0) == 0 );
C_ASSERT( min(1, 2) == 1 );
C_ASSERT( min(2, 1) == 1 );
C_ASSERT( min(0xFF, 0) == 0 );
C_ASSERT( min(0xFFFF, 0) == 0 );
C_ASSERT( min(0xFFFFFFFF, 0) == 0 );
C_ASSERT( min(0xFFFFFFFFFFFFFFFF, 0) == 0 );
C_ASSERT( min(67848, 98792) == 67848 );

#ifndef max
#	define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
C_ASSERT( max(0, 0) == 0 );
C_ASSERT( max(0, -1) == 0 );
C_ASSERT( max(-1, 0) == 0 );
C_ASSERT( max(-1, -1) == -1 );
C_ASSERT( max(0, 1) == 1 );
C_ASSERT( max(1, 0) == 1 );
C_ASSERT( max(1, 2) == 2 );
C_ASSERT( max(2, 1) == 2 );
C_ASSERT( max(0xFF, 0) == 0xFF );
C_ASSERT( max(0xFFFF, 0) == 0xFFFF );
C_ASSERT( max(0xFFFFFFFF, 0) == 0xFFFFFFFF );
C_ASSERT( max(0xFFFFFFFFFFFFFFFF, 0) == 0xFFFFFFFFFFFFFFFF );
C_ASSERT( max(67848, 98792) == 98792 );

#define countof(x) (sizeof(x) / sizeof((x)[0]))
C_ASSERT( sizeof ( "ABC") == 4);
C_ASSERT( countof( "ABC") == 4);
C_ASSERT( sizeof (L"ABC") == 8);
C_ASSERT( countof(L"ABC") == 4);

// Char-width neutral typedefs for STL classes
#ifdef _UNICODE
#	define tstring wstring
#	define tcin wcin
#	define tcout wcout
#	define tcerr wcerr
#	define tclog wclog
#	define tostream wostream
#	define tofstream wofstream
#	define tistream wistream
#	define tifstream wifstream
#	define tstrcmp wcscmp
#	define tstrcpy wcscpy
#else
#	define tstring string
#	define tcin cin
#	define tcout cout
#	define tcerr cerr
#	define tclog clog
#	define tostream ostream
#	define tofstream ofstream
#	define tistream istream
#	define tifstream ifstream
#	define tstrcmp strcmp
#	define tstrcpy strcpy
#endif

#ifndef ELLISYS_DISABLE_WARNINGS
#	pragma warning(disable:4100) // Unreferenced formal parameter
#endif

#endif // __STANDARDELLISYS_H__
