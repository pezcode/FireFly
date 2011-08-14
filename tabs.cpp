#include "tabs.h"

#include <windows.h>
#include <commctrl.h> // Tabs
#include <uxtheme.h>  // EnableThemeDialogTexture
#include <shlwapi.h>  // DllGetVersion
#include <tchar.h>
#include <cassert>

/**
 * Enables/disables the theme-defined background texture for a dialog.
 * 
 * The dialog texture is a theme-specific background image that should be applied
 * to child dialogs of themed tab controls. This makes sure the child dialogs
 * visually fit into the frame and resemble the standard Windows appearance.
 * 
 * @param Window
 *   Handle of the dialog for which to set the texture.
 * @param Enable
 *   Enable or disable theme background.
 * 
 * @return
 *   Returns true if no error occured.
 */
bool SetTabThemeTexture(HWND Window, bool Enable);

/**
 * Checks if visual styles are enabled in the current process.
 * 
 * Visual styles exist on Windows XP and above to support theming of common controls.
 * 
 * @return
 *   Returns true if visual styles are supported and enabled.
 */
bool VisualStylesEnabled();

const TCHAR PropName[] = _T("TabCtrlCurWnd");

int AddTab(HWND TabWindow, HWND Window, const TCHAR* Caption, int Index)
{
	assert(IsWindow(TabWindow));
	assert(IsWindow(Window));
	assert(Caption);

	if(Index == -1)
		Index = TabCtrl_GetItemCount(TabWindow);

	assert(Index >= 0);

	TC_ITEM TabData;
	TabData.mask       = TCIF_TEXT | TCIF_PARAM;
	TabData.pszText    = const_cast<TCHAR*>(Caption);
	TabData.cchTextMax = _tcslen(Caption) + 1;
	TabData.lParam     = reinterpret_cast<LPARAM>(Window);

	Index = TabCtrl_InsertItem(TabWindow, Index, &TabData);
	if(Index != -1)
	{
		RECT TabRect;
		GetWindowRect(TabWindow, &TabRect);
		MapWindowPoints(HWND_DESKTOP, GetParent(TabWindow), reinterpret_cast<POINT*>(&TabRect), 2);
		TabCtrl_AdjustRect(TabWindow, false, &TabRect);
		TabRect.right  -= TabRect.left; // .right  == width
		TabRect.bottom -= TabRect.top;  // .bottom == heigth
		SetWindowPos(Window, HWND_BOTTOM, TabRect.left, TabRect.top, TabRect.right, TabRect.bottom, SWP_HIDEWINDOW);
		SetProp(TabWindow, PropName, Window);
		SetTabThemeTexture(Window, true);
	}
	return Index;
}

bool SetTabIcon(HWND TabWindow, int Index, HICON Icon)
{
	assert(IsWindow(TabWindow));
	assert(Index >= 0);
	assert(Index < TabCtrl_GetItemCount(TabWindow));

	HIMAGELIST ImageList = TabCtrl_GetImageList(TabWindow);
	if(!ImageList) // Create a new image list
	{
		ImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1);
		TabCtrl_SetImageList(TabWindow, ImageList);
	}

	TC_ITEM TabData;
	TabData.mask = TCIF_IMAGE;

	if(TabCtrl_GetItem(TabWindow, Index, &TabData))
	{
		if(Icon) // Add icon
		{
			if(TabData.iImage == -1) // There is no icon yet
				TabData.iImage = ImageList_AddIcon(ImageList, Icon);
			else
				TabData.iImage = ImageList_ReplaceIcon(ImageList, TabData.iImage, Icon);
		}
		else // Remove icon
		{
			if(TabData.iImage != -1) // Skip if there is no icon
			{
				TabCtrl_RemoveImage(TabWindow, TabData.iImage);
				TabData.iImage = -1;
			}
		}
		return TabCtrl_SetItem(TabWindow, Index, &TabData);
	}
	return false;
}

