#include <Windows.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <intrin.h>
#include "Helpers.h"

#define __cdecl CDECL

DWORD
IntrinsicHasher(PUNICODE_STRING String)
{
    DWORD dwHash = 0UL;
    __m128i hash = _mm_setzero_si128();
    DWORD length = String->Length / sizeof(WCHAR);
    DWORD i = 0;

    for (; i + 8 <= length; i += 8)
    {
        __m128i chunk = _mm_loadu_si128((__m128i*) & String->Buffer[i]);
        hash = _mm_xor_si128(hash, chunk);
    }

    DWORD dwParts[4];
    _mm_storeu_si128((__m128i*)dwParts, hash);
    dwHash = dwParts[0] ^ dwParts[1] ^ dwParts[2] ^ dwParts[3];

    for (; i < length; ++i)
    {
        dwHash ^= String->Buffer[i];
    }

    return dwHash;
}

DWORD
CDECL
start()
{
	DWORD retval = ERROR_SUCCESS;
	
    // create a stack string
    WCHAR chars[] = { 'K','E','R','N','E','L','B','A','S','E','.','d','l','l',UNICODE_NULL };
    // init the string
    UNICODE_STRING kBase = MY_RTL_CONSTANT_STRINGW(chars);
    // make the hash
    DWORD dwHash = IntrinsicHasher(&kBase);

    retval = IntrinsicHasher(&kBase);

	return retval;
}