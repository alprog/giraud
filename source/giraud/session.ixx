export module session;

import std;
import timestamp;

export class Session
{
public:
	bool IsLoggedIn() const
	{
		return !accessToken.empty();
	}

	std::string accessToken;
	Timestamp expirationTime;
};