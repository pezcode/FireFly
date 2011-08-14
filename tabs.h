#ifndef TABS_H
#define TABS_H

#include <windows.h>

int  AddTab(HWND TabWindow, HWND Window, const TCHAR* Caption, int Index);
bool SetTabIcon(HWND TabWindow, int Index, HICON Icon);
int  TabToFront(HWND TabWindow, int Index);
HWND GetTabWindow(HWND TabWindow, int Index);
HWND GetTabFrontWindow(HWND TabWindow);
bool RemoveTab(HWND TabWindow, int Index);
bool TabCleanup(HWND TabWindow);

#endif
