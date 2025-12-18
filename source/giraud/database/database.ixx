export module database;

import std;
import issue;
import project;
import user;

// for intellisense

export class Database
{
public:
	User myself;

	std::vector<Project*> projects;
};