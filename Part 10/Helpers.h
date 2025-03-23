#pragma once
#include <Windows.h>

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximimLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

#define MY_RTL_CONSTANT_STRINGA(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PSTR)s	 }
#define MY_RTL_CONSTANT_STRINGW(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PWSTR)s }

#ifdef UNICODE
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGW
#else
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGA
#endif
