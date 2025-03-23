/*
This code provides a set of functions to perform HTTP requests using the WinINet API in Windows.
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


#pragma once

#include <Windows.h>
#include <wininet.h>
#include <stdio.h>

#pragma comment(lib, "wininet.lib")


DWORD
WINAPI
InitSession(
	_Out_ LPHINTERNET hSession
);


DWORD
WINAPI
CloseSession(
	_In_ HINTERNET hSession
);


DWORD
WINAPI
MakeRequest(
	_In_ HINTERNET hSession,
	_In_ LPSTR theTarget,
	_In_ INTERNET_PORT thePort,
	_In_ LPSTR httpVerb,
	_Out_ LPHINTERNET hRequest
);


DWORD
WINAPI
SendRequest(
	_In_ HINTERNET hRequest
);


DWORD
WINAPI
ProcessResults
(
	_In_ HINTERNET hRequest
);