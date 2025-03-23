#pragma once
#include <Windows.h>
#include <stdio.h>
#include <intsafe.h>
#include <ImageHlp.h>
#include "myntdll.h"

#define FlagOn( Flags, TheFlag )     ( (Flags) & (TheFlag) )

#define ERROR_INVALID_PARAMETER_1 ((DWORD)0xc00000efUL)
#define ERROR_INVALID_PARAMETER_2 ((DWORD)0xc00000f0UL)
#define ERROR_INVALID_PARAMETER_3 ((DWORD)0xc00000f1UL)


#define RVA2VA( Type, Base, Rva ) ( Type )( (DWORD_PTR)(Base) + (DWORD_PTR)(Rva) )


//
// macro to convert to uppercase
//
#define upcase(C) (WCHAR )(((C) >= 'a' && (C) <= 'z' ? (C) - ('a' - 'A') : (C)))


#define MY_RTL_CONSTANT_STRINGA(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PSTR)s  }
#define MY_RTL_CONSTANT_STRINGW(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PWSTR)s }


#ifdef UNICODE
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGW
#else
#define MY_RTL_CONSTANT_STRING MY_RTL_CONSTANT_STRINGA
#endif


#define LDRP_HASH_TABLE_SIZE	32
#define LDRP_HASH_MASK			(LDRP_HASH_TABLE_SIZE-1)


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
	_In_ const PUNICODE_STRING String,
	_In_ BOOLEAN bCaseSensitivity,
	_In_ ULONG HashingAlgorithm,
	_Out_ PULONG HashedValue
);



/// <summary>
/// 
/// </summary>
/// <param name="ModuleName"></param>
/// <param name="ModuleNameHash"></param>
/// <returns></returns>
_Must_inspect_result_
DWORD
WINAPI
FindSystemModule(
	_In_opt_ const PUNICODE_STRING ModuleName,
	_When_(ModuleName != NULL, _In_opt_) _When_(ModuleName == NULL, _In_) const PULONG ModuleNameHash
);



/// <summary>
/// 
/// </summary>
/// <param name="PebPtr"></param>
/// <returns></returns>
_Must_inspect_result_
DWORD
WINAPI
GetPebPtr(
	_Out_ PPEB* PebPtr
);



/// <summary>
/// 
/// </summary>
/// <param name="PebPtr"></param>
/// <param name="ModuleName"></param>
/// <param name="ModuleNameHash"></param>
/// <param name="bHashOnly"></param>
/// <returns></returns>
_Must_inspect_result_
DWORD
WINAPI
GetModuleBase(
	_In_ const PUNICODE_STRING ModuleName,
	_In_ const PULONG ModuleNameHash,
	_In_ const BOOLEAN bHashOnly,
	_Out_ PVOID* ModuleBase
);



_Ret_maybenull_
PIMAGE_DOS_HEADER
WINAPI
GetDosHeader(
	_In_ PVOID ModuleBase
);




_Ret_maybenull_
PIMAGE_NT_HEADERS
WINAPI
GetNtHeader(
	_In_ PVOID ModuleBase
);



_Ret_maybenull_
PIMAGE_DATA_DIRECTORY
WINAPI
GetDataDirectory(
	DWORD DirectoryIndex
);



_Ret_maybenull_
PIMAGE_FILE_HEADER
WINAPI
GetFileHeader();



BOOLEAN
WINAPI
Is64Bit();



_Ret_maybenull_
PIMAGE_EXPORT_DIRECTORY
WINAPI
GetExports();



_Ret_maybenull_
PVOID
WINAPI
GetAddress(
	ULONG Rva
);



DWORD
WINAPI
ConvertRvaToFileOffset(
	DWORD Rva
);



VOID
WINAPI
DumpDosHeader();



VOID
WINAPI
DumpNtHeader();



VOID
WINAPI
DumpOptHeader();



VOID
WINAPI
DumpFileHeader();



VOID
WINAPI
DumpImports();



VOID
WINAPI
DumpExports();



VOID
WINAPI
DumpIAT();
