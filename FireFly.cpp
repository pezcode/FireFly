#include "FireFly.h"

#include <windows.h>
//#include <ollydbg.h>
#include <scintilla.h>
#include "resource.h"

FireFly::FireFly(HINSTANCE instance, HWND parent) : instance(instance), parent(parent), dialog_main(NULL)
{
	Scintilla_RegisterClasses(this->instance);
	Scintilla_LinkLexers();
}

FireFly::~FireFly()
{
	Scintilla_ReleaseResources();
}

bool FireFly::show()
{
	if(hDialog)
		return true;
	hDialog = CreateDialog(this->instance, MAKEINTRESOURCE(IDD_MAIN), this->parent, &FireFly::dialog_func);
	return (hDialog != NULL);
}

bool FireFly::close()
{
	if(hDialog)
	{
		return DestroyWindow(hDialog);
	}
}

bool FireFly::about()
{
const wchar_t TEXT_ABOUT[] =	L"FireFly 0.1\n"
								L"C++ scripting with Ollydbg interface\n"
								L"AngelScript: 2.21.0"
								L"Scintilla: 2.28";

	DialogBox(this->instance, MAKEINTRESOURCE(IDD_ABOUT), parent, &FireFly::about_func);
	return true;
}

void FireFly::run_script()
{
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
}

void FireFly::message(const string& str)
{
	MessageBoxA(_hwollymain, str.c_str(), "FireFly", MB_ICONINFORMATION);
}

INT_PTR CALLBACK FireFly::dialog_func(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			SendDlgItemMessage(Window, IDC_EDIT, SCI_SETLEXER)
			break;

		case WM_COMMAND:
				switch(LOWORD(wParam))
				{
				case IDC_EDIT:
					break;

				default:
					return false;
				}
			break;

		case WM_CLOSE:
			EndDialog(Window, 0);
			break;

		default:
			return false;
	}
	return true;
}
