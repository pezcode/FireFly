#ifndef TABS_H
#define TABS_H

#include <windows.h>

/**
 * Adds a tab item to the tab control and associates the window to it.
 * 
 * The window is moved and resized to fit the tab control dimensions.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * @param Window
 *   Window handle of the window to associate with the tab item
 * @param Caption
 *   C-String that contains the tab item's caption
 * @param Index
 *   Index of the new item (indices start at 0).
 *   If Index is -1, it adds the tab item at the end.
 * 
 * @return
 *   Returns the new index or -1 if an error occured.
 */
int AddTab(HWND TabWindow, HWND Window, const TCHAR* Caption, int Index);

/**
 * Removes the tab item from the tab control.
 * 
 * The associated icon (if any) is removed from the image list.
 * If it is the currently selected tab item it selects the next one.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * @param Index
 *   Index of the item to remove
 * 
 * @return
 *   Returns true if no error occured.
 */
bool RemoveTab(HWND TabWindow, int Index);

/**
 * Removes all tab items from the tab control.
 * 
 * Cleans up internal resources and hides all windows associated with it.
 * You should call this function before the tab control is destroyed.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * 
 * @return
 *   Returns true if no error occured.
 */
bool TabCleanup(HWND TabWindow);

/**
 * Sets the tab item's icon.
 * 
 * Supports 32bit icons (24bit + 8bit alpha) if comctl32.dll v6 is used (ie. visual styles enabled).
 * The icon should be 16x16 in size and at least contain an 8bit (256 colors) channel.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * @param Index
 *   Index of the item to set the icon for
 * @param Icon
 *   The icon to set for the tab item.
 *   If Icon is 0, it removes the current icon.
 * 
 * @return Returns true if no error occured.
 */
bool SetTabIcon(HWND TabWindow, int Index, HICON Icon);

/**
 * Selects the specified tab and shows the appropiate window.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * @param Index
 *   Index of the item to select.
 *   If Index is -1, it show's the currently selected tab's window.
 * 
 * @return
 *   Returns the selected tab item index or -1 if an error occured.
 */
int TabToFront(HWND TabWindow, int Index);

/**
 * Returns the window associated to the tab item.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * @param Index
 *   Index of the item to query.
 * 
 * @return
 *   Returns the tab item's window handle.
 */
HWND GetTabWindow(HWND TabWindow, int Index);

/**
 * Returns the window associated to the currently selected tab item.
 * 
 * @param TabWindow
 *   Window handle of the tab control
 * 
 * @return
 *   Returns the tab's front item's window handle.
 */
HWND GetTabFrontWindow(HWND TabWindow);

#endif
