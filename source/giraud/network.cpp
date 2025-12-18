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
	std::string address = "/oauth/token/accessible-resources";
	nlohmann::json responseJson = GetRequestInternal(address);
	if (responseJson.is_array())
	{
		auto website = from_json<WebSite>(responseJson[0]);
		return website.id;
	}

	return "";
}

User Network::GetCurrentUser()
{
	std::string address = std::format("/ex/jira/{}/{}", session.cloudId, "rest/api/3/myself");
	return from_json<User>(GetRequestInternal(address));
}

ProjectsResponse Network::GetAllProjects()
{
	std::string address = std::format("/ex/jira/{}/{}", session.cloudId, "rest/api/3/project/search");
	return from_json<ProjectsResponse>(GetRequestInternal(address));
}

std::vector<IssueDesc> Network::GetAllEpics(const Project* project)
{
	std::vector<IssueDesc> descs;

	constexpr const char* fmt = "jql?jql=project={}%20AND%20issuetype=epic%20AND%20statusCategory=2&maxResults=1000&fields=customfield_11633,summary";
	std::string args = std::format(fmt, project->key);
	std::string address = std::format("/ex/jira/{}/{}/{}", session.cloudId, "rest/api/3/search", args);
	IssuesResponse response = from_json<IssuesResponse>(GetRequestInternal(address));

	descs.insert(descs.end(), response.issues.begin(), response.issues.end());

	return descs;
}

nlohmann::json Network::GetRequestInternal(std::string address)
{
	httplib::Client cli("https://api.atlassian.com");
	std::string authorizationString = std::format("Bearer {}", session.accessToken);
	httplib::Headers headers = {
		{ "Authorization", authorizationString },
		{ "Accept", "application/json" }
	};

	auto response = cli.Get(address, headers);
	std::string responseText = response->body;
	return nlohmann::json::parse(responseText);
}