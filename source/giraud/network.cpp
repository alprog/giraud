module;
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "kuku_json.h"
module network;

std::string Network::Post(std::string address, std::string postBody)
{
	httplib::Client cli("https://auth.atlassian.com");
	std::string contentType = "application/json";
	auto response = cli.Post(address, postBody, contentType);
	return response->body;
}

std::string Network::GetCloudId()
{
	httplib::Client cli("https://api.atlassian.com");

	std::string address = "/oauth/token/accessible-resources";

	std::string authorizationString = std::format("Bearer {}", session.accessToken);
	httplib::Headers headers = {
		{ "Authorization", authorizationString },
		{ "Accept", "application/json" }
	};

	auto response = cli.Get(address, headers);
	std::string responseText = response->body;
	nlohmann::json responseJson = nlohmann::json::parse(responseText);

	if (responseJson.is_array())
	{
		auto website = from_json<WebSite>(responseJson[0]);
		return website.id;
	}

	return "";
}

User Network::GetCurrentUser()
{
	httplib::Client cli("https://api.atlassian.com");
	std::string address = std::format("/ex/jira/{}/{}", session.cloudId, "rest/api/3/myself");

	std::string authorizationString = std::format("Bearer {}", session.accessToken);
	httplib::Headers headers = {
		{ "Authorization", authorizationString },
		{ "Accept", "application/json" }
	};

	auto response = cli.Get(address, headers);
	std::string responseText = response->body;

	nlohmann::json responseJson = nlohmann::json::parse(responseText);
	return from_json<User>(responseJson);
}

ProjectsResponse Network::GetAllProjects()
{
	httplib::Client cli("https://api.atlassian.com");
	std::string address = std::format("/ex/jira/{}/{}", session.cloudId, "rest/api/3/project/search");

	std::string authorizationString = std::format("Bearer {}", session.accessToken);
	httplib::Headers headers = {
		{ "Authorization", authorizationString },
		{ "Accept", "application/json" }
	};

	auto response = cli.Get(address, headers);
	std::string responseText = response->body;

	nlohmann::json responseJson = nlohmann::json::parse(responseText);
	return from_json<ProjectsResponse>(responseJson);
}

IssuesResponse Network::GetAllEpics(Project project)
{
	httplib::Client cli("https://api.atlassian.com");
	std::string args = "jql?jql=project=JUS%20AND%20issuetype=epic%20AND%20statusCategory=2&maxResults=1000&fields=customfield_11633,summary";
	std::string address = std::format("/ex/jira/{}/{}/{}", session.cloudId, "rest/api/3/search", args);

	std::string authorizationString = std::format("Bearer {}", session.accessToken);
	httplib::Headers headers = {
		{ "Authorization", authorizationString },
		{ "Accept", "application/json" }
	};

	auto response = cli.Get(address, headers);
	std::string responseText = response->body;

	nlohmann::json responseJson = nlohmann::json::parse(responseText);
	return from_json<IssuesResponse>(responseJson);
}