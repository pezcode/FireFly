#include "FireFlyLang.h"

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <cassert>
#include <string>
#include "Dialogs.h"
#include "FireFly.h"

const char FireFlyLang::KEYWORDS[] = ""; // whitespace separated

FireFlyLang::FireFlyLang(FireFly& interface) : interface(interface)
{
	this->engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	RegisterStdString(this->engine);

	this->register_functions();
}

FireFlyLang::~FireFlyLang()
{
	this->engine->Release();
	this->engine = NULL;
}

bool FireFlyLang::register_functions()
{
	//this->engine->RegisterGlobalFunction("void print(const string& in)", asMETHOD(FireFlyLang, print), asCALL_THISCALL);
	return true;
}

bool FireFlyLang::script_load(const std::string& script)
{
	// The CScriptBuilder helper is an add-on that loads the file,
	// performs a pre-processing pass if necessary, and then tells
	// the engine to build a script module.
	CScriptBuilder builder;
	builder.StartNewModule(this->engine, "MyModule"); 
	builder.AddSectionFromMemory(script.c_str());

	builder.BuildModule();
	return true;
}

bool FireFlyLang::script_compile()
{
	return true;
}

bool FireFlyLang::script_run()
{
	// Find the function that is to be called. 
	asIScriptModule *mod = this->engine->GetModule("MyModule");
	int funcId = mod->GetFunctionIdByDecl("void main()");
	if(funcId < 0)
	{
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		DlgQuestion::show(this->interface.parent, L"The script must have the function 'void main()'. Please add it and try again.", L"FireFly", DlgQuestion::tError);
		return false;
	}

	// Create our context, prepare it, and then execute
	asIScriptContext *ctx = this->engine->CreateContext();
	ctx->Prepare(funcId);
	int r = ctx->Execute();
	switch(r)
	{
		case asEXECUTION_FINISHED:
			DlgQuestion::show(this->interface.parent, L"Execution finished.", L"FireFly", DlgQuestion::tInfo);
			break;
		case asEXECUTION_EXCEPTION:
			DlgQuestion::show(this->interface.parent, L"An exception occured.", L"FireFly", DlgQuestion::tError);
			break;
		default:
			DlgQuestion::show(this->interface.parent, L"Unknown error.", L"FireFly", DlgQuestion::tError);
			break;
	}
	return true;
}

void FireFlyLang::print(const string& in)
{
	//this->interface.message(in);
	DlgQuestion::show(this->interface.parent, L"message called", L"FireFly", DlgQuestion::tInfo);
}
