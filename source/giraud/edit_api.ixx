export module edit_api;

import configuration;
import database;
import network;

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

	void UpdateProjectList()
	{
		database.projects = network.GetAllProjects().values;
	}

private:
	Database& database;
	Network& network;
};