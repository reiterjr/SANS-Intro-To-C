#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winnt.h>


#include "../Part 5/ntdll.h"

#define __cdecl CDECL
#define PEB_PTR_OFFSET 0x60

INT
CDECL
main()
{
	PPEB PebPtr = (PPEB)__readgsqword(PEB_PTR_OFFSET);

	return ERROR_SUCCESS;
}