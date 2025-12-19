export module database;

import std;
import db_map;
import issue;
import tree_item;
import project;
import folder;
import folders_type;
import user;

// for intellisense

export class Database
{
	using TFolderNameGetter = std::function<std::string(TreeItem*)>;

public:
	User myself;
	DBMap<int, Project> projects;
	DBMap<int, Issue> issues;
	DBMap<std::string, Folder> folders;

	std::vector<TreeItem*> treeview;

	void CreateSubFolders(TreeItem* item, EFoldersType type);
	void CreateSubFolders(TreeItem* item, TFolderNameGetter folderNameGetter);
};