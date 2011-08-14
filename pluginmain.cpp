/**
 * @file
 * Sets up plugin structures and forwards plugin notifications from Ollydbg.
 */

#include <cstring>
#include <windows.h>
#include <ollydbg.h>
#include "FireFly.h"
#include "Dialogs.h"

extern HMODULE g_Instance; // in dllmain.cpp

const wchar_t PLUGINNAME[SHORTNAME] = L"FireFly";
const int MINOLLYDBGVERSION = 201;

FireFly* FF_instance = NULL;

int Menu_Run(t_table* pt, wchar_t* name, ulong index, int mode);
int Menu_About(t_table* pt, wchar_t* name, ulong index, int mode);

t_menu mainmenu[] =
{
	{
		L"FireFly",
		L"Start FireFly",
		K_NONE, &Menu_Run, NULL, 0
	},

	{
		L"|About",
		L"About Firefly",
		K_NONE, &Menu_About, NULL, 0
	},

	{
		NULL, NULL, K_NONE, NULL, NULL, 0
	}
};

int Menu_Run(t_table* pt, wchar_t* name, ulong index, int mode)
{
	switch(mode)
	{
	case MENU_VERIFY:
		return MENU_NORMAL;
	case MENU_EXECUTE:
		//FF_instance->show_dialog();
		return MENU_NOREDRAW;
	default:
		return MENU_ABSENT;
	}
}

int Menu_About(t_table* pt, wchar_t* name, ulong index, int mode)
{
	switch(mode)
	{
	case MENU_VERIFY:
		return MENU_NORMAL;
	case MENU_EXECUTE:
		//FF_instance->about();
		{
		DlgAbout about(g_Instance, _hwollymain);
		about.modal();
		}
		return MENU_NOREDRAW;
	default:
		return MENU_ABSENT;
	}
}

/**
 * Called when Ollydbg loads this plugin and requests information.
 * 
 * @param ollydbgversion
 *   The version of this instance of Ollydbg.
 * @param pluginname
 *   String buffer that should hold the plugin name on return.
 * @param pluginversion
 *   String buffer that should hold the plugin version on return.
 * 
 * @return
 *   The supported plugin version, should always be PLUGIN_VERSION.
 *   0 to report an error and abort loading the plugin.
 */
extc _export int cdecl ODBG2_Pluginquery(int ollydbgversion, wchar_t pluginname[SHORTNAME], wchar_t pluginversion[SHORTNAME])
{
	// plugin load
	if(ollydbgversion < MINOLLYDBGVERSION)
		return 0;

	FF_instance = new FireFly(g_Instance, _hwollymain);

	wcscpy(pluginname, PLUGINNAME);
	wcscpy(pluginversion, FireFly::VERSION);
	return PLUGIN_VERSION;
}

/**
 * Called when Ollydbg requests us to add ourselves to the plugin.
 * 
 * This information comes from the main Windows debug loop.
 * Not all events are reported if command emulation is active, use ODBG2_Pluginexception in that case.
 * Only use this callback if you need to as it may have an impact on speed.
 * 
 * This callback is optional.
 * 
 * @param type
 * 
 * @return
 *   Pointer to an array of t_menu items, terminated by an empty t_menu.
 */
extc _export t_menu* cdecl ODBG2_Pluginmenu(wchar_t* type)
{
	if(!wcscmp(type, PWM_MAIN))
		return mainmenu;
	//else if(!wcscmp(type, PWM_DISASM))
	//	return disasmmenu;

	return NULL;
}

/**
 * Called when Ollydbg encounters a debug event in the debugged process.
 * 
 * This information comes from the main Windows debug loop.
 * Not all events are reported if command emulation is active, use ODBG2_Pluginexception in that case.
 * Only use this callback if you need to as it may have an impact on speed.
 * 
 * This callback is optional.
 * 
 * @param debugevent
 */
extc void _export cdecl ODBG2_Pluginmainloop(DEBUG_EVENT* debugevent)
{

}

/**
 * Called when Ollydbg finished analysis of a module.
 * 
 * @param preg
 */
extc void _export cdecl ODBG2_Pluginexception(t_reg* preg)
{

}

/**
 * Called when Ollydbg finished analysis of a module.
 * 
 * This callback is optional.
 * 
 * @param pmod
 */
extc void _export cdecl ODBG2_Pluginanalyse(t_module* pmod)
{

}

/**
 * Called when Ollydbg is about to save internal data to a .udd file.
 * 
 * @param psave
 * @param pmod
 * @param ismainmodule
 */
extc void _export cdecl ODBG2_Pluginsaveudd(t_uddsave* psave, t_module* pmod, int ismainmodule)
{

}

/**
 * Called when Ollydbg finds data belonging to this plugin in a .udd file.
 * 
 * The data submitted may be corrupted or incomplete. Don't rely on its correctness.
 * 
 * @param pmod
 * @param ismainmodule
 * @param tag
 * @param size
 * @param data
 */
extc void _export cdecl ODBG2_Pluginuddrecord(t_module* pmod, int ismainmodule, ulong tag, ulong size, void* data)
{
	// check pointers and size
}

/**
 * Called when a process was (re)started.
 */
extc void _export cdecl ODBG2_Pluginreset()
{
	
}

/**
 * Called when Ollydbg is requesting to quit.
 * 
 * This callback is optional.
 * 
 * @return
 *   0 if it's safe to quit, non-0 to abort termination.
 */
extc int _export cdecl ODBG2_Pluginclose()
{
	return 0;
}

/**
 * Called when Ollydbg is about to quit.
 */
extc void _export cdecl ODBG2_Plugindestroy()
{
	//FF_instance->close();
	delete FF_instance;
	FF_instance = NULL;
}
