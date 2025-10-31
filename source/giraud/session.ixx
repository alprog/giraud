export module session;

import std;

export class Session
{
public:
	bool IsLoggedIn() const
	{
		return !accessToken.empty();
	}

	std::string accessToken;
};