#include "FireFly.h"

#include <commctrl.h>
#include <shellapi.h>
#include <angelscript.h>
#include <cassert>

const wchar_t FireFly::DlgAbout::TEXT[] =	L"FireFly 0.1\n"
											L"C++ scripting with Ollydbg interface\n"
											L"AngelScript: 2.21.0"
											L"Scintilla: 2.28";

const wchar_t URL_ANGELSCRIPT[L_MAX_URL_LENGTH] = L"http://www.angelcode.com/angelscript/";
const wchar_t   URL_SCINTILLA[L_MAX_URL_LENGTH] = L"http://www.scintilla.org/";

bool FireFly::DlgAbout::show()
{
	return Dialog::show(true);
}

INT_PTR FireFly::DlgAbout::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
		LITEM item;
		item.mask = LIF_ITEMINDEX | LIF_URL;
		item.iLink = 0;
		wcscpy(item.szUrl, URL_ANGELSCRIPT);
		SendDlgItemMessage(this->window, LnkAboutAS, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));
		item.iLink = 1;
		wcscpy(item.szUrl, URL_SCINTILLA);
		SendDlgItemMessage(this->window, LnkAboutSC, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));
		//SetDlgItemText(this->window, LnkAboutAS, URL_STR_AS);
		//SetDlgItemText(this->window, LnkAboutSC, URL_STR_SC);
		//FireFly::VERSION, ANGELSCRIPT_VERSION_STRING
		}
		break;

	case WM_NOTIFY:
		NMHDR* nmHdr;
		nmHdr = reinterpret_cast<NMHDR*>(lParam);
		assert(nmHdr);
		switch(nmHdr->code)
		{
			case NM_CLICK:  // mouse
			case NM_RETURN: // keyboard
			{
				NMLINK* nmLink = reinterpret_cast<NMLINK*>(nmHdr);
				LITEM item = nmLink->item;

				assert((nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutAS)) || (nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutSC)));
				assert(item.iLink == 0 || item.iLink == 1);

				ShellExecute(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
				break;
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
	}

	return true;
}
