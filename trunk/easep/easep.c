// easep.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "eathena/plugin.h"
#include "hookfunc.h"
#include "decrypt.h"

EASEP_EXPORT PLUGIN_INFO =
{
	"Script Decrypt Plugin",
	PLUGIN_ALL,
	"1.0.0",
	"1.03",
	"A scripts and databases decrypt plugin for eAthena windows version",
};

EASEP_EXPORT void** plugin_call_table;

EASEP_EXPORT PLUGIN_EVENTS_TABLE =
{
	{ "plugin_init", EVENT_PLUGIN_INIT },
	{ "plugin_final", EVENT_PLUGIN_FINAL },
	{ NULL, NULL }
};

EASEP_EXPORT void plugin_init()
{
	char key[100] = { 0 };
	int keypos = 0;
	int ch;

	printf("[EASEP] Enter the password to decrypt the files: ");
	while (ch = _getch())
	{
		if (ch == 13) break;
		if (ch == 8 && keypos > 0)
			key[--keypos] = '\0';
		else if (keypos < sizeof(key))
			key[keypos++] = ch;
	}
	printf("\n");

	DECRYPT_SetKey(key);
	HF_Init();
}

EASEP_EXPORT void plugin_final()
{
	HF_Final();
}
