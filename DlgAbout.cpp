#include "Dialogs.h"

#include <commctrl.h>
#include <shellapi.h>
#include <angelscript.h>
#include <cassert>
#include "tabs.h"
#include "tooltip.h"

INT_PTR DlgAbout::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
static HWND TabWindow;
static TabInfo* TabDlgInfo;
static TabLibraries* TabDlgLibs;
static TabLicense* TabDlgLicense;

	switch(message)
	{
	case WM_INITDIALOG:
		{
		// Prepare tabs
		TabWindow = GetDlgItem(this->window, TabAbout);
		TabDlgInfo    = new TabInfo(this->instance, this->window);
		TabDlgLibs    = new TabLibraries(this->instance, this->window);
		TabDlgLicense = new TabLicense(this->instance, this->window);

		TabDlgInfo->modeless();
		TabDlgLibs->modeless();
		TabDlgLicense->modeless();

		AddTab(TabWindow, TabDlgInfo->hwnd(),    L"Info",      -1);
		AddTab(TabWindow, TabDlgLibs->hwnd(),    L"3rd-party", -1);
		AddTab(TabWindow, TabDlgLicense->hwnd(), L"License",   -1);

		TabToFront(TabWindow, 0);

		// Set tab icons
		/*
		HICON TabIconFiles     = (HICON)LoadImage(Instance, MAKEINTRESOURCE(IDI_SETTINGS_FILE), IMAGE_ICON, 16, 16, 0);
		HICON TabIconInterface = (HICON)LoadImage(Instance, MAKEINTRESOURCE(IDI_SETTINGS_UI),   IMAGE_ICON, 16, 16, 0);
		SetTabIcon(TabWindow, TAB_FILE, TabIconFiles);
		SetTabIcon(TabWindow, TAB_UI,   TabIconInterface);
		DestroyIcon(TabIconFiles);
		DestroyIcon(TabIconInterface);
		*/
		}
		break;

	case WM_NOTIFY:
		{
		const NMHDR* nmHdr = reinterpret_cast<const NMHDR*>(lParam);
		assert(nmHdr);
		switch(nmHdr->code)
		{
			case TCN_SELCHANGE: // Get selected tab window to front
				if(nmHdr->hwndFrom == TabWindow)
				{
					TabToFront(TabWindow, -1);
					break;
				}
				return false;
			default:
				return false;
		}
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case BnOKAbout:
			SendMessage(this->window, WM_CLOSE, 0, 0);
			break;

		default:
			return false;
		}
		break;

	case WM_CLOSE:
		TabDlgInfo->close();
		TabDlgLibs->close();
		TabDlgLicense->close();
		delete TabDlgInfo;
		delete TabDlgLibs;
		delete TabDlgLicense;
		this->internal_close(0);
		break;

	default:
		return false;
	}

	return true;
}

INT_PTR DlgAbout::TabInfo::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
		//FireFly::VERSION
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		default:
			return false;
		}
		break;

	case WM_CLOSE:
		this->internal_close(0);
		break;

	default:
		return false;
	}

	return true;
}

const wchar_t DlgAbout::TabLibraries::URL_ANGELSCRIPT[] = L"http://www.angelcode.com/angelscript/";
const wchar_t DlgAbout::TabLibraries::URL_SCINTILLA[]   = L"http://www.scintilla.org/";

