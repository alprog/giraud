module;
#include "nlohmann/json.h"
#include "kuku_json.h"
export module network;

import std;
import configuration;
import requests;
import session;
import user;
import project;
import issue;

// for intellisense

export class Network
{
public:
	explicit Network(const Configuration& config)
		: config{ config }
	{
		session = json::loadFromFile<Session>("session.txt");
	}

	const Configuration& GetConfig() const { return config; }
	const Session& GetSession() const { return session; }
	Session& GetSession() { return session; }

	bool IsLoggedIn() const
	{
		return session.IsLoggedIn();
	}

	void Login(std::string code)
	{
		TokenRequest request;
		request.grant_type = "authorization_code";
		request.client_id = config.app.id;
		request.client_secret = config.app.secret;
		request.code = code;
		request.redirect_uri = config.app.redirect_uri;
		TokenExchange(request);
	}

	void RefreshTokens()
	{
		TokenRequest request;
		request.grant_type = "refresh_token";
		request.client_id = config.app.id;
		request.client_secret = config.app.secret;
		request.refresh_token = session.refreshToken;
		TokenExchange(request);
	}

	void TokenExchange(TokenRequest request)
	{
		TokenResponse response = Post(request);
		session.accessToken = response.access_token;
		session.refreshToken = response.refresh_token;
		session.expirationTime = Timestamp::now() + response.expires_in;

		session.cloudId = GetCloudId();

		json::saveToFile(session, "session.txt");
	}

	void Logout()
	{
		session.accessToken = {};
	}

	TokenResponse Post(TokenRequest Request)
	{
		std::string requestText = Request.to_json().dump();
		std::string responseText = Post("/oauth/token", requestText);
		nlohmann::json responseJson = nlohmann::json::parse(responseText);
		return from_json<TokenResponse>(responseJson);
	}

	User GetCurrentUser();
	ProjectsResponse GetAllProjects();
	std::vector<IssueDesc> GetAllEpics(const Project* project);
	std::vector<IssueDesc> GetAllSubTasks(const Issue* issue);

	std::string Post(std::string address, std::string bodyText);
	std::string GetCloudId();

private:
	nlohmann::json GetRequestInternal(std::string address);

private:
	const Configuration& config;
	Session session;
};