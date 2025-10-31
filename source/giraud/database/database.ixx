export module database;

import std;
import issue;
import project;

export class Database
{
public:
	std::vector<Project> projects;
	std::unordered_map<int, std::shared_ptr<Issue>> issues;
};