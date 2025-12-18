export module project;

import std;
import avatar_urls;
import issue;
import requests;

// for intellisense

export struct Project
{
	int id;
	std::string key;
	std::string name;
	std::string description;
	AvatarUrls avatarUrls;

	std::unordered_map<int, std::shared_ptr<Issue>> issues;

	Project(int id)
		: id{ id }
	{
	}

	void update(const ProjectDesc& desc)
	{
		this->key = desc.key;
		this->name = desc.name;
		this->description = desc.description;
	}
};