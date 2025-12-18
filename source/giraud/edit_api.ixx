export module edit_api;

import configuration;
import database;
import selection;
import network;
import utils;

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
		for (auto desc : network.GetAllProjects().values)
		{
			int id = std::atoi(desc.id.c_str());
			access_or_create(database.projects, id).update(desc);
		}
	}

	void UpdateEpics()
	{
		for (auto& [key, project] : database.projects)
		{
			network.GetAllEpics(*project);
			return;
		}
	}

private:
	Database& database;
	Selection& selection;
	Network& network;
};