#include <Windows.h>
#include <stdio.h>

#define __cdecl CDECL
#define COUNTOF_NAMES 4

/// <summary>
/// the procedure obtained the PID of self
/// </summary>
/// <param name=""></param>
/// <returns>DWORD of PID</returns>
DWORD
WINAPI
MyGetCurrentProcId(VOID);

FARPROC
WINAPI
MyGetProcAddress();

HANDLE
WINAPI
MyLoadLibraryA();

BOOL
WINAPI
MyEnumProcesses();

INT
CDECL
main(VOID)
{
	printf("[#] %s:%d this program was compuled on %s %s\n", __FUNCTION__, __LINE__, __DATE__, __TIME__);

	ULONG ulCourse = 670UL;
	PULONG pCourse = &ulCourse;

	printf("[#] %s:%d the address of ulCourse is 0x%p\n", __FUNCTION__, __LINE__, pCourse);
	printf("[#] %s:%d after deref pCourse, the value is %lu\n", __FUNCTION__, __LINE__, *pCourse);

	PCHAR AddressOfNames[COUNTOF_NAMES] = { "MyGetCurrentProcId", "MyGetProcAddress", "MyLoadLibraryA", "MyEnumProcesses" };
	PVOID AddressOfFunctions[COUNTOF_NAMES] = { MyGetCurrentProcId, MyGetProcAddress, MyLoadLibraryA, MyEnumProcesses };
	ULONG AddressOfNameOrdinals[COUNTOF_NAMES] = { 0, 1, 2, 3 };

	for (ULONG i = 0UL; i < COUNTOF_NAMES; i++)
	{
		if (0 == strcmp("MyLoadLibraryA", AddressOfNames[i]))
		{
			printf("[+] found the procedure at index %lu\n", i);
			break;
		}
	}

	return ERROR_SUCCESS;
}

DWORD
WINAPI
MyGetCurrentProcId(VOID)
{
	return ERROR_SUCCESS;
}

FARPROC
WINAPI
MyGetProcAddress()
{
	return NULL;
}

HANDLE
WINAPI
MyLoadLibraryA()
{
	return INVALID_HANDLE_VALUE;
}

BOOL
WINAPI
MyEnumProcesses()
{
	return TRUE;
}




/*
* 	printf("---> %s \n", __FUNCTION__);
* 
*	printf("<--- %s \n", __FUNCTION__);
* 
* 	printf("\n");
*/