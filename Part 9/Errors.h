#pragma once

#include <Windows.h>
#include <stdio.h>

VOID
WINAPI
CheckLastInetError(
	_In_ LPCWSTR lpMessage,
	_In_ DWORD dwLastErr
);
