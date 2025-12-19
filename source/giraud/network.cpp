module;
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "kuku_json.h"
module network;

import jql;
import get_uri;

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
	std::string address = std::format("/ex/jira/{}/rest/api/3/{}", session.cloudId, "myself");
	return from_json<User>(GetRequestInternal(address));
}

ProjectsResponse Network::GetAllProjects()
{
	std::string address = std::format("/ex/jira/{}/rest/api/3/{}", session.cloudId, "project/search");
	return from_json<ProjectsResponse>(GetRequestInternal(address));
}

std::vector<IssueDesc> Network::GetAllEpics(const Project* project)
{
	std::vector<IssueDesc> descs;

	GetUri uri(std::format("/ex/jira/{}/rest/api/3/{}", session.cloudId, "search/jql"));
	uri.AddParam("jql", JQL().project(project).statusCategory(2).issuetype("epic").toString());
	uri.AddParam("maxResults", "1000");
	uri.AddParam("fields", "customfield_11633,summary");

	while (true)
	{
		std::string address = uri.BuildFullUrl();
		IssuesResponse response = from_json<IssuesResponse>(GetRequestInternal(address));
		descs.insert(descs.end(), response.issues.begin(), response.issues.end());

		if (response.isLast)
			break;
		uri.AddParam("nextPageToken", response.nextPageToken);
	}

	return descs;
}

std::vector<IssueDesc> Network::GetAllSubTasks(const Issue* issue)
{
	std::vector<IssueDesc> descs;

	GetUri uri(std::format("/ex/jira/{}/rest/api/3/{}", session.cloudId, "search/jql"));
	uri.AddParam("jql", JQL().parent(issue).statusCategory(2).toString());
	uri.AddParam("maxResults", "1000");
	uri.AddParam("fields", "customfield_11633,summary");

	while (true)
	{
		std::string address = uri.BuildFullUrl();
		IssuesResponse response = from_json<IssuesResponse>(GetRequestInternal(address));
		descs.insert(descs.end(), response.issues.begin(), response.issues.end());

		if (response.isLast)
			break;
		uri.AddParam("nextPageToken", response.nextPageToken);
	}

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