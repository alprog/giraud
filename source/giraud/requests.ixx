module;
#include "kuku_json.h"
export module requests;

import std;
import project;

export import json;

// for intellisense

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
	std::string refresh_token;

	JSCHEME(grant_type, client_id, client_secret, code, redirect_uri, refresh_token);
};

export struct TokenResponse : public json::serializable
{
	std::string access_token;
	std::string refresh_token;
	int expires_in;
	std::string token_type;
	std::string scope;

	JSCHEME(access_token, refresh_token, expires_in, token_type, scope);
};

export struct RefreshTokenRequest : public json::serializable
{
	std::string grant_type;
	std::string client_id;
	std::string client_secret;
	std::string code;
	std::string redirect_uri;

	JSCHEME(grant_type, client_id, client_secret, code, redirect_uri);
};

export struct ProjectsResponse : public json::serializable
{
	std::vector<Project> values;

	JSCHEME(values);
};

export struct IssueFields : public json::serializable
{
	std::string summary;

	JSCHEME(summary);
};

export struct IssueDesc : public json::serializable
{
	std::string id;
	std::string key;
	IssueFields fields;

	JSCHEME(id, key, fields);
};

export struct IssuesResponse : public json::serializable
{
	bool isLast;
	std::string nextPageToken;
	std::vector<IssueDesc> issues;

	JSCHEME(issues, isLast, nextPageToken);
};