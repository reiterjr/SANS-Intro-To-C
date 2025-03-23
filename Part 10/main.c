#include <Windows.h>
#include "Helpers.h"


//------------------------------------------------------------
//-----------       Created with 010 Editor        -----------
//------         www.sweetscape.com/010editor/          ------
//
// File    : D:\C_C++ bootcamp\C_C++ bootcamp\x64\Release\Part 10.exe
// Address : 1024 (0x400)
// Size    : 600 (0x258)
//------------------------------------------------------------

unsigned char hexData[600] = {
    0x48, 0x83, 0xEC, 0x08, 0x44, 0x0F, 0xB7, 0x09, 0x48, 0x8D, 0x51, 0x08, 0x41, 0xD1, 0xE9, 0x33,
    0xC0, 0x4C, 0x8B, 0xD9, 0x0F, 0x57, 0xC9, 0x41, 0x83, 0xF9, 0x08, 0x72, 0x17, 0x4C, 0x8B, 0x02,
    0xF3, 0x41, 0x0F, 0x6F, 0x04, 0x40, 0x83, 0xC0, 0x08, 0x0F, 0x57, 0xC8, 0x8D, 0x48, 0x08, 0x41,
    0x3B, 0xC9, 0x76, 0xEC, 0x66, 0x0F, 0x6F, 0xC1, 0x66, 0x41, 0x0F, 0x7E, 0xC8, 0x66, 0x0F, 0x73,
    0xD8, 0x04, 0x66, 0x0F, 0x7E, 0xC1, 0x66, 0x0F, 0x6F, 0xC1, 0x66, 0x0F, 0x73, 0xD8, 0x08, 0x66,
    0x0F, 0x73, 0xD9, 0x0C, 0x44, 0x33, 0xC1, 0x66, 0x0F, 0x7E, 0xC1, 0x44, 0x33, 0xC1, 0x66, 0x0F,
    0x7E, 0xC9, 0x44, 0x33, 0xC1, 0x41, 0x8B, 0xC9, 0x2B, 0xC8, 0x41, 0x3B, 0xC1, 0x0F, 0x83, 0x9F,
    0x00, 0x00, 0x00, 0x83, 0xF9, 0x08, 0x72, 0x68, 0x83, 0xE1, 0x07, 0x48, 0x89, 0x1C, 0x24, 0x48,
    0x8B, 0x1A, 0x45, 0x8B, 0xD1, 0x44, 0x2B, 0xD1, 0x0F, 0x57, 0xDB, 0x0F, 0x57, 0xD2, 0x66, 0x90,
    0xF3, 0x0F, 0x7E, 0x0C, 0x43, 0x8D, 0x48, 0x04, 0x0F, 0x57, 0xC0, 0x83, 0xC0, 0x08, 0x66, 0x0F,
    0x61, 0xC8, 0x0F, 0x57, 0xD9, 0xF3, 0x0F, 0x7E, 0x0C, 0x4B, 0x66, 0x0F, 0x61, 0xC8, 0x0F, 0x57,
    0xD1, 0x41, 0x3B, 0xC2, 0x72, 0xDA, 0x48, 0x8B, 0x1C, 0x24, 0x49, 0x8D, 0x53, 0x08, 0x0F, 0x57,
    0xD3, 0x66, 0x0F, 0x6F, 0xC2, 0x66, 0x0F, 0x73, 0xD8, 0x08, 0x0F, 0x57, 0xD0, 0x66, 0x0F, 0x6F,
    0xC2, 0x66, 0x0F, 0x73, 0xD8, 0x04, 0x0F, 0x57, 0xD0, 0x66, 0x0F, 0x7E, 0xD1, 0x44, 0x33, 0xC1,
    0x41, 0x3B, 0xC1, 0x73, 0x2D, 0x4C, 0x8B, 0x12, 0x44, 0x2B, 0xC8, 0x48, 0x8D, 0x14, 0x00, 0x41,
    0x8B, 0xC1, 0x0F, 0x1F, 0x40, 0x00, 0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x0F, 0xB7, 0x0C, 0x12, 0x48, 0x8D, 0x52, 0x02, 0x44, 0x33, 0xC1, 0x48, 0x83, 0xE8, 0x01,
    0x75, 0xEE, 0x41, 0x8B, 0xC0, 0x48, 0x83, 0xC4, 0x08, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
    0x48, 0x83, 0xEC, 0x28, 0x33, 0xC0, 0xC7, 0x04, 0x24, 0x4B, 0x00, 0x45, 0x00, 0x66, 0x89, 0x44,
    0x24, 0x1C, 0x0F, 0x57, 0xC9, 0xC7, 0x44, 0x24, 0x04, 0x52, 0x00, 0x4E, 0x00, 0xC7, 0x44, 0x24,
    0x08, 0x45, 0x00, 0x4C, 0x00, 0xC7, 0x44, 0x24, 0x0C, 0x42, 0x00, 0x41, 0x00, 0xC7, 0x44, 0x24,
    0x10, 0x53, 0x00, 0x45, 0x00, 0xC7, 0x44, 0x24, 0x14, 0x2E, 0x00, 0x44, 0x00, 0xC7, 0x44, 0x24,
    0x18, 0x4C, 0x00, 0x4C, 0x00, 0x66, 0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xF3, 0x0F, 0x6F, 0x04, 0x44, 0x83, 0xC0, 0x08, 0x0F, 0x57, 0xC1, 0x66, 0x0F, 0x6F, 0xC8, 0x8D,
    0x48, 0x08, 0x83, 0xF9, 0x0E, 0x76, 0xE9, 0x66, 0x0F, 0x73, 0xD8, 0x04, 0x41, 0xB9, 0x0E, 0x00,
    0x00, 0x00, 0x66, 0x0F, 0x7E, 0xC1, 0x66, 0x0F, 0x6F, 0xC1, 0x66, 0x41, 0x0F, 0x7E, 0xC8, 0x66,
    0x0F, 0x73, 0xD8, 0x08, 0x66, 0x0F, 0x73, 0xD9, 0x0C, 0x44, 0x33, 0xC1, 0x66, 0x0F, 0x7E, 0xC1,
    0x44, 0x33, 0xC1, 0x66, 0x0F, 0x7E, 0xC9, 0x44, 0x33, 0xC1, 0x41, 0x8B, 0xC9, 0x2B, 0xC8, 0x41,
    0x3B, 0xC1, 0x0F, 0x83, 0x88, 0x00, 0x00, 0x00, 0x83, 0xF9, 0x08, 0x72, 0x60, 0x83, 0xE1, 0x07,
    0x45, 0x8B, 0xD1, 0x44, 0x2B, 0xD1, 0x0F, 0x57, 0xDB, 0x0F, 0x57, 0xD2, 0x0F, 0x1F, 0x40, 0x00,
    0xF3, 0x0F, 0x7E, 0x0C, 0x44, 0x8D, 0x48, 0x04, 0x0F, 0x57, 0xC0, 0x83, 0xC0, 0x08, 0x66, 0x0F,
    0x61, 0xC8, 0x0F, 0x57, 0xD9, 0xF3, 0x0F, 0x7E, 0x0C, 0x4C, 0x66, 0x0F, 0x61, 0xC8, 0x0F, 0x57,
    0xD1, 0x41, 0x3B, 0xC2, 0x72, 0xDA, 0x0F, 0x57, 0xD3, 0x66, 0x0F, 0x6F, 0xC2, 0x66, 0x0F, 0x73,
    0xD8, 0x08, 0x0F, 0x57, 0xD0, 0x66, 0x0F, 0x6F, 0xC2, 0x66, 0x0F, 0x73, 0xD8, 0x04, 0x0F, 0x57,
    0xD0, 0x66, 0x0F, 0x7E, 0xD1, 0x44, 0x33, 0xC1, 0x41, 0x3B, 0xC1, 0x73, 0x23, 0x48, 0x8D, 0x14,
    0x24, 0x44, 0x2B, 0xC8, 0x48, 0x8D, 0x14, 0x42, 0x41, 0x8B, 0xC9, 0x0F, 0x1F, 0x44, 0x00, 0x00,
    0x0F, 0xB7, 0x02, 0x48, 0x8D, 0x52, 0x02, 0x44, 0x33, 0xC0, 0x48, 0x83, 0xE9, 0x01, 0x75, 0xF0,
    0x41, 0x8B, 0xC0, 0x48, 0x83, 0xC4, 0x28, 0xC3
};


