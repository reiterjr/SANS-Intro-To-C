/*
This header file defines a function and a structure for enumerating files in a directory on Windows.
It includes necessary headers and defines a macro for checking flags. The main components are:

1. DirSearch: A function prototype for enumerating files in a directory. It takes a parameter of type LPVOID,
   which is a pointer to any type, allowing flexibility in the data passed to the function.
2. ARGS Structure: A structure used to pass arguments to the DirSearch function. It contains a flexible array member
   for storing a file name.

The file also includes a macro for checking if a specific flag is set within a set of flags.
*/

#pragma once

#include <Windows.h>
#include <stdio.h>

#define FlagIsOn(Flags, TheFlag) ((Flags) & (TheFlag))

/// <summary>
/// enumerates a folder
/// </summary>
/// <returns>0 upon success</returns>
DWORD
WINAPI
DirSearch(_In_ LPVOID lpParam);

typedef struct _ARGS
{
	CHAR FileName[ANYSIZE_ARRAY];
} ARGS, *PARGS;