INT_PTR DlgAbout::TabLibraries::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
static HWND TT_URL[2];

	switch(message)
	{
	case WM_INITDIALOG:
		{
		assert(sizeof(DlgAbout::TabLibraries::URL_ANGELSCRIPT) <= L_MAX_URL_LENGTH);
		assert(sizeof(DlgAbout::TabLibraries::URL_SCINTILLA)   <= L_MAX_URL_LENGTH);

		LITEM item;
		item.mask = LIF_ITEMINDEX | LIF_URL;
		item.iLink = 0;

		wcscpy(item.szUrl, URL_ANGELSCRIPT);
		SendDlgItemMessage(this->window, LnkAboutAS, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));
		wcscpy(item.szUrl, URL_SCINTILLA);
		SendDlgItemMessage(this->window, LnkAboutSC, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));

		TT_URL[0] = CreateTooltip(GetDlgItem(this->window, LnkAboutAS), DlgAbout::TabLibraries::URL_ANGELSCRIPT, false, this->instance);
		TT_URL[1] = CreateTooltip(GetDlgItem(this->window, LnkAboutSC), DlgAbout::TabLibraries::URL_SCINTILLA,   false, this->instance);

		SetDlgItemTextA(this->window, StAboutVerAS, ANGELSCRIPT_VERSION_STRING);
		SetDlgItemTextA(this->window, StAboutVerSC, "2.28");
		}
		break;

	case WM_NOTIFY:
		{
		const NMHDR* nmHdr = reinterpret_cast<const NMHDR*>(lParam);
		assert(nmHdr);
		switch(nmHdr->code)
		{
			case NM_CLICK:  // mouse
			case NM_RETURN: // keyboard
			{
				const NMLINK* nmLink = reinterpret_cast<const NMLINK*>(nmHdr);
				LITEM item = nmLink->item;

				assert((nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutAS)) || (nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutSC)));
				assert(item.iLink == 0);

				ShellExecute(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
				break;
			}
		}
		}
		break;

		/*
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		default:
			return false;
		}
		break;
		*/

	case WM_DESTROY:
		for(int i = 0; i < _countof(TT_URL); i++)
		{
			DestroyWindow(TT_URL[i]);
		}
		break;

	default:
		return false;
	}

	return true;
}

const wchar_t DlgAbout::TabLicense::LICENSE_TEXT[] =

L"Copyright (c) 2011 Christopher Daun" L"\r\n\r\n"

L"This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software." L"\r\n\r\n"

L"Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:" L"\r\n"
L"1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required." L"\r\n"
L"2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software." L"\r\n"
L"3. This notice may not be removed or altered from any source distribution.";

INT_PTR DlgAbout::TabLicense::dlg_proc(UINT message, WPARAM wParam, LPARAM lParam)
{
//static HWND TT_URL[2];

	switch(message)
	{
	case WM_INITDIALOG:
		{
			SetDlgItemText(this->window, EdLicense, DlgAbout::TabLicense::LICENSE_TEXT);
			/*
		assert(sizeof(DlgAboutTabLibraries::URL_ANGELSCRIPT) <= L_MAX_URL_LENGTH);
		assert(sizeof(DlgAboutTabLibraries::URL_SCINTILLA) <= L_MAX_URL_LENGTH);

		LITEM item;
		item.mask = LIF_ITEMINDEX | LIF_URL;
		item.iLink = 0;

		wcscpy(item.szUrl, URL_ANGELSCRIPT);
		SendDlgItemMessage(this->window, LnkAboutAS, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));
		wcscpy(item.szUrl, URL_SCINTILLA);
		SendDlgItemMessage(this->window, LnkAboutSC, LM_SETITEM, NULL, reinterpret_cast<LPARAM>(&item));

		TT_URL[0] = CreateTooltip(GetDlgItem(this->window, LnkAboutAS), DlgAboutTabLibraries::URL_ANGELSCRIPT);
		TT_URL[1] = CreateTooltip(GetDlgItem(this->window, LnkAboutSC), DlgAboutTabLibraries::URL_SCINTILLA);
		*/
		}
		break;

		/*
	case WM_NOTIFY:
		{
		const NMHDR* nmHdr = reinterpret_cast<const NMHDR*>(lParam);
		assert(nmHdr);
		switch(nmHdr->code)
		{
			case NM_CLICK:  // mouse
			case NM_RETURN: // keyboard
			{
				const NMLINK* nmLink = reinterpret_cast<const NMLINK*>(nmHdr);
				LITEM item = nmLink->item;

				assert((nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutAS)) || (nmHdr->hwndFrom == GetDlgItem(this->window, LnkAboutSC)));
				assert(item.iLink == 0);

				ShellExecute(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
				break;
			}
		}
		}
		break;
		*/

		/*
	case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			default:
				return false;
			}
		break;
		*/

	case WM_DESTROY:
		/*
		for(int i = 0; i < _countof(TT_URL); i++)
		{
			DestroyWindow(TT_URL[i]);
		}*/
		break;

	default:
		return false;
	}

	return true;
}
