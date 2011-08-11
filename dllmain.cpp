/**
 * @file
 * Defines the entry point for the DLL application.
 */

#include <windows.h>

/**
 * The instance handle of this DLL.
 */
HMODULE g_Instance = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_Instance = hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		g_Instance = NULL;
		break;
	}
	return TRUE;
}
