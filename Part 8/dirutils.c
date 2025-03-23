#include "dirutils.h"

_Use_decl_annotations_
DWORD
WINAPI
DirSearch(LPVOID lpParam)
{
	if (lpParam)
	{
		printf("thread arg: %s\n", ((PARGS)lpParam)->FileName);
	}
	
	DWORD dwLastError = ERROR_SUCCESS;
	HANDLE hSearchHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA FindData = { 0 };				// find wildcard api's
	SYSTEMTIME ModifiedTime =	{ 0 };				// M
	SYSTEMTIME AccessTime =		{ 0 };				// A
	SYSTEMTIME CreationTime =	{ 0 };				// C no B :(
	__int64 FullFileSize =		0;
	LARGE_INTEGER FileSize =	{ 0 };				// required for Find api's and WIN32_FIND_DATAA
	CHAR FileName[] =			"C:\\USERS\\*";

	hSearchHandle = FindFirstFileA(FileName, &FindData);

	if (INVALID_HANDLE_VALUE == hSearchHandle)
	{
		return GetLastError();
	}

	do
	{
		// convert file times to system times
		if (!FileTimeToSystemTime(&FindData.ftCreationTime, &CreationTime))
		{
			return GetLastError();
		}

		// grab full file size of current entry
		FileSize.HighPart = FindData.nFileSizeHigh;
		FileSize.LowPart =	FindData.nFileSizeLow;
		FullFileSize =		FileSize.QuadPart;

		// is it a directory?
		if ((FILE_ATTRIBUTE_DIRECTORY & FindData.dwFileAttributes) &&
			0 != (_stricmp("..", FindData.cFileName)) &&
			0 != (_stricmp(".", FindData.cFileName))
			)
		{
			printf("%14s %02d/%02d/%02d %02d:%02d    %6lld %-20s \n",
				"<DIR>", CreationTime.wDay, CreationTime.wMonth, CreationTime.wYear,
				CreationTime.wHour, CreationTime.wMinute,
				FullFileSize,
				FindData.cFileName
				);
		}

		// dealing with a file. Directory negation
		else if (!(FILE_ATTRIBUTE_DIRECTORY & FindData.dwFileAttributes) &&
			0 != (_stricmp("..", FindData.cFileName)) &&
			0 != (_stricmp(".", FindData.cFileName)))
		{
			printf("%14s %02d/%02d/%02d %02d:%02d    %6lld %-20s \n",
				"<--->", CreationTime.wDay, CreationTime.wMonth, CreationTime.wYear,
				CreationTime.wHour, CreationTime.wMinute,
				FullFileSize,
				FindData.cFileName
			);

			if (FlagIsOn(FindData.dwFileAttributes, FILE_ATTRIBUTE_NORMAL)) printf("\t normal \n");
		}


	} while (FindNextFileA(hSearchHandle, &FindData));

	// handle returned from "FindFirstFile" is not a real handle. Pseudo-handle.
	FindClose(hSearchHandle);

	return dwLastError;
}