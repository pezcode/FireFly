#include "Dialog.h"

#include <cassert>

const string Dialog::DEFTITLE = "FireFly";
const TCHAR Dialog::PROP_INSTANCE[] = _T("FireFlyDialogInstancePtr");

Dialog::Dialog(HINSTANCE instance, LPTSTR str_template, HWND parent) : instance(instance), restemplate(str_template), parent(parent), window(NULL)/*, result(0)*/
{
	assert(parent == NULL || IsWindow(parent));
}

Dialog::Dialog(HINSTANCE instance, UINT id_template, HWND parent) : instance(instance), restemplate(MAKEINTRESOURCE(id_template)), parent(parent), window(NULL)/*, result(0)*/
{
	assert(parent == NULL || IsWindow(parent));
}

bool Dialog::modal(INT_PTR* result)
{
	if(!this->closed())
		return false;

	this->is_modal = true;
	INT_PTR result_ = DialogBoxParam(this->instance, this->restemplate, this->parent, &Dialog::dlg_proc_proxy, reinterpret_cast<LPARAM>(this));
	this->window = NULL;
	if(result)
		*result = result_;
	return (result_ != -1);
}

bool Dialog::modeless()
{
	if(!this->closed())
		return false;

	this->is_modal = false;
	HWND window = CreateDialogParam(this->instance, this->restemplate, this->parent, &Dialog::dlg_proc_proxy, reinterpret_cast<LPARAM>(this));
	return (window != NULL);
}

bool Dialog::close()
{
	if(this->closed())
		return true;

	if(this->is_modal)
	{
		SendMessage(this->window, WM_CLOSE, 0, 0);
	}
	else
	{
		DestroyWindow(this->window);
	}
	return this->closed();
}

bool Dialog::set_instance(HWND window, Dialog* dlg)
{
	assert(IsWindow(window));

	return SetProp(window, Dialog::PROP_INSTANCE, reinterpret_cast<HANDLE>(dlg));
}

Dialog* Dialog::get_instance(HWND window)
{
	assert(IsWindow(window));

	return reinterpret_cast<Dialog*>(GetProp(window, Dialog::PROP_INSTANCE));
}

bool Dialog::internal_close(INT_PTR result)
{
	if(this->closed())
		return true;

	if(this->is_modal)
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
Dialog* dlg = Dialog::get_instance(window);

	switch(message)
	{
	case WM_INITDIALOG:
		dlg = reinterpret_cast<Dialog*>(lParam);
		assert(dlg);
		dlg->window = window;
		Dialog::set_instance(window, dlg);
		break;
	case WM_DESTROY:
		assert(dlg);
		dlg->dlg_proc(message, wParam, lParam);
		dlg->window = NULL;
		Dialog::set_instance(window, NULL);
		return false;
	}

	if(dlg)
		return dlg->dlg_proc(message, wParam, lParam);
	else
		return false;
}
