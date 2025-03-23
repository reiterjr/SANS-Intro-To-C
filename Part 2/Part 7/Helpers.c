#include "Helpers.h"
#include "myntdll.h"

#include <strsafe.h>

#pragma comment(lib, "imagehlp.lib")

#define PWINAPI WINAPI*

typedef DWORD(PWINAPI GET_CURRENT_PROCESS_ID)(void);

typedef struct _PE_INFO {
	IMAGE_DOS_HEADER ImageDosHdr;
	IMAGE_FILE_HEADER ImageFileHdr;
	DWORD Signature;
	union nameless
	{
		IMAGE_OPTIONAL_HEADER32 ImageOptHdr32;
		IMAGE_OPTIONAL_HEADER64 ImageOptHdr64;
	} OptionalHdr;
} PE_INFO, * PPE_INFO;

PE_INFO g_PeInfo = { 0 };

DWORD g_Offset = 0UL;
PVOID g_ModuleBase = NULL;
PIMAGE_DOS_HEADER g_pImgDosHeader = NULL;
PIMAGE_NT_HEADERS32 g_pImgNt32 = NULL;
PIMAGE_NT_HEADERS64 g_pImgNt64 = NULL;
PIMAGE_FILE_HEADER g_pImgFileHeader = NULL;
PIMAGE_SECTION_HEADER g_pImgSections = NULL;
PIMAGE_OPTIONAL_HEADER32 g_pImgOpt32 = NULL;
PIMAGE_OPTIONAL_HEADER64 g_pImgOpt64 = NULL;
PIMAGE_EXPORT_DIRECTORY g_pImgExports = NULL;
PIMAGE_DATA_DIRECTORY g_pImgDataDir = NULL;


