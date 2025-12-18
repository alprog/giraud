export module get_uri;

import std;
import utils;

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

		std::string result = ss.str();
		if (result.contains(' '))
		{
			replace(result, " ", "%20");
		};
		return result;
	}

	std::string baseUri;
	std::unordered_map<std::string, std::string> params;
};