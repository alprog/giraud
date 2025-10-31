module;
#include "kuku_json.h"
export module session;

import std;
import timestamp;

// for intellisense

export class Session : public json::serializable
{
public:
	bool IsLoggedIn() const
	{
		return !accessToken.empty();
	}

	std::string accessToken;
	std::string refreshToken;
	Timestamp expirationTime;

	JSCHEME(accessToken, refreshToken, expirationTime);
};