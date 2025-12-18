export module issue;

import std;
import tree_item;
import requests;

// for intellisense

export struct Issue : public TreeItem
{
	int id;
	std::string key;
	std::string summary;
	std::string description;

	Issue(int id)
		: id{ id }
	{
	}

	void update(IssueDesc desc)
	{
		this->key = desc.key;
		this->summary = desc.fields.summary;
	}

	std::string& getDisplayText()
	{
		return summary;
	}
};