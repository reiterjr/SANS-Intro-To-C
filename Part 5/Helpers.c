#include "Helpers.h"
#include "myntdll.h"

#define upcase(C) (WCHAR )(((C) >= 'a' && (C) <= 'z' ? (C) - ('a' - 'A') : (C)))

_Use_decl_annotations_
DWORD
WINAPI
LdrpHashUnicodeString(
	const PSTRING_EX String,
	BOOLEAN bCaseSensitivity,
	ULONG HashingAlgorithm,
	PULONG HashedValue
)
{
	ULONG TempHashVal = 0UL;
	ULONG Chars = 0UL;
	PCWSTR Buffer = NULL;

	Buffer = String->Buffer;
	*HashedValue = 0UL;
	Chars = String->Length / sizeof(WCHAR);

	if (bCaseSensitivity)
	{
		while (Chars-- != 0)
		{
			WCHAR Char = *Buffer++;
			TempHashVal = (TempHashVal * 65599) + upcase(Char);
		}
	}
	else
	{
		while (Chars-- != 0)
		{
			TempHashVal = (TempHashVal * 65599) + *Buffer++;
		}
	}

	*HashedValue = TempHashVal;
	return ERROR_SUCCESS;
}