_Use_decl_annotations_
DWORD
WINAPI
LdrpHashUnicodeString(
	const PUNICODE_STRING String,
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


_Use_decl_annotations_
DWORD
WINAPI
FindSystemModule(
	const PUNICODE_STRING ModuleName,
	const PULONG ModuleNameHash
)
{
	DWORD dwLastError = ERROR_SUCCESS;
	BOOLEAN bHashOnly = FALSE;
	PVOID ModuleBase = NULL;

#ifdef _DEBUG
	printf("%s:%s:%d checking params \n", __FILE__, __FUNCTION__, __LINE__);
#endif // _DEBUG

	//
	// check the params
	//
	// since both are kind of marked _opt_ then they both must be checked for NULL ptr
	//
	if (NULL == ModuleName)
	{
#ifdef _DEBUG
		printf("%s:%s:%d no module name \n", __FILE__, __FUNCTION__, __LINE__);
#endif // _DEBUG

		//
		// if modulename isn't present then the hash must be supplied
		// 
		if (NULL == ModuleNameHash)
		{
#ifdef _DEBUG
			printf("%s:%s:%d no hash value \n", __FILE__, __FUNCTION__, __LINE__);
#endif // _DEBUG
			return ERROR_INVALID_PARAMETER;
		}

		//
		// the hash must be there
		//
		bHashOnly = TRUE;
	}


#ifdef _DEBUG
	if (ModuleName) wprintf(L"%S:%s:%d ModuleName: %wZ \n", __FILE__, __FUNCTIONW__, __LINE__, ModuleName);
	if (ModuleNameHash) wprintf(L"%S:%s:%d ModuleNameHash: 0x%08x \n", __FILE__, __FUNCTIONW__, __LINE__, *ModuleNameHash);
#endif // _DEBUG


	//dwLastError = GetModuleBase(ModuleName, ModuleNameHash, bHashOnly, &ModuleBase);
	dwLastError = GetModuleBase(ModuleName, ModuleNameHash, bHashOnly, &g_ModuleBase);
	if (ERROR_SUCCESS != dwLastError)
	{
		return dwLastError;
	}


	//PIMAGE_DOS_HEADER pImgDos = GetDosHeader(ModuleBase);
	g_pImgDosHeader = GetDosHeader(g_ModuleBase);
	//PIMAGE_NT_HEADERS pImgNt = GetNtHeader(ModuleBase);
	g_pImgFileHeader = GetFileHeader();
	g_pImgExports = GetExports();

	DumpDosHeader();
	DumpFileHeader();
	DumpOptHeader();
	DumpExports();

	//
	// optional, clean up global variables once completely done with them
	// eg. NULL them all out 
	//


#ifdef _DEBUG
	printf("%s:%s:%d Done! \n", __FILE__, __FUNCTION__, __LINE__);
#endif // _DEBUG

	return dwLastError;
}


FARPROC
WINAPI
ObtainProcedureAddress()
{
	return NULL;
}


_Use_decl_annotations_
DWORD
WINAPI
GetPebPtr
(
	PPEB* PebPtr
)
{
	if (!PebPtr)
	{
		return ERROR_INVALID_PARAMETER_1;
	}
	//
	// obtain a pointer to this process' PEB
	//
	PPEB pPebPtr = (PPEB)__readgsqword(FIELD_OFFSET(TEB, ProcessEnvironmentBlock));
	*PebPtr = pPebPtr;

#ifdef _DEBUG
	printf("%s:%s:%d PPEB: 0x%p \n", __FILE__, __FUNCTION__, __LINE__, PebPtr);
#endif // _DEBUG

	return ERROR_SUCCESS;
}


_Use_decl_annotations_
DWORD
WINAPI
GetModuleBase(
	PUNICODE_STRING ModuleName,
	PULONG ModuleNameHash,
	BOOLEAN bHashOnly,
	PVOID* ModuleBase
)
{
	DWORD dwLastError = ERROR_SUCCESS;
	PPEB_LDR_DATA LdrDataPtr = NULL;
	PLDR_DATA_TABLE_ENTRY LdrDataEntry = NULL;
	PVOID pModuleBase = NULL;
	PLIST_ENTRY HeadList = NULL;
	PLIST_ENTRY CurrentModule = NULL;
	PEB Peb = { 0 };
	PPEB PebPtr = &Peb;

	dwLastError = GetPebPtr(&PebPtr);
	if (ERROR_SUCCESS != dwLastError)
	{
		*ModuleBase = NULL;
		return dwLastError;
	}

	//
	// obtain the loader lock
	//
	EnterCriticalSection(PebPtr->LoaderLock);

	//
	// obtain a pointer to the Ldr Data from the PEB
	//
	LdrDataPtr = (PPEB_LDR_DATA)PebPtr->Ldr;

#ifdef _DEBUG
	printf("%s:%s:%d LdrData: 0x%p \n", __FILE__, __FUNCTION__, __LINE__, LdrDataPtr);
#endif // _DEBUG

	//
	// obtain a pointer to the head of the list and make a variable to use for looping
	//
	HeadList = &(LdrDataPtr->InMemoryOrderModuleList);
	CurrentModule = HeadList->Flink;

#ifdef _DEBUG
	printf("%s:%s:%d HeadList: 0x%p, CurrentModule: 0x%p \n", __FILE__, __FUNCTION__, __LINE__, HeadList, CurrentModule);
#endif // _DEBUG

	//
	// begin looping over each entry in the linked list until we see the 
	// head of the list come up again
	//
	while (CurrentModule != HeadList)
	{
		LdrDataEntry = CONTAINING_RECORD(CurrentModule, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

#ifdef _DEBUG
		wprintf(L"%wZ, 0x%p, 0x%08x\n", LdrDataEntry->BaseDllName, LdrDataEntry->DllBase, LdrDataEntry->BaseNameHashValue);
#endif // _DEBUG

		//
		// are we looking for a string or hash
		//
		if (bHashOnly)
		{
			//
			// grab the hash of the module name and compare it with what was passed in as an arg
			//
			ULONG BaseNameHash = LdrDataEntry->BaseNameHashValue;
			SIZE_T IsMatch = RtlCompareMemory(&BaseNameHash, ModuleNameHash, sizeof(ULONG));
			if (sizeof(ULONG) == IsMatch)
			{
				wprintf(L"arg hash: 0x%08x \n", *ModuleNameHash);
				wprintf(L"base name hash: 0x%08x \n", BaseNameHash);
				wprintf(L"-------------------------------------------------\n");
				wprintf(L"%S:%s:%d Found a match! \n", __FILE__, __FUNCTIONW__, __LINE__);
				wprintf(L"BaseDllName\n");
				wprintf(L"    \\====> %wZ \n", LdrDataEntry->BaseDllName);
				wprintf(L"BaseAddr\n");
				wprintf(L"    \\====> 0x%p \n", LdrDataEntry->DllBase);
				wprintf(L"HashValue\n");
				wprintf(L"    \\====> 0x%08x \n", LdrDataEntry->BaseNameHashValue);
				wprintf(L"-------------------------------------------------\n");

				pModuleBase = LdrDataEntry->DllBase;

				break;
			}
		}
		//
		// looking via string
		//
		else
		{
			UNICODE_STRING BaseName = LdrDataEntry->BaseDllName;
			INT IsMatch = _wcsicmp(BaseName.Buffer, ModuleName->Buffer);

			if (0 == IsMatch)
			{
				wprintf(L"%S:%s:%d Found a match! \n", __FILE__, __FUNCTIONW__, __LINE__);
				wprintf(L"-------------------------------------------------\n");
				wprintf(L"BaseDllName\n");
				wprintf(L"    \\====> %wZ \n", LdrDataEntry->BaseDllName);
				wprintf(L"BaseAddr\n");
				wprintf(L"    \\====> 0x%p \n", LdrDataEntry->DllBase);
				wprintf(L"HashValue\n");
				wprintf(L"    \\====> 0x%08x \n", LdrDataEntry->BaseNameHashValue);
				wprintf(L"-------------------------------------------------\n");

				pModuleBase = LdrDataEntry->DllBase;

				break;
			}
		}

		CurrentModule = CurrentModule->Flink;
	}

	//
	// release the loader lock
	//
	LeaveCriticalSection(PebPtr->LoaderLock);

	if (pModuleBase)
	{
		*ModuleBase = pModuleBase;
	}
	else
	{
		*ModuleBase = NULL;
	}


	return ERROR_SUCCESS;
}


_Use_decl_annotations_
PIMAGE_DOS_HEADER
WINAPI
GetDosHeader(
	PVOID ModuleBase
)
{
	// 
	// sanity check
	//
	if (IMAGE_DOS_SIGNATURE != ((PIMAGE_DOS_HEADER)ModuleBase)->e_magic)
	{
		printf("[ERROR] not valid PE signature \n");
		return NULL;
	}
	//
	// copy over the DOS header into the gobal PeInfo struct and the signature
	//
	memcpy(&g_PeInfo.ImageDosHdr, g_ModuleBase, sizeof(IMAGE_DOS_HEADER));
	g_Offset = g_PeInfo.ImageDosHdr.e_lfanew;

	memcpy(&g_PeInfo.Signature, (PVOID)((DWORD_PTR)g_ModuleBase + g_Offset), sizeof(DWORD));
	g_Offset += sizeof(DWORD);

	return (PIMAGE_DOS_HEADER)ModuleBase;
}


_Use_decl_annotations_
PIMAGE_NT_HEADERS
WINAPI
GetNtHeader(
	PVOID ModuleBase
)
{
	return RVA2VA(PIMAGE_NT_HEADERS, ModuleBase, g_pImgDosHeader->e_lfanew);

	//return (PIMAGE_NT_HEADERS)(((DWORD_PTR)ModuleBase) + (DWORD_PTR)(((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew));
}


_Use_decl_annotations_
PIMAGE_DATA_DIRECTORY
WINAPI
GetDataDirectory(
	DWORD DirectoryIndex
)
{
	//
	// validate the range of the index, there are only 16
	//
	if (DirectoryIndex < 0 || DirectoryIndex > 15)
	{
		return NULL;
	}

	if (NULL == g_pImgOpt64)
	{
		return NULL;
	}

	return Is64Bit() ?
		&g_pImgOpt64->DataDirectory[DirectoryIndex] :
		&g_pImgOpt32->DataDirectory[DirectoryIndex];
}


_Use_decl_annotations_
PIMAGE_EXPORT_DIRECTORY
WINAPI
GetExports()
{
	//PIMAGE_DATA_DIRECTORY pImgDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	g_pImgDataDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);

	//
	// sanity check on exports
	//
	if ((NULL == g_pImgDataDir) || (0 == g_pImgDataDir->Size))
	{
		printf("[INFO] no exports found! \n");
		return NULL;
	}

	PIMAGE_EXPORT_DIRECTORY pImgExports = RVA2VA(PIMAGE_EXPORT_DIRECTORY, g_ModuleBase, g_pImgDataDir->VirtualAddress);

	return pImgExports;
}


_Use_decl_annotations_
PVOID
WINAPI
GetAddress(
	ULONG Rva
)
{
	return ImageRvaToVa(ImageNtHeader(g_ModuleBase), g_ModuleBase, Rva, NULL);
}


BOOLEAN
WINAPI
Is64Bit()
{
	return g_pImgOpt64 ? g_pImgOpt64->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC : FALSE;
}


_Use_decl_annotations_
PIMAGE_FILE_HEADER
WINAPI
GetFileHeader()
{
	memcpy(&g_PeInfo.ImageFileHdr, (PVOID)((DWORD_PTR)g_ModuleBase + g_Offset), sizeof(IMAGE_FILE_HEADER));
	g_Offset += sizeof(IMAGE_FILE_HEADER);

	if (IMAGE_FILE_MACHINE_AMD64 == g_PeInfo.ImageFileHdr.Machine)
	{
		memcpy(&g_PeInfo.OptionalHdr.ImageOptHdr64, (PVOID)((DWORD_PTR)g_ModuleBase + g_Offset), sizeof(IMAGE_OPTIONAL_HEADER64));
		g_pImgOpt64 = &(GetNtHeader(g_ModuleBase)->OptionalHeader);
	}

	else if (IMAGE_FILE_MACHINE_I386 == g_PeInfo.ImageFileHdr.Machine)
	{
		memcpy(&g_PeInfo.OptionalHdr.ImageOptHdr32, (PVOID)((DWORD_PTR)g_ModuleBase + g_Offset), sizeof(IMAGE_OPTIONAL_HEADER32));
	}

	g_Offset += g_PeInfo.ImageFileHdr.SizeOfOptionalHeader;


	return &(GetNtHeader(g_ModuleBase)->FileHeader);
}


VOID
WINAPI
DumpExports()
{
	HANDLE hSelf = GetCurrentProcess();
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize(hSelf, NULL, TRUE);

	//
	// sanity check the pointer for NULL
	//
	if (!g_pImgExports)
	{
		printf("[ERROR] can't dump exports, NULL ptr \n");
		return;
	}

	//
	// dump on
	//
	DWORD NameCount = g_pImgExports->NumberOfNames;
	DWORD FuncCount = g_pImgExports->NumberOfFunctions;


	//
	// method 1 - use the global offset variable to find the pointer to
	// the three tables needed to parse exports
	//
	g_Offset = g_pImgExports->AddressOfFunctions;
	PDWORD fnTable = RVA2VA(PDWORD, g_ModuleBase, g_Offset);

	g_Offset = g_pImgExports->AddressOfNames;
	PDWORD fnNames = RVA2VA(PDWORD, g_ModuleBase, g_Offset);

	g_Offset = g_pImgExports->AddressOfNameOrdinals;
	PWORD fnOrd = RVA2VA(PWORD, g_ModuleBase, g_Offset);

	//
	// you could loop using the g_Offset too, that's on you to make
	//
	/*for ()
	{

	}*/

	//
	// method 2 - use ternary operators and the RVA2VA macro to grab the 
	// pointers to the three tables
	//
	PDWORD FunctionsTable =
		(PDWORD)(g_pImgExports->AddressOfFunctions != 0 ?
			RVA2VA(PDWORD, g_ModuleBase, g_pImgExports->AddressOfFunctions) :
			NULL);

	PDWORD FunctionNamesTable = (PDWORD)(g_pImgExports->AddressOfNames != 0 ?
		RVA2VA(PDWORD, g_ModuleBase, g_pImgExports->AddressOfNames) :
		NULL);

	PWORD FunctionOrdinalsTable =
		(PWORD)(g_pImgExports->AddressOfNameOrdinals != 0 ?
			RVA2VA(PWORD, g_ModuleBase, g_pImgExports->AddressOfNameOrdinals) :
			NULL);

	CHAR szUndecoratedSymbol[1 << 10] = { 0 };
	DWORD dwOrdinalBase = g_pImgExports->Base;

	//
	// method 3 - no safety checks in place against the tables, just use
	// the RVA2VA macro to obtain the pointers to the three tables
	//
	PDWORD AddressOfNamesTable = RVA2VA(PDWORD, g_ModuleBase, g_pImgExports->AddressOfNames);	// export address table
	PWORD AddressOfNameOrdinalsTable = RVA2VA(PWORD, g_ModuleBase, g_pImgExports->AddressOfNameOrdinals); // hints table
	PDWORD AddressOfFunctionTable = RVA2VA(PDWORD, g_ModuleBase, g_pImgExports->AddressOfFunctions); // export name table

	//
	// loop count for how many names are present in the table
	//
	for (DWORD Entry = 0UL; Entry < g_pImgExports->NumberOfNames; Entry++)
	{
		//
		// obtain 3 attributes about the current entry in the table, name, ordinal, address
		//
		PCHAR FunctionName = RVA2VA(PCHAR, g_ModuleBase, AddressOfNamesTable[Entry]);
		DWORD FunctionOrdinal = AddressOfNameOrdinalsTable[Entry];
		PVOID FunctionAddress = RVA2VA(LPVOID, g_ModuleBase, AddressOfFunctionTable[FunctionOrdinal]);

		DWORD dwAddress = *(FunctionsTable + FunctionOrdinal - dwOrdinalBase);
		DWORD dwOffset = ConvertRvaToFileOffset(dwAddress);


		//
		// optionally turning this into a version of GetProcAddress with this check
		// 
		// obviously it would be best to not hard code this but instead, one could use a list
		// of API names, hashes of API names, or other obfuscation method
		// this is just the simplest example
		//
		if (0 == _stricmp(FunctionName, "GetCurrentProcessId"))
		{
			printf("found a match!! \n");
			printf("%s: 0x%p (0x%08x) \n", FunctionName, FunctionAddress, FunctionOrdinal);

			//
			// now we have the address tied to the function, cast it to the proper function pointer
			// type and then invoke it
			//
			GET_CURRENT_PROCESS_ID pfnGetCurrentProcessId = (GET_CURRENT_PROCESS_ID)FunctionAddress;
			printf("current pid is %d \n", pfnGetCurrentProcessId());

			//
			// pause execution here to verify the information found
			//
			getchar();

			//
			// all done
			//
			break;
		}


		// 
		// this section can be better implemented using strdup and a few other mechanisms
		// as of now, it's the most simplest check but not the best
		//
		// is the function forwarded to another DLL
		//
		//if (FunctionAddress > g_pImgExports && FunctionAddress < (g_pImgExports + g_pImgDataDir->Size))
		//if (dwOffset > g_pImgDataDir->VirtualAddress &&
		//	dwOffset < g_pImgDataDir->VirtualAddress + g_pImgDataDir->Size)
		//{
		//	// do forwarding stuff here
		//	printf("   %s is forwarded \n", FunctionName);
		//}
		//else 
		//{
		//	printf("   %s \n", FunctionName);
		//}

	}

	printf("%s: done! \n", __FUNCTION__);
}


DWORD
WINAPI
ConvertRvaToFileOffset(
	DWORD Rva
)
{
	PIMAGE_NT_HEADERS pImgNt = RVA2VA(PIMAGE_NT_HEADERS, g_ModuleBase, g_pImgDosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER pImgSection = IMAGE_FIRST_SECTION(pImgNt);

	for (DWORD i = 0UL; i < g_pImgFileHeader->NumberOfSections; i++)
	{
		if (Rva >= pImgSection[i].VirtualAddress && Rva < pImgSection[i].VirtualAddress + pImgSection[i].Misc.VirtualSize)
		{
			return pImgSection[i].PointerToRawData + Rva - pImgSection[i].VirtualAddress;
		}
	}
	return Rva;
}


VOID
WINAPI
DumpDosHeader()
{
	printf("DOS HEADER \n");
	printf("%15x e_magic \n", g_PeInfo.ImageDosHdr.e_magic);
	printf("%15x e_lfanew \n", g_PeInfo.ImageDosHdr.e_lfanew);
	printf("\n");
}


VOID
WINAPI
DumpOptHeader()
{
	//
	// hold the characteristics in its own variable 
	// and prepare the CHAR array to hold the string equivalent of the flags found
	//
	WORD Characteristics = g_PeInfo.OptionalHdr.ImageOptHdr64.DllCharacteristics;
	CHAR Values[1024] = { ANSI_NULL };
	//
	// test each flag to determine how to build out the string
	//
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA)) StringCchCatA(Values, _countof(Values), "\t\t\tHigh entropy \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE)) StringCchCatA(Values, _countof(Values), "\t\t  DLL can move \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY)) StringCchCatA(Values, _countof(Values), "\t\t  Code integrity image \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_NX_COMPAT)) StringCchCatA(Values, _countof(Values), "\t\t  Image is NX compatible \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_NO_ISOLATION)) StringCchCatA(Values, _countof(Values), "\t\t  Image understands isolation and doesn't want it \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_NO_SEH)) StringCchCatA(Values, _countof(Values), "\t\t  Image does not use SEH \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_NO_BIND)) StringCchCatA(Values, _countof(Values), "\t\t  Do not bind this image \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_APPCONTAINER)) StringCchCatA(Values, _countof(Values), "\t\t  Image should execute in an AppContainer \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_WDM_DRIVER)) StringCchCatA(Values, _countof(Values), "\t\t  Driver uses WDM model \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_GUARD_CF)) StringCchCatA(Values, _countof(Values), "\t\t  Image supports Control Flow Guard \n");
	if (FlagOn(Characteristics, IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)) StringCchCatA(Values, _countof(Values), "\t\t  Image is terminal server aware \n");

	printf("OPTIONAL HEADER \n");
	printf("%15x Magic\n", g_PeInfo.OptionalHdr.ImageOptHdr64.Magic);
	printf("%15x Size Of Code\n", g_PeInfo.OptionalHdr.ImageOptHdr64.SizeOfCode);
	printf("%15x Size Of Initialized Data\n", g_PeInfo.OptionalHdr.ImageOptHdr64.SizeOfInitializedData);
	printf("%15x Entry Point\n", g_PeInfo.OptionalHdr.ImageOptHdr64.AddressOfEntryPoint);
	printf("%15llx Image Base\n", g_PeInfo.OptionalHdr.ImageOptHdr64.ImageBase);
	printf("%15x Section Alignment\n", g_PeInfo.OptionalHdr.ImageOptHdr64.SectionAlignment);
	printf("%15x File Alignment\n", g_PeInfo.OptionalHdr.ImageOptHdr64.FileAlignment);
	printf("%15x Subsystem\n", g_PeInfo.OptionalHdr.ImageOptHdr64.Subsystem);
	printf("%15x DLL Characteristics\n", g_PeInfo.OptionalHdr.ImageOptHdr64.DllCharacteristics);
	printf("\t%s", Values);
	printf("%15x Number of Directories\n", g_PeInfo.OptionalHdr.ImageOptHdr64.NumberOfRvaAndSizes);
	//
	// loop over the directories and dump the info
	//
	for (DWORD Entry = 0UL; Entry < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; Entry++)
	{
		IMAGE_DATA_DIRECTORY TempEntry = g_PeInfo.OptionalHdr.ImageOptHdr64.DataDirectory[Entry];
		printf("%15x [%8x] RVA [size] of DataDirectory[%lu] \n", TempEntry.VirtualAddress, TempEntry.Size, Entry);
	}
}


