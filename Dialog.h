#ifndef DIALOG_H
#define DIALOG_H

#include <windows.h>
#include <tchar.h>
#include <string>

using std::string;

class Dialog
{
public:
	Dialog(HINSTANCE instance, LPTSTR str_template, HWND parent = NULL);
	Dialog(HINSTANCE instance, UINT id_template, HWND parent = NULL);

	bool modal(INT_PTR* result = NULL);
	bool modeless();
	bool close();

	bool closed() const { return (this->window == NULL); }
	HWND hwnd() const { return this->window; }

protected:
	static const string DEFTITLE;
	HINSTANCE instance;
	HWND parent;
	HWND window;
	bool is_modal;

	bool internal_close(INT_PTR result = 0);
	virtual INT_PTR dlg_proc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
	static const TCHAR PROP_INSTANCE[];
	LPTSTR restemplate;

	static bool set_instance(HWND window, Dialog* dlg);
	static Dialog* get_instance(HWND window);
	static INT_PTR CALLBACK dlg_proc_proxy(HWND, UINT, WPARAM, LPARAM);
};

#endif