INT
inject(VOID);

INT
main(VOID)
{
    return inject();
}

INT
inject(VOID)
{
    // make memory
    PUCHAR pBuffer = (PUCHAR)VirtualAlloc(
        NULL,
        sizeof(hexData),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READ
    );

    if (!pBuffer)
    {
        printf("%s: VirtualAlloc failed\n", __FUNCTION__);
        return GetLastError();
    }

    SIZE_T bytesWritten = 0;

    if (!WriteProcessMemory(
        GetCurrentProcess(),
        pBuffer,
        hexData,
        sizeof(hexData),
        &bytesWritten
    ))
    {
        printf("%s: WriteProcessMemory failed\n", __FUNCTION__);
        return GetLastError();
    }

    printf("%s: write %I64u bytes to 0x%p\n", __FUNCTION__, bytesWritten, pBuffer);

    HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pBuffer, NULL, 0, NULL);
    if (!thread)
    {
        printf("%s: CreateThread failed\n", __FUNCTION__);
        return GetLastError();
    }

    WaitForSingleObject(thread, INFINITE);

    DWORD dwExitThread = 0UL;
    GetExitCodeThread(thread, &dwExitThread);

    printf("%s: thread exited with %lu (0x%x)\n", __FUNCTION__, dwExitThread, dwExitThread);

    return ERROR_SUCCESS;
}