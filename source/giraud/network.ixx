module;
#include "nlohmann/json.h"
#include "kuku_json.h"
export module network;

import std;
import configuration;
import requests;
import session;

// for intellisense

export class Network
{
public:
	explicit Network(const Configuration& config)
		: config{ config }
	{
	}

	const Configuration& GetConfig() const { return config; }
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

		TokenResponse response = Post(request);
		session.accessToken = response.access_token;
		session.refreshToken = response.refresh_token;
		session.expirationTime = Timestamp::now() + response.expires_in;

		cloudId = GetCloudId();
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

	ProjectsResponse GetAllProjects();

	std::string Post(std::string address, std::string bodyText);
	std::string GetCloudId();

private:
	const Configuration& config;
	Session session;
	std::string cloudId;
};