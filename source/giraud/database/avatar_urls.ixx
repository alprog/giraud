module;
#include "../kuku_json.h"
export module avatar_urls;

// for intellisense

export struct AvatarUrls : public json::serializable
{
	std::string _16x16;
	std::string _24x24;
	std::string _32x32;
	std::string _64x64;

	JSCHEME(_16x16, _24x24, _32x32, _64x64);
};