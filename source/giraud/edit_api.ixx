export module edit_api;

import configuration;
import database;
import selection;
import network;
import utils;
import project;
import utils;
import issue;

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
		auto project = dynamic_cast<Project*>(selection.item);
		if (project)
		{
			project->children.clear();
			for (auto& desc : network.GetAllEpics(project))
			{
				int id = std::atoi(desc.id.c_str());
				auto& epic = access_or_create(project->issues, id);
				epic.update(desc);
				project->children.push_back(&epic);
			}
		}
	}

private:
	Database& database;
	Selection& selection;
	Network& network;
};