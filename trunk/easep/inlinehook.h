#pragma once

void IH_Init(FARPROC *origFuncs, size_t funcCount, FARPROC *newFuncs);
void IH_Final();
void IH_HookOn(int index);
void IH_HookOff(int index);

#define IH_HOOKINDEX(func) HOOK_INDEX_##func

#define IH_HOOKON(func) IH_HookOn(IH_HOOKINDEX(func))
#define IH_HOOKOFF(func) IH_HookOff(IH_HOOKINDEX(func))
