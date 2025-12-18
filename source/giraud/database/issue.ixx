export module issue;

import std;
import tree_item;
import requests;
import utils;

// for intellisense

export struct Issue : public TreeItem
{
	int id;
	std::string key;
	std::string summary;
	std::string title;
	std::string description;

	Issue(int id)
		: id{ id }
	{
	}

	void update(IssueDesc desc)
	{
		this->key = desc.key;
		this->summary = desc.fields.summary;
		this->title = makeTitle(summary);
	}

	std::string& getDisplayText()
	{
		return title;
	}

	std::string makeTitle(const std::string& summary)
	{
		std::string title = summary;

		size_t index = title.find('-');
		while (index >= 0)
		{
			auto prefixStr = title.substr(0, index);
			trim(prefixStr);
			to_lower(prefixStr);
			if (prefixStr.empty() || prefixStr == "code" || prefixStr == "jus" || prefixStr == "audio")
			{
				title = title.substr(index + 1);
			}
			else
			{
				break;
			}

			index = title.find('-');
		}

		trim(title);
		return title;
	}
};