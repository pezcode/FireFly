#include <windows.h>
#include <string>

class FireFly
{
public:
	FireFly(HINSTANCE instance, HWND parent);
	~FireFly();

	//bool show_main();
	//bool close_main();

	void about();

	bool info(const std::string& str);
	bool warning(const std::string& str);
	bool error(const std::string& str);

	bool ask(const std::string& str);

	bool input(const std::string& str);

	void run_script();

public:

	static const wchar_t VERSION[];

private:
	HINSTANCE instance;
	HWND parent;

	HWND hwnd_main;

	static INT_PTR CALLBACK dialog_func(HWND, UINT, WPARAM, LPARAM);
};
