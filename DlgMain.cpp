#include "Dialogs.h"

#include <scintilla.h>
#include <cassert>

/*
bool DlgMain::show()
{
	return Dialog::modeless();
}
*/

INT_PTR DlgMain::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(this->window, IDC_EDIT, SCI_SETLEXER, 0, 0);
		break;

	case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			//case IDC_EDIT:
			//	break;

			default:
				return false;
			}
		break;

	case WM_CLOSE:
		internal_close(0);
		break;

	default:
		return false;
	}

	return true;
}
