/*
This C file implements a set of functions to perform HTTP requests using the WinINet API in Windows.
The functions work together to initialize a session, make an HTTP request, send the request,
and process the results. Here's a brief overview of each function:

1. InitSession: Initializes an internet session using the WinINet API, which is required to make HTTP requests.
2. CloseSession: Closes the internet session to free up resources.
3. MakeRequest: Prepares an HTTP request to a specified target and port using a specified HTTP verb (e.g., GET, POST).
4. SendRequest: Sends the prepared HTTP request to the server.
5. ProcessResults: Processes the results of the HTTP request, typically by reading the response.

Each function returns a DWORD, which is a Windows data type for a 32-bit unsigned integer,
commonly used for error codes or status information.
*/

#include "HttpApis.h"



_Use_decl_annotations_
DWORD
WINAPI
InitSession(
	LPHINTERNET hSession // wininet.h
)
{
	DWORD dwLastErr = ERROR_SUCCESS;

	*hSession = InternetOpenA(
		"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36",
		INTERNET_OPEN_TYPE_PRECONFIG, // Use system's internet configuration
		NULL, // No proxy name
		NULL, // No proxy bypass
		0 // No flags
	);

	// Error checking
	if (!hSession)
	{
		dwLastErr = GetLastError();
		printf(
			"%s: InternetOpenA failed: 0x%08x \n",
			__FUNCTION__,
			dwLastErr
		);
		return dwLastErr;
	}


	return dwLastErr;
}



_Use_decl_annotations_
DWORD
WINAPI
CloseSession(
	HINTERNET hSession
)
{
	InternetCloseHandle(hSession); // wininet.h

	return ERROR_SUCCESS;
}



_Use_decl_annotations_
DWORD
WINAPI
MakeRequest(
	HINTERNET hSession,
	LPSTR theTarget,
	INTERNET_PORT thePort,
	LPSTR httpVerb,
	LPHINTERNET hRequest
)
{
	DWORD dwLastErr = ERROR_SUCCESS;

	printf("%s: making the connection handle... \n", __FUNCTION__);

	// wininet.h
	HINTERNET hConnect = InternetConnectA(
		hSession, // internet session handle
		theTarget, // target server name
		thePort, // port number
		NULL, //no username
		NULL, // no password
		INTERNET_SERVICE_HTTP, // http service
		0UL, // no flags
		0ULL // no context
	);

	if (!hConnect)
	{
		*hRequest = NULL; // sets request handle to null
		dwLastErr = GetLastError();
		printf("%s: InternetConnectA failed: 0x%08x \n",
			__FUNCTION__,
			dwLastErr
		);
		return dwLastErr;
	}

	printf("%s: making the request handle... \n", __FUNCTION__);

	// wininet.h
	*hRequest = HttpOpenRequestA(
		hConnect, // connection handle
		httpVerb, // GET, POST, etc.
		"/", // URI
		NULL, NULL, NULL, // no version, referrer, accept types
		0UL, // no flags
		0ULL // no context
	);

	if (!hRequest)
	{
		dwLastErr = GetLastError();
		printf("%s: HttpOpenRequestA failed: 0x%08x \n",
			__FUNCTION__,
			dwLastErr
		);
		return dwLastErr;
	}

	printf("%s: done! \n", __FUNCTION__);

	return dwLastErr;
}



_Use_decl_annotations_
DWORD
WINAPI
SendRequest(
	HINTERNET hRequest
)
{
	DWORD dwLastErr = ERROR_SUCCESS;

	BOOL bRet = HttpSendRequestA(
		hRequest, // request handle
		NULL, // no additional headers
		0UL, // no header length
		NULL, // no optional data
		0ULL // no optional data length
	);

	if (!bRet)
	{
		dwLastErr = GetLastError();
		printf("%s: HttpSendRequestA failed: 0x%08x \n",
			__FUNCTION__,
			dwLastErr
		);
		return dwLastErr;
	}

	return dwLastErr;
}



_Use_decl_annotations_
DWORD
WINAPI
ProcessResults
(
	HINTERNET hRequest
)
{
	DWORD dwLastErr = ERROR_SUCCESS;
	DWORD dwBytesToRead = 1024UL;
	DWORD dwBytesRead = 0UL;
	LPVOID lpBuffer = NULL;

	// heapapi.h
	lpBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024ULL);

	if (!lpBuffer)
	{
		return GetLastError();
	}

	while (TRUE)
	{
		// wininet.h
		if (!InternetReadFile(hRequest,
			lpBuffer,
			dwBytesToRead,
			&dwBytesRead
		))
		{
			return GetLastError();
		}

		if (0 == dwBytesRead) break;
	}

	printf("%s: dump results... \n", __FUNCTION__);
	printf("%s \n", (LPSTR)lpBuffer);

	if (lpBuffer)
	{
		// heapapi.h
		HeapFree(GetProcessHeap(), 0, lpBuffer);
		lpBuffer = NULL;
	}

	return dwLastErr;
}