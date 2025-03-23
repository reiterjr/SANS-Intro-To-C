// directory walker
#include <Windows.h>
#include <stdio.h> // for debugging (printf)
#include "dirutils.h"
#include <intrin.h>

#define __cdecl CDECL

// inline vs stack frame
#pragma intrinsic(memcpy)
INT
CDECL
main(VOID)
{
	DWORD dwLastError = ERROR_SUCCESS;
	HANDLE hThread =	NULL;
	
	// allocating chunk in process heap with error checking
	PARGS pArgs = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ARGS));

	if (!pArgs)
	{
		return GetLastError;
	}

	// copies into the memory array on the heap
	memcpy(pArgs->FileName, "C:\\Users\\*", strlen("C:\\Users\\*"));

	// dwLastError = DirSearch();
	// hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DirSearch, NULL, 0, NULL);
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DirSearch, pArgs, 0, NULL);
	if (!hThread)
	{
		return GetLastError();
	}

	WaitForSingleObject(hThread, INFINITE);

	// Close the handle, free the heap, and then zeroize it
	CloseHandle(hThread);
	if (pArgs)
	{
		HeapFree(GetProcessHeap(), 0, pArgs);
		pArgs = NULL;
	}

	return dwLastError;
}