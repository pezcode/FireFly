#include "tooltip.h"

#include <commctrl.h>

HWND CreateTooltip(HWND Window, HINSTANCE Instance, const TCHAR* Text, bool Transparent)
{
HWND TTWindow;
TOOLINFO ti;

	TTWindow = CreateWindow(/*WS_EX_TOPMOST, */TOOLTIPS_CLASS, NULL,
	                          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
	                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                          Window, NULL, Instance, NULL);

	// "You must explicitly define a ToolTip control as topmost. Otherwise, it might be covered by the parent window." ???
	//SetWindowPos(TTWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	ti.cbSize   = sizeof(TOOLINFO);
	ti.uFlags   = TTF_SUBCLASS | (Transparent ? TTF_TRANSPARENT : 0); // Don't hide the tooltip on mouse over
	ti.hwnd     = Window;
	ti.hinst    = Instance;
	ti.uId      = 0;
	ti.lpszText = Text ? const_cast<TCHAR*>(Text) : LPSTR_TEXTCALLBACK;
	GetClientRect(Window, &ti.rect);

	SendMessage(TTWindow, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));

	return TTWindow;
}
