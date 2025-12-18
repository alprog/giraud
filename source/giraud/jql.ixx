export module jql;

import std;
import project;

export class JQL
{
public:
	std::ostringstream stream;

	JQL& project(const Project* project)
	{
		return addCondition("project", project->key);
	}

	JQL& issuetype(std::string type)
	{
		return addCondition("issuetype", type);
	}

	JQL& statusCategory(int value)
	{
		return addCondition("statusCategory", std::to_string(value));
	}

	std::string toString()
	{
		return stream.str();
	}

private:
	JQL& addCondition(std::string key, std::string value)
	{
		const bool isEmtpy = stream.tellp() == 0;
		if (!isEmtpy)
		{
			stream << " AND ";
		}
		stream << key << "=" << value;
		return *this;
	}
};