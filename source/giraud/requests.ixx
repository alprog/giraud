module;
#include "kuku_json.h"
export module requests;

import std;
import project;

export import json;

export struct WebSite : public json::serializable
{
	std::string id;
	std::string name;
	std::string url;

	JSCHEME(id, name, url);
};

export struct TokenRequest : public json::serializable
{
	std::string grant_type;
	std::string client_id;
	std::string client_secret;
	std::string code;
	std::string redirect_uri;

	JSCHEME(grant_type, client_id, client_secret, code, redirect_uri);
};

export struct TokenResponse : public json::serializable
{
	std::string access_token;
	int expires_in;
	std::string token_type;
	std::string scope;

	JSCHEME(access_token, expires_in, token_type, scope);
};

export struct ProjectsResponse : public json::serializable
{
	std::vector<Project> values;

	JSCHEME(values);
};