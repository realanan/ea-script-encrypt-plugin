// easep.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "eathena/plugin.h"

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
	{ "athena_init", EVENT_ATHENA_INIT },
	{ "athena_final", EVENT_ATHENA_FINAL },
	{ "plugin_test", EVENT_PLUGIN_TEST },
};

EASEP_EXPORT void plugin_init()
{

}

EASEP_EXPORT void plugin_final()
{

}

EASEP_EXPORT void athena_init()
{

}

EASEP_EXPORT void athena_final()
{

}

EASEP_EXPORT int plugin_test()
{
	return 1;
}
