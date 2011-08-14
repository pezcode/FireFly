#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <windows.h>
#include <tchar.h>

HWND CreateTooltip(HWND Window, HINSTANCE Instance, const TCHAR* Text, bool Transparent);

#endif
