module;
#include "kuku_json.h"
export module jira_app;

import std;
import get_uri;
export import json;

export struct JiraApp : public json::serializable
{
	std::string id;
	std::string secret;
	std::string redirect_uri;

	bool IsValid()
	{
		return !id.empty() && !secret.empty();
	}

	std::string getAuthUrl()
	{
		GetUri uri("https://auth.atlassian.com/authorize");
		uri.AddParam("audience", "api.atlassian.com");
		uri.AddParam("client_id", id);
		uri.AddParam("scope", "read%3Ajira-work%20write%3Ajira-work%20read%3Ajira-user%20offline_access");
		uri.AddParam("redirect_uri", "https%3A%2F%2Falprog.github.io%2Fgiraud%2Fauth.html");
		uri.AddParam("state", "${YOUR_USER_BOUND_VALUE}");
		uri.AddParam("response_type", "code");
		uri.AddParam("prompt", "consent");
		return uri.BuildFullUrl();
	}

	JSCHEME(id, secret, redirect_uri)
};

export JiraApp GetPublicGiraudApp()
{
	// secret "encrypted" against automatic bots, but it is practically public for any human intruder
	// do not use PublicGiraudApp for any sensetive project
	// create your secure app at https://developer.atlassian.com/console instead

	JiraApp app;
	app.id = "9kRa3WQXnsoGo2SS690tLszrk05tcjhx";
	app.secret = "@SN@Wll682jN5x044YTqvTcUYmnKnAkdBm1s36sSSquQgtI5NEhCHPDbeBG5RyhdKanXE152DCC6";
	for (auto& c : app.secret)
	{
		c++;
	}
	app.redirect_uri = "https://alprog.github.io/giraud/auth.html";
	return app;
}