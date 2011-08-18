#ifndef DIALOGS_H
#define DIALOGS_H

#include "Dialog.h"
#include "resource.h"

//#include "FireFly.h"

#include <scintilla.h>

class FireFly;

/*
 * Main dialog (editor, debug controls, etc.)
 */
class DlgMain : public Dialog
{
public:
	DlgMain(HINSTANCE instance, HWND parent, FireFly& interface) : interface(interface), Dialog(instance, IDD_MAIN, parent) { }

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
	FireFly& interface;
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

class DlgQuestion // yes, no, cancel
{
public:
	enum eType { tInfo, tWarning, tError, tQuestion };
	enum eChoice { cOk, cOkCancel, cRetryCancel, cYesNo, cYesNoCancel, cAbortRetryIgnore };
	enum eResult { rOk, rCancel, rYes, rNo, rAbort, rRetry, rIgnore };
	DlgQuestion(HWND parent) : parent(parent) { }
	static eResult show(HWND parent, const wchar_t* text, const wchar_t* title, eType type = tInfo, eChoice choice = cOk)
	{
		UINT mb_type = MB_OK;
		switch(type)
		{
		case tInfo:     mb_type |= MB_ICONINFORMATION; break;
		case tWarning:  mb_type |= MB_ICONWARNING;     break;
		case tError:    mb_type |= MB_ICONERROR;       break;
		case tQuestion: mb_type |= MB_ICONQUESTION;    break;
		}
		switch(choice)
		{
		case cOk:               mb_type |= MB_OK;               break;
		case cOkCancel:         mb_type |= MB_OKCANCEL;         break;
		case cRetryCancel:      mb_type |= MB_RETRYCANCEL;      break;
		case cYesNo:            mb_type |= MB_YESNO;            break;
		case cYesNoCancel:      mb_type |= MB_YESNOCANCEL;      break;
		case cAbortRetryIgnore: mb_type |= MB_ABORTRETRYIGNORE; break;
		}
		eResult result = rOk;
		switch(MessageBox(parent, text, title, mb_type))
		{
		case IDABORT:  result = rAbort;  break;
		case IDCANCEL: result = rCancel; break;
		case IDIGNORE: result = rIgnore; break;
		case IDNO:     result = rNo;     break;
		case IDOK:     result = rOk;     break;
		case IDRETRY:  result = rRetry;  break;
		case IDYES:    result = rYes;    break;
		}
		return result;
	}
	bool show(const wchar_t* text, const wchar_t* title, eType type = tInfo, eChoice choice = cOk)
	{
		this->result = DlgQuestion::show(this->parent, text, title, type, choice);
		return true;
	}
	eResult get_result() const { return this->result; }
private:
	HWND parent;
	eResult result;
};

/*
class DlgMessage // plain messagebox
{
public:
	enum eType { Msg_Info, Msg_Warning, Msg_Error };
	DlgMessage(HINSTANCE instance, HWND parent) : instance(instance), parent(parent) { }
	bool show(const wchar_t* text, const wchar_t* title = L"", eType type = Msg_Info)
	{
		UINT mb_type = MB_ICONINFORMATION;
		switch(type)
		{
		case Msg_Info:
			mb_type = MB_ICONINFORMATION; break;
		case Msg_Warning:
			mb_type = MB_ICONWARNING; break;
		case Msg_Error:
			mb_type = MB_ICONERROR; break;
		}
		return (MB_OK == MessageBox(this->parent, text, title, mb_type));
	}
private:
	HINSTANCE hinstance;
	HWND parent;
};
*/

class DlgFileOpen
{
public:
private:
};

class DlgFileSave
{
public:
private:
};

#endif
