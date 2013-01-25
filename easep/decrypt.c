#include "stdafx.h"
#include "decrypt.h"

#include "polarssl/config.h"
#include "polarssl/aes.h"
#include "polarssl/base64.h"
#include "polarssl/sha2.h"

static unsigned char decrypt_key[32];

void DECRYPT_SetKey(char *lpKey)
{
	const unsigned char hash_key[] = { 0x24, 0xde, 0x14, 0x64, 0x45, 0x78, 0xfa, 0x46, 0x55, 0x9c, 0xd3, 0xf9, 0xfd, 0x02, 0x19, 0x88 };
	sha2_context ctx;

	sha2_hmac_starts(&ctx, hash_key, sizeof(hash_key), 0);
	sha2_hmac_update(&ctx, (const unsigned char*)lpKey, strlen(lpKey));
	sha2_hmac_finish(&ctx, decrypt_key);
}

BOOL DECRYPT_Check(LPCWSTR lpFileName)
{
	FILE *fp;
	char line[20];

	_wfopen_s(&fp, lpFileName, L"r");
	if (fp == NULL) return FALSE;
	if (fgets(line, sizeof(line), fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}
	fclose(fp);
	return strcmp(line, "[EASEP]") == 0;
}

PDECRYPT_DATA DECRYPT_Open(LPCWSTR lpFileName)
{
	FILE *fp;
	char line[20];
	PDECRYPT_DATA pdd;
	long fileSize, contentSize;
	unsigned char *srcBuf = NULL, *dstBuf = NULL;
	size_t dstSize;
	aes_context aes_ctx;
	unsigned char iv[16] = { 0 };
	int ret;

	_wfopen_s(&fp, lpFileName, L"r");
	if (fp == NULL) return NULL;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fgets(line, sizeof(line), fp);
	contentSize = fileSize - ftell(fp);
	if (contentSize <= 0)
	{
		fclose(fp);
		return NULL;
	}

	srcBuf = (unsigned char*)malloc(contentSize);
	fread(srcBuf, 1, contentSize, fp);
	fclose(fp);

	dstSize = contentSize;
	dstBuf = (unsigned char*)malloc(dstSize);
	ret = base64_decode(dstBuf, &dstSize, srcBuf, contentSize);
	if (ret == POLARSSL_ERR_BASE64_INVALID_CHARACTER)
	{
		SAFE_FREE(srcBuf);
		SAFE_FREE(dstBuf);
		return NULL;
	}
	if (ret == POLARSSL_ERR_BASE64_BUFFER_TOO_SMALL)
	{
		SAFE_FREE(dstBuf);
		dstBuf = (unsigned char*)malloc(dstSize);
		ret = base64_decode(dstBuf, &dstSize, srcBuf, contentSize);
	}
	SAFE_FREE(srcBuf);

	pdd = (PDECRYPT_DATA)malloc(sizeof(DECRYPT_DATA));
	pdd->data = (char *)malloc(dstSize);
	aes_setkey_dec(&aes_ctx, decrypt_key, sizeof(decrypt_key) * 8);
	aes_crypt_cbc(&aes_ctx, AES_DECRYPT, dstSize, iv, dstBuf, (unsigned char*)pdd->data);

	pdd->dwSize = strlen(pdd->data);
	pdd->dwPos = 0;

	SAFE_FREE(dstBuf);
	return pdd;
}

void DECRYPT_Close(PDECRYPT_DATA pdd)
{
	SAFE_FREE(pdd->data);
	SAFE_FREE(pdd);
}

DWORD DECRYPT_Seek(PDECRYPT_DATA pdd, LONG move, DWORD method)
{
	LONG pos;

	switch (method)
	{
	case FILE_BEGIN:
		pos = 0;
		break;
	case FILE_END:
		pos = pdd->dwSize;
		break;
	}
	pos += move;
	if (pos < 0) pos = 0;
	if (pos > pdd->dwSize) pos = pdd->dwSize;
	pdd->dwPos = (DWORD)pos;
	return pdd->dwPos;
}

DWORD DECRYPT_GetSize(PDECRYPT_DATA pdd)
{
	return pdd->dwSize;
}

DWORD DECRYPT_Read(PDECRYPT_DATA pdd, LPVOID lpBuffer, DWORD len)
{
	DWORD read;

	read = min(pdd->dwSize - pdd->dwPos, len);
	memcpy(lpBuffer, pdd->data + pdd->dwPos, read);
	return read;
}
