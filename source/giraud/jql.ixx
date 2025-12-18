export module jql;

import std;

export class JQL
{
public:
	std::ostringstream stream;

	JQL& project(std::string key);
	JQL& issuetype(std::string type);
	JQL& statusCategory(int value);

	std::string toString();

private:
	JQL& addCondition(std::string key, std::string value);
};