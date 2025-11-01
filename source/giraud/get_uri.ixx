export module get_uri;

import std;

// for intellisense

export struct GetUri
{
	GetUri(std::string baseUri)
		: baseUri{ baseUri }
	{
	}

	void AddParam(std::string key, std::string value)
	{
		params[key] = value;
	}

	std::string BuildFullUrl()
	{
		std::stringstream ss;

		ss << baseUri;

		bool first = true;
		for (auto& param : params)
		{
			ss << (first ? "?" : "&") << param.first << "=" << param.second;
			first = false;
		}

		return ss.str();
	}

	std::string baseUri;
	std::unordered_map<std::string, std::string> params;
};