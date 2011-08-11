#include "FireFly.h"

class asIScriptEngine;

class FireFlyLang
{
public:

	FireFlyLang();
	~FireFlyLang();

	bool register_functions();

	/* script functions */
	static void print(const string& in);

private:
	FireFly& interface;
	asIScriptEngine* engine;
};
