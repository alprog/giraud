module;
#include "../kuku_json.h"
export module project;

import std;

// for intellisense

export struct Project : public json::serializable
{
	std::string id;
	std::string key;
	std::string name;
	std::string description;

	JSCHEME(id, key, name, description);
};