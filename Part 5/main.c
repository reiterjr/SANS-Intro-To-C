#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "myntdll.h"
#include "Helpers.h"

#define __cdecl CDECL
#define PEB_PTR_OFFSET 0x60

#ifdef _WIN64
#pragma intrinsic(__readgsqword)
#else
#error "Cannot compile for x86 configuration"
// add readfsdword and move readgsword to ifdef
#endif

INT
CDECL
main()
{
	// grabs PEB offset from intrinsic
	PPEB PebPtr = (PPEB)__readgsqword(FIELD_OFFSET(TEB, ProcessEnvironmentBlock));

	// safety and opsec
	EnterCriticalSection(PebPtr->LoaderLock);

	// pointer to PEB loader data
	PPEB_LDR_DATA LdrDataPtr = (PPEB_LDR_DATA)PebPtr->Ldr;

	// contains only f/blink
	PLIST_ENTRY HeadList = &(LdrDataPtr->InMemoryOrderModuleList);
	PLIST_ENTRY CurrentModule = HeadList->Flink;

	// hashing
	STRING_EX strNtdll = MY_RTL_CONSTANT_STRING(L"ntdll.dll");
	DWORD HashedValue = 0UL;
	LdrpHashUnicodeString(&strNtdll, TRUE, 0, &HashedValue);

	wprintf(L"the hash of ntdll.dll is 0x%08x\n", HashedValue);

	// enumerates through PEB doubly linked lists
	while (HeadList != CurrentModule)
	{
		// grabs value of LDR_DATA_TABLE_ENTRY
		PLDR_DATA_TABLE_ENTRY LdrDataEntry = CONTAINING_RECORD(CurrentModule, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		// prints dll names, base addresses, and base address 4B hash
		wprintf(L"DllName: %wZ, BaseAddress 0x%p, hashValue: 0x%08x\n",
			LdrDataEntry->BaseDllName,
			LdrDataEntry->DllBase,
			LdrDataEntry->BaseNameHashValue
		);

		CurrentModule = CurrentModule->Flink;
	}

	// safety and opsec
	LeaveCriticalSection(PebPtr->LoaderLock);

	// not removed from stack until overwritten
	PebPtr = NULL;
	LdrDataPtr = NULL;
	HeadList = NULL;
	CurrentModule = NULL;

	return ERROR_SUCCESS;
}