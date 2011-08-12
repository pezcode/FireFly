#ifndef FIREFLYLANG_H
#define FIREFLYLANG_H

#include "FireFly.h"

class asIScriptEngine;

class FireFlyLang
{
public:

	FireFlyLang(FireFly& interface);
	~FireFlyLang();

	bool register_functions();

	/* script functions */
	static void print(const string& in);

private:
	FireFly& interface;
	asIScriptEngine* engine;
};

#endif
