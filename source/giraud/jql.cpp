module jql;

JQL& JQL::project(std::string key)
{
	return addCondition("project", key);
}

JQL& JQL::issuetype(std::string type)
{
	return addCondition("issuetype", type);
}

JQL& JQL::statusCategory(int value)
{
	return addCondition("statusCategory", std::to_string(value));
}

JQL& JQL::addCondition(std::string key, std::string value)
{
	if (!stream.tellp())
	{
		stream << " AND ";
	}
	stream << key << " = " << value;
	return *this;
}

std::string JQL::toString()
{
	return stream.str();
}