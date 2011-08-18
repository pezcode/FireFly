#include "Dialogs.h"

#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <scintilla.h>
#include <scilexer.h>
#include <cassert>

#include "FireFly.h"

extern HMODULE g_Instance;

INT_PTR DlgMain::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
static int margin_size = 0;
static bool show_lines = true;
static HWND StatusWindow = NULL;
static HIMAGELIST hImgListTool = NULL;
static HWND ToolWindow = NULL;
static HMENU hMenuFile = NULL, hMenuView = NULL;

	switch(message)
	{
	case WM_INITDIALOG:
		{
		this->hEditor = GetDlgItem(this->window, ScEditor);
		assert(IsWindow(this->hEditor));

		hMenuFile = GetSubMenu(GetMenu(this->window), 0);
		hMenuView = GetSubMenu(GetMenu(this->window), 1);
		assert(IsMenu(hMenuFile));
		assert(IsMenu(hMenuView));

		// We need a custom message loop to add TranslateAccelerator
		// hint: WTL now!!!
		//LoadAccelerators(this->instance, MAKEINTRESOURCE(IDR_ACCELERATOR));

		// Create toolbar
		ToolWindow = CreateWindow(TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->window, reinterpret_cast<HMENU>(TbMain), this->instance, NULL); ;//CreateToolbarEx(this->window, WS_VISIBLE | )
		assert(IsWindow(ToolWindow));

		// Declare and initialize local constants.
		const int ImageListID    = 0;
		const int numButtons     = 3;
		const int bitmapSize     = 16;

		const DWORD buttonStyles = BTNS_AUTOSIZE;

		// Create the image list
		hImgListTool = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR32 | ILC_MASK, numButtons, 0);

		// Set the image list
		SendMessage(ToolWindow, TB_SETIMAGELIST, ImageListID, reinterpret_cast<LPARAM>(hImgListTool));

		// Load the button images
		SendMessage(ToolWindow, TB_LOADIMAGES, IDB_STD_SMALL_COLOR, reinterpret_cast<LPARAM>(HINST_COMMCTRL));

		// Initialize button info
		// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

		TBBUTTON tbButtons[numButtons] = 
		{
			{ MAKELONG(STD_FILENEW,  ImageListID), IDM_FILE_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, reinterpret_cast<LPARAM>(L"New") },
			{ MAKELONG(STD_FILEOPEN, ImageListID), IDM_FILE_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, reinterpret_cast<LPARAM>(L"Open")},
			{ MAKELONG(STD_FILESAVE, ImageListID), IDM_FILE_SAVE, 0,               buttonStyles, {0}, 0, reinterpret_cast<LPARAM>(L"Save")}
		};

		// Add buttons
		SendMessage(ToolWindow, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
		SendMessage(ToolWindow, TB_ADDBUTTONS, numButtons, reinterpret_cast<LPARAM>(&tbButtons));

		// Resize the toolbar
		SendMessage(ToolWindow, TB_AUTOSIZE, 0, 0);

		RECT toolRect;
		GetClientRect(ToolWindow, &toolRect);
		RECT editRect;
		GetWindowRect(this->hEditor, &editRect);
		MapWindowRect(HWND_DESKTOP, this->window, &editRect);
		
		MoveWindow(this->hEditor, editRect.left, editRect.top + toolRect.bottom, editRect.right - editRect.left, editRect.bottom - editRect.top - toolRect.bottom, false);

		// Create status bar
		StatusWindow = CreateWindow(STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, this->window, reinterpret_cast<HMENU>(SbInfo), this->instance, NULL);
		assert(IsWindow(StatusWindow));
		RECT DlgRect, SbRect;
		GetWindowRect(this->window, &DlgRect); // Increase dialog height to integrate the status bar
		GetClientRect(StatusWindow, &SbRect);
		MoveWindow(this->window, DlgRect.left, DlgRect.top, DlgRect.right-DlgRect.left, DlgRect.bottom-DlgRect.top+SbRect.bottom, true);
		SendMessage(StatusWindow, WM_SIZE, 0, 0); // Force Status bar to reposition

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
		}
		break;

	case WM_SIZING:
		{
		// calculate difference to previous size
		const RECT* rectNew = reinterpret_cast<const RECT*>(lParam);
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

	case WM_SIZE:
		SendMessage(StatusWindow, WM_SIZE, 0, 0); // Force Status bar to reposition
		//SendMessage(ToolWindow, TB_AUTOSIZE, 0, 0);
		//InvalidateRect(this->window, NULL, true);
		break;

	case WM_NOTIFY:
		{
		const NMHDR* nmHdr = reinterpret_cast<const NMHDR*>(lParam);
		assert(nmHdr);
		if(nmHdr->hwndFrom == this->hEditor)
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
				case SCN_SAVEPOINTREACHED:
					SetWindowText(this->window, L"FireFly");
					EnableMenuItem(hMenuFile, IDM_FILE_SAVE, MF_BYCOMMAND | MF_GRAYED);
					break;
				case SCN_SAVEPOINTLEFT:
					SetWindowText(this->window, L"* FireFly");
					EnableMenuItem(hMenuFile, IDM_FILE_SAVE, MF_BYCOMMAND | MF_ENABLED);
					break;
				default:
					return false;
			}
		}
		else return false;
		}
		break;

	case WM_DROPFILES:
		{
		HDROP hDrop = reinterpret_cast<HDROP>(wParam);
		unsigned int Count = DragQueryFile(hDrop, static_cast<UINT>(-1), 0, 0);
		if(Count > 0)
		{
			wchar_t FileName[MAX_PATH];
			DragQueryFile(hDrop, 0, FileName, _countof(FileName));
			if(!(GetFileAttributes(FileName) & FILE_ATTRIBUTE_DIRECTORY))
			{
				this->SendEditor(SCI_SETTEXT, NULL, "SNAKE");
				// do stuff
			}
		}
		DragFinish(hDrop);
		}
		break;

	case WM_HELP: // F1
		break;

	//case WM_GETDLGCODE:
	//	return DLGC_WANTCHARS;

		/*
		 * I want CTRL+S :(
		 * only sent to windows with focus (= Scintilla)
		 * can't use accelerator (message loop in olly)
		 * can't use RegisterHotKey (might be registered)
		 **/
	case WM_CHAR:
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'S':
		case 's':
			//this->SendEditor(SCI_SETSAVEPOINT);
			//if(!(lParam & (1 << 30))) //previous key state
			{
				if(GetKeyState(VK_CONTROL)/* & (1 << 15)*/)
				{
					//SendMessage(this->window, WM_COMMAND, )
					this->SendEditor(SCI_SETSAVEPOINT);
					break;
				}
			}
			return false;
		default:
			return false;
		}
		break;

	case WM_COMMAND:
		switch(GET_WM_COMMAND_ID(wParam, lParam))
		{
		case IDM_FILE_NEW:
			{
			bool cancel = false;
			if(this->SendEditor(SCI_GETMODIFY))
			{
				DlgQuestion question(this->window);
				question.show(L"The code has been modified. Save changes?", L"FireFly", DlgQuestion::tWarning, DlgQuestion::cYesNoCancel);
				switch(question.get_result())
				{
				case DlgQuestion::rYes:
					/*
					if(!filename)
					{
						DlgFileSave save(this->instance, this->window);
						save.modal();
						if(save.cancelled())
						{
							cancel = true;
							break;
						}
					}
					save(file);
					*/
				case DlgQuestion::rNo:
					break;
				case DlgQuestion::rCancel:
					cancel = true;
					break;
				}
			}
			if(!cancel)
			{
				this->SendEditor(SCI_CLEARALL);
				this->SendEditor(SCI_SETSAVEPOINT);
				//file = "";
			}
			}
			break;
		case IDM_FILE_OPEN:
			{
				/*
			int len = this->SendEditor(SCI_GETLENGTH) + 1;
			assert(len > 0);
			const char* buf = new char[len];
			this->SendEditor(SCI_GETTEXT, len, buf);
			this->interface.run_script(buf);
			delete[] buf;
			*/
			//DlgFileOpen open(this->instance, this->window);
			//open.modal();
			//filename = ??
			//this->SendEditor(SCI_SETTEXT, NULL, text);
			}
			break;
		case IDM_FILE_SAVE:
			//if(!filename)
			//{
			//DlgFileSave save(this->instance, this->window);
			//save.modal();
			//filename = ??
			//}
			//assert(this->SendEditor(SCI_GETMODIFY));
			this->SendEditor(SCI_SETSAVEPOINT);
			break;
		case IDM_FILE_CLOSE:
			{
			bool cancel = false;
			if(this->SendEditor(SCI_GETMODIFY))
			{
				DlgQuestion question(this->window);
				question.show(L"The code has been modified. Save changes?", L"FireFly", DlgQuestion::tWarning, DlgQuestion::cYesNoCancel);
				switch(question.get_result())
				{
				case DlgQuestion::rYes:
					/*
					if(!filename)
					{
						DlgFileSave save(this->instance, this->window);
						save.modal();
						if(save.cancelled())
						{
							cancel = true;
							break;
						}
					}
					save(file);
					*/
				case DlgQuestion::rNo:
					break;
				case DlgQuestion::rCancel:
					cancel = true;
					break;
				}
			}
			if(!cancel)
				SendMessage(this->window, WM_CLOSE, 0, 0);
			}
			break;

		case IDM_OPTIONS_FONT:
			break;
		case IDM_OPTIONS_COLORS:
			break;

		case IDM_VIEW_LINENUMBERS:
			bool checked;
			checked = ((GetMenuState(hMenuView, IDM_VIEW_LINENUMBERS, MF_BYCOMMAND) & MF_CHECKED) == MF_CHECKED);
			show_lines = !checked;
			CheckMenuItem(hMenuView, IDM_VIEW_LINENUMBERS, MF_BYCOMMAND | (show_lines ? MF_CHECKED : MF_UNCHECKED));
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
		DestroyWindow(StatusWindow);
		DestroyWindow(ToolWindow);
		ImageList_Destroy(hImgListTool);
		internal_close(0);
		break;

	default:
		return false;
	}

	return true;
}
