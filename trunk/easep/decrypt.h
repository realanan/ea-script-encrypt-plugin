#pragma once

typedef struct DecryptData
{
	DWORD dwSize;
	DWORD dwPos;
	char *data;
} DECRYPT_DATA, *PDECRYPT_DATA;

void DECRYPT_SetKey(char *lpKey);
BOOL DECRYPT_Check(LPCWSTR lpFileName);
PDECRYPT_DATA DECRYPT_Open(LPCWSTR lpFileName);
void DECRYPT_Close(PDECRYPT_DATA pdd);
DWORD DECRYPT_Seek(PDECRYPT_DATA pdd, LONG move, DWORD method);
DWORD DECRYPT_GetSize(PDECRYPT_DATA pdd);
DWORD DECRYPT_Read(PDECRYPT_DATA pdd, LPVOID lpBuffer, DWORD len);
