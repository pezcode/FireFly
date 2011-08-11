#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

FireFlyLang::FireFlyLang(FireFly& interface) : interface(interface)
{
	this->engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	RegisterStdString(this->engine);
}

FireFlyLang::~FireFlyLang()
{
	this->engine->Release();
	this->engine = NULL;
}

bool FireFlyLang::register_functions()
{
	engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(&FireFlyLang::print), asCALL_CDECL);
}

void FireFlyLang::print(const string& in)
{
	this->interface.message(in);
}
