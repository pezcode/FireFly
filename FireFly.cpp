#include "FireFly.h"

//#include <ollydbg.h>
#include <commctrl.h>
#include <scintilla.h>

const wchar_t FireFly::VERSION[] = L"0.02";

FireFly::FireFly(HINSTANCE instance, HWND parent) : instance(instance), parent(parent)
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

void FireFly::run_script()
{
	/*
	// The CScriptBuilder helper is an add-on that loads the file,
	// performs a pre-processing pass if necessary, and then tells
	// the engine to build a script module.
	CScriptBuilder builder;
	builder.StartNewModule(engine, "MyModule"); 
	builder.AddSectionFromFile("test.as");
	builder.BuildModule();

	// Find the function that is to be called. 
	asIScriptModule *mod = engine->GetModule("MyModule");
	int funcId = mod->GetFunctionIdByDecl("void main()");
	if(funcId < 0)
	{
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		message("The script must have the function 'void main()'. Please add it and try again.");
		return;
	}

	// Create our context, prepare it, and then execute
	asIScriptContext *ctx = engine->CreateContext();
	ctx->Prepare(funcId);
	int r = ctx->Execute();
	switch(r)
	{
		case asEXECUTION_FINISHED:
			break;
		case asEXECUTION_EXCEPTION:
			message("Exception");
			break;
		default:
			message("Unknown error.");
			break;
	}
	}*/
}
