#include "tooltip.h"

#include <commctrl.h>
#include <cassert>

HWND CreateTooltip(HWND Window, const TCHAR* Text, bool Transparent, HINSTANCE Instance)
{
	assert(IsWindow(Window));

	HWND TTWindow = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
	                               WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
	                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                               Window, NULL, Instance, NULL);

	if(TTWindow)
	{
		TOOLINFO ti;
		ti.cbSize   = sizeof(TOOLINFO);
		ti.uFlags   = TTF_SUBCLASS | (Transparent ? TTF_TRANSPARENT : 0); // Don't hide the tooltip on mouse over
		ti.hwnd     = Window;
		ti.hinst    = Instance;
		ti.uId      = 0;
		ti.lpszText = Text ? const_cast<TCHAR*>(Text) : LPSTR_TEXTCALLBACK;
		GetClientRect(Window, &ti.rect);

		SendMessage(TTWindow, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
	}
	return TTWindow;
}
