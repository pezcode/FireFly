#ifndef FIREFLY_H
#define FIREFLY_H

#include <windows.h>
#include <string>
#include "Dialog.h"
#include "resource.h"

class FireFly
{
public:

	class DlgMain : Dialog
	{
	public:
		DlgMain(HINSTANCE instance, HWND parent) : Dialog(instance, MAKEINTRESOURCE(IDD_MAIN), parent) { }
		bool show();

	private:
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
	};

	class DlgAbout : Dialog
	{
	public:
		DlgAbout(HINSTANCE instance, HWND parent) : Dialog(instance, MAKEINTRESOURCE(IDD_ABOUT), parent) { }
		bool show();

	private:
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
		static const wchar_t TEXT[];
	};

	class DlgInput : Dialog
	{
	public:
		DlgInput(HINSTANCE instance, HWND parent) : Dialog(instance, MAKEINTRESOURCE(IDD_INPUT), parent) { }
		string show(const string& text, const string& title = DEFTITLE);

		bool accepted() const;
		string input() const;

	private:
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
	};

	class DlgQuestion
	{
		// plain messagebox (yes, no, cancel)
	};

	class DlgMessage
	{
		// plain messagebox
	};

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

	void run_script();

public:

	static const wchar_t VERSION[];

private:

	HINSTANCE instance;
	HWND parent;
};

#endif
