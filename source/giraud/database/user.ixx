module;
#include "../kuku_json.h"
export module user;

import std;
import avatar_urls;

// for intellisense

export struct User : public json::serializable
{
	std::string accountId;
	std::string displayName;
	std::string emailAddress;
	AvatarUrls avatarUrls;

	bool isValid() const
	{
		return accountId != "";
	}

	JSCHEME(accountId, displayName, emailAddress, avatarUrls);
};