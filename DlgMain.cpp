#include "Dialogs.h"

#include <windowsx.h>
#include <scintilla.h>
#include <scilexer.h>
#include <cassert>

extern HMODULE g_Instance;

INT_PTR DlgMain::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
static int margin_size = 0;
static bool show_lines = true;

	switch(message)
	{
	case WM_INITDIALOG:
		{
		this->hEditor = GetDlgItem(this->window, ScEditor);

		// Set up lexer
		this->SendEditor(SCI_SETLEXER, SCLEX_CPP);
		this->SendEditor(SCI_SETSTYLEBITS, this->SendEditor(SCI_GETSTYLEBITSNEEDED));

		// Set up line number margin
		this->SendEditor(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
		this->SendEditor(SCI_SETMARGINCURSORN, 0, SC_CURSORARROW);
		margin_size = this->SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, "_1");
		this->SendEditor(SCI_SETMARGINWIDTHN, 0, margin_size);
		show_lines = true;

		// Disable all other margins
		this->SendEditor(SCI_SETMARGINWIDTHN, 1, 0);
		this->SendEditor(SCI_SETMARGINWIDTHN, 2, 0);
		this->SendEditor(SCI_SETMARGINWIDTHN, 3, 0);
		this->SendEditor(SCI_SETMARGINWIDTHN, 4, 0);

		// Set up default color, font, size
		const COLORREF CLR_BLACK = RGB(0,0,0);
		const COLORREF CLR_WHITE = RGB(255,255,255);

		this->SendEditor(SCI_STYLESETFORE, STYLE_DEFAULT, CLR_BLACK);
		this->SendEditor(SCI_STYLESETBACK, STYLE_DEFAULT, CLR_WHITE);
		this->SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
		this->SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, "Courier New");

		// Set up Lexer colors
		const COLORREF CLR_LGREEN = RGB(0, 128, 0);
		const COLORREF CLR_LBLUE  = RGB(0, 127, 174);

		/*
		String 163, 21, 21 (Rot)
		Stale 128, 0, 128 (Violett)
		Preprocessor 0, 0, 255
		Keyword 0, 0, 255

		Sweet blue 43, 145, 175 / 0, 127, 174
		*/

		this->set_editor_color(SCE_C_DEFAULT, CLR_BLACK);

		this->set_editor_color(SCE_C_COMMENT, CLR_LGREEN);
		this->set_editor_color(SCE_C_COMMENTLINE, CLR_LGREEN);
		this->set_editor_color(SCE_C_COMMENTDOC, CLR_LGREEN);

		//this->set_editor_color(SCE_C_NUMBER, CLR_LBLUE);
		//this->set_editor_color(SCE_C_WORD, CLR_LBLUE);

		this->set_editor_color(SCE_C_STRING, CLR_LBLUE);
		this->set_editor_color(SCE_C_CHARACTER, CLR_LBLUE);

		/*
		this->set_editor_color(SCE_C_UUID, CLR_LBLUE);
		this->set_editor_color(SCE_C_PREPROCESSOR, CLR_LBLUE);
		this->set_editor_color(SCE_C_OPERATOR, CLR_LBLUE);
		this->set_editor_color(SCE_C_IDENTIFIER, CLR_LBLUE);
		this->set_editor_color(SCE_C_STRINGEOL, CLR_LBLUE);
		this->set_editor_color(SCE_C_VERBATIM, CLR_LBLUE);
		this->set_editor_color(SCE_C_REGEX, CLR_LBLUE);
		*/
		this->set_editor_color(SCE_C_COMMENTLINEDOC, CLR_LGREEN);
		
		//this->set_editor_color(SCE_C_WORD2, CLR_LBLUE);

		this->set_editor_color(SCE_C_COMMENTDOCKEYWORD, CLR_LGREEN);
		this->set_editor_color(SCE_C_COMMENTDOCKEYWORDERROR, CLR_LGREEN);
		/*
		this->set_editor_color(SCE_C_GLOBALCLASS, CLR_LBLUE);
		this->set_editor_color(SCE_C_STRINGRAW, CLR_LBLUE);
		this->set_editor_color(SCE_C_TRIPLEVERBATIM, CLR_LBLUE);
		*/

		this->SendEditor(SCI_STYLESETFORE, STYLE_LINENUMBER, CLR_LBLUE);
		//this->SendEditor(SCI_STYLESETBACK, STYLE_LINENUMBER, CLR_WHITE);
		this->SendEditor(SCI_STYLESETSIZE, STYLE_LINENUMBER, 10);
		this->SendEditor(SCI_STYLESETFONT, STYLE_LINENUMBER, "Courier New");

		//SendEditor(SCI_STYLECLEARALL);

		//this->SendEditor(SCI_SETPROPERTY, "lexer.cpp.allow.dollars", "1");

		// compilation errors etc.:
		// SCI_ANNOTATIONSETTEXT
		}
		break;

	case WM_GETMINMAXINFO:
		{
		MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
		assert(mmi);
		mmi->ptMinTrackSize.x = 400;
		mmi->ptMinTrackSize.y = 400;
		//mmi->ptMaxTrackSize.x = 700;
		//mmi->ptMaxTrackSize.y = 700;
		}
		break;

	case WM_SIZING:
		{
		// calculate difference to previous size
		RECT* rectNew = reinterpret_cast<RECT*>(lParam);
		assert(rectNew);
		RECT rectOld;
		GetWindowRect(this->window, &rectOld);
		long deltaX = (rectNew->right  - rectNew->left) - (rectOld.right  - rectOld.left);
		long deltaY = (rectNew->bottom - rectNew->top)  - (rectOld.bottom - rectOld.top);
		// resize child window
		RECT rectEditor;
		GetWindowRect(this->hEditor, &rectEditor);
		int cx = rectEditor.right - rectEditor.left + deltaX;
		int cy = rectEditor.bottom - rectEditor.top + deltaY;
		MapWindowRect(HWND_DESKTOP, this->window, &rectEditor);
		MoveWindow(this->hEditor, rectEditor.left, rectEditor.top, cx, cy, false);
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
				{
				char buffer[16] = "";
				int lines = this->SendEditor(SCI_GETLINECOUNT);
				assert(lines > 0);
				sprintf(buffer, "_%d", lines);
				margin_size = this->SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, buffer);
				if(show_lines)
					this->SendEditor(SCI_SETMARGINWIDTHN, 0, margin_size);
				}
				break;
			}
		}
		}
		break;

	case WM_COMMAND:
		HMENU hMenu;
		hMenu = GetSubMenu(GetMenu(this->window), 1);
		assert(hMenu);
		switch(GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDM_FILE_NEW:
			break;
		case IDM_FILE_OPEN:
			break;
		case IDM_FILE_SAVE:
			break;
		case IDM_FILE_CLOSE:
			SendMessage(this->window, WM_CLOSE, 0, 0);
			break;

		case IDM_OPTIONS_FONT:
			break;
		case IDM_OPTIONS_COLORS:
			break;

		case IDM_VIEW_LINENUMBERS:
			bool checked;
			checked = ((GetMenuState(hMenu, IDM_VIEW_LINENUMBERS, MF_BYCOMMAND) & MF_CHECKED) == MF_CHECKED);
			show_lines = !checked;
			CheckMenuItem(hMenu, IDM_VIEW_LINENUMBERS, MF_BYCOMMAND | (show_lines ? MF_CHECKED : MF_UNCHECKED));
			this->SendEditor(SCI_SETMARGINWIDTHN, 0, (show_lines ? margin_size : 0));
			break;

		case IDM_HELP_HELP:
			break;
		case IDM_HELP_ABOUT:
			{
			DlgAbout about(this->instance, this->window);
			about.modal();
			}
			break;
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
