/*
This C program demonstrates how to perform an HTTP GET request using the WinINet API in Windows.
The program initializes an internet session, makes a request to a specified target and port,
sends the request, processes the results, and then closes the session.
It uses functions defined in "HttpApis.h" and handles errors using "Errors.h".

Here's a brief overview of the main steps:

1. InitSession: Initializes an internet session using the WinINet API.
2. MakeRequest: Prepares an HTTP GET request to the specified target and port.
3. SendRequest: Sends the prepared HTTP request to the server.
4. ProcessResults: Processes the results of the HTTP request, typically by reading the response.
5. CloseSession: Closes the internet session to free up resources.

The program returns a DWORD, which is a Windows data type for a 32-bit unsigned integer,
commonly used for error codes or status information.
*/


#include <Windows.h>
#include "HttpApis.h"
#include "Errors.h"

#define __cdecl CDECL



INT
CDECL
main(VOID)
{
	DWORD dwLastErr = ERROR_SUCCESS;
	HINTERNET hSession = NULL;

	LPSTR theTarget = "127.0.0.1";
	INTERNET_PORT thePort = 8080;
	HINTERNET hRequest = NULL;
	LPSTR theVerb = "GET";

	dwLastErr = InitSession(&hSession);

#ifdef _DEBUG
	printf("%s: making the request... \n", __FUNCTION__);
#endif // _DEBUG

	dwLastErr = MakeRequest(
		hSession,
		theTarget,
		thePort,
		theVerb,
		&hRequest
	);

	dwLastErr = SendRequest(hRequest);
	if (ERROR_SUCCESS != dwLastErr)
	{
#ifdef _DEBUG
		CheckLastInetError(L"send request bombed ", dwLastErr);
		return dwLastErr;
#endif // _DEBUG
	}

	dwLastErr = ProcessResults(hRequest);

	CloseSession(hSession);

	return dwLastErr;
}