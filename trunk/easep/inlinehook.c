#include "stdafx.h"
#include "inlinehook.h"

typedef struct InlineHookData
{
	unsigned char oldData[5];
	unsigned char newData[5];
	void *funcAddr;
	CRITICAL_SECTION cs;
	DWORD dwOldProtect;
} INLINE_HOOK_DATA, *PINLINE_HOOK_DATA;

static PINLINE_HOOK_DATA ih_data = NULL;

void IH_Init(FARPROC *origFuncs, size_t funcCount, FARPROC *newFuncs)
{
	size_t i;

	ih_data = (PINLINE_HOOK_DATA)calloc(funcCount + 1, sizeof(INLINE_HOOK_DATA));
	for (i = 0; i < funcCount; i++)
	{
		memcpy(ih_data[i].oldData, origFuncs[i], sizeof(ih_data[i].oldData));
		ih_data[i].newData[0] = 0xe9;
		*(DWORD*)(ih_data[i].newData + 1) = (DWORD)newFuncs[i] - (DWORD)origFuncs[i] - 5;
		ih_data[i].funcAddr = origFuncs[i];
		InitializeCriticalSection(&ih_data[i].cs);
		VirtualProtect(ih_data[i].funcAddr, sizeof(ih_data[i].newData), PAGE_EXECUTE_READWRITE, &ih_data[i].dwOldProtect);
		memcpy(ih_data[i].funcAddr, ih_data[i].newData, sizeof(ih_data[i].newData));
	}
}

void IH_Final()
{
	size_t i;

	for (i = 0; ih_data[i].funcAddr; i++)
	{
		memcpy(ih_data[i].funcAddr, ih_data[i].oldData, sizeof(ih_data[i].oldData));
		VirtualProtect(ih_data[i].funcAddr, sizeof(ih_data[i].newData), ih_data[i].dwOldProtect, &ih_data[i].dwOldProtect);
		DeleteCriticalSection(&ih_data[i].cs);
	}
	SAFE_FREE(ih_data);
}

void IH_HookOn(int index)
{
	memcpy(ih_data[index].funcAddr, ih_data[index].newData, sizeof(ih_data[index].newData));
	LeaveCriticalSection(&ih_data[index].cs);
}

void IH_HookOff(int index)
{
	EnterCriticalSection(&ih_data[index].cs);
	memcpy(ih_data[index].funcAddr, ih_data[index].oldData, sizeof(ih_data[index].oldData));
}
