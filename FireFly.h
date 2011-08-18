#ifndef FIREFLY_H
#define FIREFLY_H

#include <windows.h>
#include <string>
#include "Dialogs.h"
#include "resource.h"

#include "FireFlyLang.h"

class FireFly
{
friend class FireFlyLang;
public:

	FireFly(HINSTANCE instance, HWND parent);
	~FireFly();

	/*
	bool dlg_main_show();
	bool dlg_main_show();

	void dlg_about();

	bool dlg_info(const std::string& str);
	bool dlg_warning(const std::string& str);
	bool dlg_error(const std::string& str);

	bool dlg_ask(const std::string& str);

	bool dlg_input(const std::string& str);
	*/

	void run_script(const std::string& script);

public:

	static const wchar_t VERSION[];

private:

	HINSTANCE instance;
	HWND parent;

	FireFlyLang lang;

	//DlgMain* dlg_main;
};

#endif
