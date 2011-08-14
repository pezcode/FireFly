#ifndef DIALOGS_H
#define DIALOGS_H

#include "Dialog.h"
#include "resource.h"

/*
 * Main dialog (editor, debug controls, etc.)
 */
class DlgMain : public Dialog
{
public:
	DlgMain(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_MAIN, parent) { }
	//bool show();

private:
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
	//static const wchar_t TEXT[];

	/*
	 * Info Dialog for About tab (description, version, URL)
	 */
	class TabInfo : public Dialog
	{
	public:
		TabInfo(HINSTANCE instance, HWND parent) : Dialog(instance, IDD_ABOUT_INFO, parent) { }
	private:
		using Dialog::modal; //only allow modeless
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
