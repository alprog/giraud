export module database;

import std;
import db_map;
import issue;
import tree_item;
import project;
import user;

// for intellisense

export class Database
{
public:
	User myself;
	DBMap<int, Project> projects;
	DBMap<int, Issue> issues;

	std::vector<TreeItem*> treeview;
};