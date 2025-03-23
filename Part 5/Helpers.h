#pragma once
#include <Windows.h>
#include "myntdll.h"

#define MY_RTL_CONSTANT_STRINGA(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PSTR)s  }
#define MY_RTL_CONSTANT_STRINGW(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PWSTR)s }

#ifdef UNICODE
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGW
#else
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGA
#endif

typedef struct _MY_UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} MY_UNICODE_STRING, * PMY_UNICODE_STRING;


typedef struct _STRING_EX
{
	USHORT Length;
	USHORT MaximumLength;
#ifdef UNICODE
	PWSTR  Buffer;
#else
	PSTR   Buffer;
#endif 

} STRING_EX, * PSTRING_EX;


/// <summary>
/// The LdrpHashUnicodeString function has been REd and source code has been read for this implementation
/// </summary>
/// <param name="String">The UNICODE_STRING to hash</param>
/// <param name="bCaseSensitivity">Should this be case insensitive</param>
/// <param name="HashingAlgorithm">What hash algorithm to choose</param>
/// <param name="HashedValue">Pointer to the calculated hashed value</param>
/// <returns>Returns ERROR_SUCCESS on success</returns>
DWORD
WINAPI
LdrpHashUnicodeString(
	_In_ const PSTRING_EX String,
	_In_ BOOLEAN bCaseSensitivity,
	_In_ ULONG HashingAlgorithm,
	_Outptr_ PULONG HashedValue
);