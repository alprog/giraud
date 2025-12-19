export module edit_api;

import configuration;
import database;
import network;
import utils;
import project;
import utils;
import issue;
import selection;
import folders_type;

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
			database.projects[id]->update(desc);
		}

		database.treeview.clear();
		for (auto& [id, project] : database.projects)
		{
			database.treeview.push_back(project);
		}
		sort_ascending(database.treeview);
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
				auto epic = database.issues[id];
				epic->update(desc);
				project->children.push_back(epic);
			}
			database.CreateSubFolders(project, EFoldersType::EpicFolders);
		}
	}

	void UpdateSubTasks()
	{
		auto issue = dynamic_cast<Issue*>(selection.item);
		if (issue)
		{
			issue->children.clear();
			for (auto& desc : network.GetAllSubTasks(issue))
			{
				int id = std::atoi(desc.id.c_str());
				auto subtask = database.issues[id];
				subtask->update(desc);
				issue->children.push_back(subtask);
			}
		}
	}

private:
	Database& database;
	Selection& selection;
	Network& network;
};