int TabToFront(HWND TabWindow, int Index)
{
	assert(IsWindow(TabWindow));

	if(Index == -1)
		Index = TabCtrl_GetCurSel(TabWindow);

	if(Index == -1)
		Index = 0;

	assert(Index >= 0);
	assert(Index < TabCtrl_GetItemCount(TabWindow));

	TC_ITEM TabData;
	TabData.mask = TCIF_PARAM;

	if(TabCtrl_GetItem(TabWindow, Index, &TabData))
	{
		ShowWindow(static_cast<HWND>(GetProp(TabWindow, PropName)), SW_HIDE);
		SetWindowPos(reinterpret_cast<HWND>(TabData.lParam), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		SetProp(TabWindow, PropName, reinterpret_cast<HANDLE>(TabData.lParam));
		TabCtrl_SetCurSel(TabWindow, Index);
		return Index;
	}
	return -1;
}

HWND GetTabWindow(HWND TabWindow, int Index)
{
	assert(IsWindow(TabWindow));
	assert(Index >= 0);
	assert(Index < TabCtrl_GetItemCount(TabWindow));

	TC_ITEM TabData;
	TabData.mask = TCIF_PARAM;
	if(TabCtrl_GetItem(TabWindow, Index, &TabData))
	{
		return reinterpret_cast<HWND>(TabData.lParam);
	}
	return NULL; 
}

HWND GetTabFrontWindow(HWND TabWindow)
{
	assert(IsWindow(TabWindow));

	return static_cast<HWND>(GetProp(TabWindow, PropName));
}

bool RemoveTab(HWND TabWindow, int Index)
{
	assert(IsWindow(TabWindow));
	assert(Index >= 0);
	assert(Index < TabCtrl_GetItemCount(TabWindow));

	TC_ITEM TabData;
	TabData.mask = TCIF_IMAGE | TCIF_PARAM;

	if(TabCtrl_GetItem(TabWindow, Index, &TabData))
	{
		int CurIndex = TabCtrl_GetCurSel(TabWindow);
		assert(CurIndex >= -1);
		if(CurIndex != -1 && TabCtrl_DeleteItem(TabWindow, Index))
		{
			if(TabData.iImage != -1)
				TabCtrl_RemoveImage(TabWindow, TabData.iImage);

			int Count = TabCtrl_GetItemCount(TabWindow);
			assert(Count >= 0);
			if(Count == 0) // We just removed the last one, do some addition cleanup
			{
				ShowWindow(reinterpret_cast<HWND>(TabData.lParam), SW_HIDE);
				HIMAGELIST ImageList = TabCtrl_GetImageList(TabWindow);
				if(ImageList)
				{
					ImageList_Destroy(ImageList);
					TabCtrl_SetImageList(TabWindow, NULL);
				}
				RemoveProp(TabWindow, PropName);
			}
			else if(Index == CurIndex) // We're deleting the currently visible tab
			{
				if(Index == Count) // Last tab
					Index--;
				TabToFront(TabWindow, Index);
			}

			SetTabThemeTexture(reinterpret_cast<HWND>(TabData.lParam), false);
			return true;
		}
	}
	return false;
}

bool TabCleanup(HWND TabWindow)
{
	assert(IsWindow(TabWindow));

	int Count = TabCtrl_GetItemCount(TabWindow);
	assert(Count >= 0);

	bool Result = true;
	for(int i = 1; i <= Count; i++)
	{
		Result = RemoveTab(TabWindow, Count-i) && Result;
	}

	return Result;
}

bool SetTabThemeTexture(HWND Window, bool Enable)
{
	assert(IsWindow(Window));

	if(VisualStylesEnabled())
	{
		return (S_OK == EnableThemeDialogTexture(Window, Enable ? ETDT_ENABLETAB : ETDT_DISABLE));
	}
	return false;
}

bool VisualStylesEnabled()
{
	HMODULE hMod = GetModuleHandle(_T("comctl32.dll"));
	if(hMod)
	{
		DLLGETVERSIONPROC pDllGetVersion = reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hMod, "DllGetVersion"));
		if(pDllGetVersion)
		{
			DLLVERSIONINFO DllVersion;
			DllVersion.cbSize = sizeof(DllVersion);
			if(pDllGetVersion(&DllVersion) == S_OK)
			{
				return (DllVersion.dwMajorVersion >= 6);
			}
		}
	}
	return false;
}
