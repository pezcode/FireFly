#include "Dialogs.h"

#include <scintilla.h>
#include <scilexer.h>
#include <cassert>

INT_PTR DlgMain::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
		this->hEditor = GetDlgItem(this->window, ScEditor);

		this->SendEditor(SCI_SETLEXER, SCLEX_CPP);
		this->SendEditor(SCI_SETSTYLEBITS, this->SendEditor(SCI_GETSTYLEBITSNEEDED));
		this->SendEditor(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);

		const COLORREF CLR_BLACK = RGB(0,0,0);
		const COLORREF CLR_WHITE = RGB(255,255,255);

		// Set up default color, font, size
		this->SendEditor(SCI_STYLESETFORE, STYLE_DEFAULT, CLR_BLACK);
		this->SendEditor(SCI_STYLESETBACK, STYLE_DEFAULT, CLR_WHITE);
		this->SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, 11);
		this->SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, "Verdana");

		// Set up Lexer colors
		this->set_editor_color(SCE_C_COMMENTLINE, RGB(255, 0, 0));
		//...

		// Line number pane
		//STYLE_LINENUMBER

		//SendEditor(SCI_STYLECLEARALL);

		//this->SendEditor(SCI_SETPROPERTY, "lexer.cpp.allow.dollars", "1");

		//this->SendEditor(SCI_COLOURISE, 0, -1);
		}
		break;

	case WM_NOTIFY:
		{
		const NMHDR* nmHdr = reinterpret_cast<const NMHDR*>(lParam);
		assert(nmHdr);
		if(nmHdr->hwndFrom == hEditor)
		{
			switch(nmHdr->code)
			{
				case SCN_MODIFIED:
					//this->SendEditor(SCI_COLOURISE, 0, -1);
				break;
			}
		}
		}
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
