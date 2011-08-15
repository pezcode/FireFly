#ifndef DIALOGS_H
#define DIALOGS_H

#include "Dialog.h"
#include "resource.h"

#include <scintilla.h>

/*
 * Main dialog (editor, debug controls, etc.)
 */
class DlgMain : public Dialog
{
public:
	DlgMain(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_MAIN, parent) { }

	// WTF!
	template<typename TW, typename TL> LRESULT SendEditor(UINT message, TW wParam, TL lParam)
	{
		return SendMessage(hEditor, message, WPARAM(wParam), LPARAM(lParam));
	}
	template<typename TW> LRESULT SendEditor(UINT message, TW wParam)
	{
		return SendEditor(message, WPARAM(wParam), 0);
	}
	LRESULT SendEditor(UINT message)
	{
		return SendEditor(message, 0, 0);
	}

	void set_editor_color(int style, COLORREF foreground, COLORREF background = RGB(255, 255, 255))
	{
		SendEditor(SCI_STYLESETFORE, style, foreground);
		SendEditor(SCI_STYLESETBACK, style, background);
	}

private:
	HWND hEditor;
	INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
};

/*
 * About dialog (holds title and tab control)
 */
class DlgAbout : public Dialog
{
public:
	DlgAbout(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_ABOUT, parent) { }

private:
	using Dialog::modeless; //only allow modal
	INT_PTR dlg_proc(UINT, WPARAM, LPARAM);

	/*
	 * Info Dialog for About tab (description, version, URL)
	 */
	class TabInfo : public Dialog
	{
	public:
		TabInfo(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_ABOUT_INFO, parent) { }
	private:
		using Dialog::modal; //only allow modeless
		static const wchar_t URL_GITHUB[];
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
	};

	/*
	 * 3rd-party libs Dialog for About tab (description, version, URL)
	 */
	class TabLibraries : public Dialog
	{
	public:
		TabLibraries(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_ABOUT_LIBS, parent) { }
	private:
		using Dialog::modal; //only allow modeless
		static const wchar_t URL_ANGELSCRIPT[];
		static const wchar_t URL_SCINTILLA[];
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
	};

	/*
	 * License Dialog for About tab (text, explanation)
	 */
	class TabLicense : public Dialog
	{
	public:
		TabLicense(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_ABOUT_LIC, parent) { }
	private:
		using Dialog::modal; //only allow modeless
		static const wchar_t LICENSE_TEXT[];
		INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
	};
};

class DlgInput : public Dialog
{
public:
	DlgInput(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_INPUT, parent) { }
	string show(const string& text, const string& title = DEFTITLE);

	bool accepted() const;
	string input() const;

private:
	INT_PTR dlg_proc(UINT, WPARAM, LPARAM);
};

class DlgQuestion
{
	// plain messagebox (yes, no, cancel)
};

class DlgMessage
{
	// plain messagebox
};

#endif
