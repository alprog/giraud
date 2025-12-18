export module edit_api;

import configuration;
import database;
import selection;
import network;

// for intellisense

export class EditAPI
{
public:
	EditAPI(Database& database, Selection& selection, Network& network)
		: database{ database }
		, selection{ selection }
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

	void UpdateEpics()
	{
		network.GetAllEpics(database.projects[0]);
	}

private:
	Database& database;
	Selection& selection;
	Network& network;
};