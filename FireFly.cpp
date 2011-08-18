#include "FireFly.h"

//#include <ollydbg.h>
#include <commctrl.h>
#include <scintilla.h>

const wchar_t FireFly::VERSION[] = L"0.02";

FireFly::FireFly(HINSTANCE instance, HWND parent) : instance(instance), parent(parent), lang(*this)
{
	INITCOMMONCONTROLSEX icce;
	icce.dwSize = sizeof(icce);
	// We need tabs, tooltips, link controls
	icce.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES | ICC_LINK_CLASS;
	InitCommonControlsEx(&icce);

	Scintilla_RegisterClasses(this->instance);
	Scintilla_LinkLexers();
}

FireFly::~FireFly()
{
	Scintilla_ReleaseResources();
}

void FireFly::run_script(const std::string& script)
{
	if(!this->lang.script_load(script))
	{
		return;
	}

	if(!this->lang.script_compile())
	{
		return;
	}

	if(!this->lang.script_run())
	{
		return;
	}
}