VOID
WINAPI
DumpFileHeader()
{
	/*
	%[flags][width][.precision][size]type
	*/
	printf("FILE HEADER \n");

	WORD Machine = g_PeInfo.ImageFileHdr.Machine;
	WORD Characteristics = g_PeInfo.ImageFileHdr.Characteristics;
	CHAR Values[1024] = { ANSI_NULL };

	if (IMAGE_FILE_MACHINE_AMD64 == Machine)
	{
		printf("%15x Machine (x64) \n", g_PeInfo.ImageFileHdr.Machine);
	}
	else if (IMAGE_FILE_MACHINE_I386 == Machine)
	{
		printf("%15x Machine (x86) \n", g_PeInfo.ImageFileHdr.Machine);
	}

	printf("%15x NumberOfSections\n", g_PeInfo.ImageFileHdr.NumberOfSections);
	printf("%15x TimeDateStamp\n", g_PeInfo.ImageFileHdr.TimeDateStamp);
	printf("%15x PointerToSymbolTable\n", g_PeInfo.ImageFileHdr.PointerToSymbolTable);
	printf("%15x NumberOfSymbols\n", g_PeInfo.ImageFileHdr.NumberOfSymbols);
	printf("%15x SizeOfOptionalHeader\n", g_PeInfo.ImageFileHdr.SizeOfOptionalHeader);
	printf("%15x Characteristics", Characteristics);

	if (FlagOn(Characteristics, IMAGE_FILE_RELOCS_STRIPPED)) StringCchCatA(Values, _countof(Values), " Relocations Stripped");
	if (FlagOn(Characteristics, IMAGE_FILE_EXECUTABLE_IMAGE)) StringCchCatA(Values, _countof(Values), " Executable");
	if (FlagOn(Characteristics, IMAGE_FILE_LARGE_ADDRESS_AWARE)) StringCchCatA(Values, _countof(Values), " Large address aware");
	if (FlagOn(Characteristics, IMAGE_FILE_32BIT_MACHINE)) StringCchCatA(Values, _countof(Values), " 32 bit machine");
	if (FlagOn(Characteristics, IMAGE_FILE_SYSTEM)) StringCchCatA(Values, _countof(Values), " SYS");
	if (FlagOn(Characteristics, IMAGE_FILE_DLL)) StringCchCatA(Values, _countof(Values), " DLL");

	printf("%s \n\n", Values);

}

/// <summary>
/// TODO: finish this
/// </summary>
/// <returns></returns>
VOID
WINAPI
DumpImports()
{

}

/// <summary>
/// TODO: finish this
/// </summary>
/// <returns></returns>
VOID
WINAPI
DumpIAT()
{

}