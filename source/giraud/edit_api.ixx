export module edit_api;

import configuration;
import database;
import network;

// for intellisense

export class EditAPI
{
public:
	EditAPI(Database& database, Network& network)
		: database{ database }
		, network{ network }
	{
	}

	const Configuration& GetConfig() const { return network.GetConfig(); }
	const Database& GetDatabase() const { return database; }
	const Network& GetNetwork() const { return network; }
	Network& GetNetwork() { return network; }

	void UpdateMyself()
	{
		database.myself = network.GetCurrentUser();
	}

	void UpdateProjectList()
	{
		database.projects = network.GetAllProjects().values;
	}

private:
	Database& database;
	Network& network;
};