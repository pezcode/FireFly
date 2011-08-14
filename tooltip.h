#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <windows.h>
#include <tchar.h>

/**
 * Creates a tooltip for a window.
 * 
 * The tooltip stays hidden until the user hovers over the window and automatically
 * disappears when the mouse is moved away.
 * 
 * @param Window
 *   Window for which to create a tooltip.
 * @param Text
 *   (optional) Text to show in the tooltip. If this parameter is NULL,
 *   the specified window will receive WM_NOTIFY with TTN_GETDISPINFO
 *   to set the text.
 * @param Transparent
 *   Creates a transparent tooltip. Transparent tooltips don't
 *   disappear when the user hovers directly over the tooltip window.
 * @param Instance
 *   (optional) Instance to be associated with the window.
 *   
 * @return
 *   Window handle of the tooltip control, NULL on failure.
 */
HWND CreateTooltip(HWND Window, const TCHAR* Text = NULL, bool Transparent = false, HINSTANCE Instance = NULL);

#endif
