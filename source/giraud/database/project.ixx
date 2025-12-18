module;
#include "../kuku_json.h"
export module project;

import std;
import avatar_urls;

// for intellisense

export struct Project : public json::serializable
{
	std::string id;
	std::string key;
	std::string name;
	std::string description;
	AvatarUrls avatarUrls;

	JSCHEME(id, key, name, description, avatarUrls);
};