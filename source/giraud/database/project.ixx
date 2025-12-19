export module project;

import std;
import avatar_urls;
import issue;
import requests;
import tree_item;
import folder;

// for intellisense

export class Project : public TreeItem
{
public:
	int id;
	std::string key;
	std::string name;
	std::string description;
	AvatarUrls avatarUrls;

	Project(int id)
		: TreeItem{ std::format("p{}", id) }
		, id{ id }
	{
	}

	std::string& getDisplayText() override
	{
		return name;
	}

	void update(const ProjectDesc& desc)
	{
		this->key = desc.key;
		this->name = desc.name;
		this->description = desc.description;
	}
};