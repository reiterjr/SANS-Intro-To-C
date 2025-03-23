#include "Errors.h"

#define BUFFER_SIZE 1024



_Use_decl_annotations_
VOID
WINAPI
CheckLastInetError(
	LPCWSTR lpMessage,
	DWORD dwLastErr
)
{
	DWORD dwBaseLen = 0UL;
	WCHAR szFormatBuffer[BUFFER_SIZE];

	dwBaseLen = FormatMessageW(
		FORMAT_MESSAGE_FROM_HMODULE,
		GetModuleHandleW(L"wininet.dll"),
		dwLastErr,
		0UL,
		szFormatBuffer,
		BUFFER_SIZE,
		NULL
	);

	wprintf(L"%s: %s \n", lpMessage, szFormatBuffer);

}