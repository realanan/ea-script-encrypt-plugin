#include "stdafx.h"
#include "inlinehook.h"
#include "hookfunc.h"
#include "decrypt.h"

#define HOOK_INDEX_CreateFileA		0
#define HOOK_INDEX_CreateFileW		1
#define HOOK_INDEX_ReadFile			2
#define HOOK_INDEX_SetFilePointer	3
#define HOOK_INDEX_GetFileSize		4
#define HOOK_INDEX_CloseHandle		5

static _locale_t current_locale;

HANDLE WINAPI HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							  DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	HANDLE hFile;
	PDECRYPT_DATA pdd;

	if (dwDesiredAccess == GENERIC_READ && dwCreationDisposition == OPEN_EXISTING)
	{
		IH_HookOffAll();
		if (DECRYPT_Check(lpFileName))
		{
			pdd = DECRYPT_Open(lpFileName);
			if (pdd)
			{
				IH_HookOnAll();
				return (HANDLE)((DWORD)pdd | 0x80000000);
			}
		}
		IH_HookOnAll();
	}

	IH_HOOKOFF(CreateFileW);
	hFile = CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	IH_HOOKON(CreateFileW);
	return hFile;
}

HANDLE WINAPI HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,
							  DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	size_t converted;
	wchar_t filename[MAX_PATH];

	_mbstowcs_s_l(&converted, filename, SIZEOF_ARRAY(filename), lpFileName, _TRUNCATE, current_locale);
	return HookCreateFileW(filename, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL WINAPI HookReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL ret;
	PDECRYPT_DATA pdd;

	if ((DWORD)hFile & 0x80000000)
	{
		DWORD dwRead;

		pdd = (PDECRYPT_DATA)((DWORD)hFile & 0x7fffffff);
		dwRead = DECRYPT_Read(pdd, lpBuffer, nNumberOfBytesToRead);
		if (lpNumberOfBytesRead) *lpNumberOfBytesRead = dwRead;
		return TRUE;
	}

	IH_HOOKOFF(ReadFile);
	ret = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	IH_HOOKON(ReadFile);
	return ret;
}

DWORD WINAPI HookSetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	DWORD ret;
	PDECRYPT_DATA pdd;

	if ((DWORD)hFile & 0x80000000)
	{
		pdd = (PDECRYPT_DATA)((DWORD)hFile & 0x7fffffff);
		return DECRYPT_Seek(pdd, lDistanceToMove, dwMoveMethod);
	}

	IH_HOOKOFF(SetFilePointer);
	ret = SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
	IH_HOOKON(SetFilePointer);
	return ret;
}

DWORD WINAPI HookGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh)
{
	DWORD ret;
	PDECRYPT_DATA pdd;

	if ((DWORD)hFile & 0x80000000)
	{
		pdd = (PDECRYPT_DATA)((DWORD)hFile & 0x7fffffff);
		return DECRYPT_GetSize(pdd);
	}

	IH_HOOKOFF(GetFileSize);
	ret = GetFileSize(hFile, lpFileSizeHigh);
	IH_HOOKON(GetFileSize);
	return ret;
}

BOOL WINAPI HookCloseHandle(HANDLE hObject)
{
	BOOL ret;

	PDECRYPT_DATA pdd;

	if ((DWORD)hObject & 0x80000000)
	{
		pdd = (PDECRYPT_DATA)((DWORD)hObject & 0x7fffffff);
		DECRYPT_Close(pdd);
		return TRUE;
	}

	IH_HOOKOFF(CloseHandle);
	ret = CloseHandle(hObject);
	IH_HOOKON(CloseHandle);
	return ret;
}

static FARPROC *orig_hook_func = NULL;

static FARPROC new_hook_func[] =
{
	(FARPROC)HookCreateFileA,
	(FARPROC)HookCreateFileW,
	(FARPROC)HookReadFile,
	(FARPROC)HookSetFilePointer,
	(FARPROC)HookGetFileSize,
	(FARPROC)HookCloseHandle,
};

void HF_Init()
{
	HMODULE hModKernel32;

	current_locale = _create_locale(LC_ALL, "");
	
	hModKernel32 = GetModuleHandle("kernel32.dll");
	orig_hook_func = (FARPROC*)malloc(sizeof(new_hook_func));
	orig_hook_func[IH_HOOKINDEX(CreateFileA)] = GetProcAddress(hModKernel32, "CreateFileA");
	orig_hook_func[IH_HOOKINDEX(CreateFileW)] = GetProcAddress(hModKernel32, "CreateFileW");
	orig_hook_func[IH_HOOKINDEX(ReadFile)] = GetProcAddress(hModKernel32, "ReadFile");
	orig_hook_func[IH_HOOKINDEX(SetFilePointer)] = GetProcAddress(hModKernel32, "SetFilePointer");
	orig_hook_func[IH_HOOKINDEX(GetFileSize)] = GetProcAddress(hModKernel32, "GetFileSize");
	orig_hook_func[IH_HOOKINDEX(CloseHandle)] = GetProcAddress(hModKernel32, "CloseHandle");

	IH_Init(orig_hook_func, SIZEOF_ARRAY(new_hook_func), new_hook_func);
}

void HF_Final()
{
	IH_Final();
	SAFE_FREE(orig_hook_func);

	_free_locale(current_locale);
}
