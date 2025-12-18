export module issue;

import std;

// for intellisense

export struct Issue
{
	int id;
	std::string key;
	std::string summary;
	std::string description;
};