#ifndef FIREFLYLANG_H
#define FIREFLYLANG_H

//#include "FireFly.h"

class FireFly;
class asIScriptEngine;
//class std::string;
#include <string>

class FireFlyLang
{
friend class FireFly;
public:

	FireFlyLang(FireFly& interface);
	~FireFlyLang();

	static const char KEYWORDS[];

	bool register_functions();

	bool script_load(const std::string& script);
	bool script_compile();
	bool script_run();

	/* script functions */
	void print(const std::string& in);

private:
	FireFly& interface;
	asIScriptEngine* engine;
};

#endif
