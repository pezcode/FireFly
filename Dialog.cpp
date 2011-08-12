#include "Dialog.h"

#include <cassert>

const string Dialog::DEFTITLE = "FireFly";
const TCHAR Dialog::PROP_INSTANCE[] = _T("FireFlyDialogInstancePtr");

Dialog::Dialog(HINSTANCE instance, LPTSTR restemplate, HWND parent) : instance(instance), restemplate(restemplate), parent(parent), window(NULL)/*, result(0)*/
{
	assert(parent == NULL || IsWindow(parent));
}

bool Dialog::show(bool modal)
{
	this->modal = modal;
	if(this->modal)
	{
		/*this->result = */DialogBoxParam(this->instance, this->restemplate, this->parent, &Dialog::dlg_proc_proxy, reinterpret_cast<LPARAM>(this));
		//this->closed = true;
		return true;
		//return (this->result != -1);
	}
	else
	{
		//this->result = 0;
		HWND wnd = CreateDialogParam(this->instance, this->restemplate, this->parent, &Dialog::dlg_proc_proxy, reinterpret_cast<LPARAM>(this));
		return (wnd != NULL);
	}
}

bool Dialog::close()
{
	if(this->closed())
		return true;

	if(this->modal)
	{
		SendMessage(this->window, WM_CLOSE, 0, 0);
	}
	else
	{
		DestroyWindow(this->window);
	}
	return this->closed();
}

bool Dialog::set_instance(HWND window)
{
	assert(IsWindow(window));

	return SetProp(window, Dialog::PROP_INSTANCE, reinterpret_cast<HANDLE>(this));
}

Dialog* Dialog::get_instance(HWND window)
{
	assert(IsWindow(window));

	return reinterpret_cast<Dialog*>(GetProp(window, Dialog::PROP_INSTANCE));
}

bool Dialog::internal_close(INT_PTR result)
{
	if(this->modal)
	{
		return EndDialog(this->window, result);
	}
	else
	{
		// how to set return value?
		return DestroyWindow(this->window);
	}
}

INT_PTR Dialog::dlg_proc_proxy(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
Dialog* dlg = NULL;

	switch(message)
	{
	case WM_INITDIALOG:
		dlg = reinterpret_cast<Dialog*>(lParam);
		assert(dlg);
		dlg->window = window;
		dlg->set_instance(dlg->window);
		break;
	case WM_DESTROY:
		dlg = Dialog::get_instance(window);
		assert(dlg);
		dlg->window = NULL;
		break;
	default:
		dlg = Dialog::get_instance(window);
		assert(dlg);
		break;
	}

	return dlg->dlg_proc(message, wParam, lParam);
}
