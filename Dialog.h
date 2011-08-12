#ifndef DIALOG_H
#define DIALOG_H

#include <windows.h>
#include <tchar.h>
#include <string>

using std::string;

class Dialog
{
public:
	Dialog(HINSTANCE instance, LPTSTR restemplate, HWND parent = NULL);
	//~Dialog();

	bool show(bool modal);
	bool close();

	bool closed() const { return (this->window == NULL); }
	//INT_PTR result() const { return this->result; }

	HWND handle() const { return this->window; }

protected:
	static const string DEFTITLE;

	HWND window;

	bool internal_close(INT_PTR result = 0);
	virtual INT_PTR dlg_proc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
	HINSTANCE instance;
	LPTSTR restemplate;
	HWND parent;

	//INT_PTR result;

	bool modal;

	static const TCHAR PROP_INSTANCE[];

	bool set_instance(HWND window);
	static Dialog* get_instance(HWND window);

	static INT_PTR CALLBACK dlg_proc_proxy(HWND, UINT, WPARAM, LPARAM);
};

#endif
