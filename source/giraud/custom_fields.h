#pragma once

#include "kuku_json.h"

struct CustomFields : public json::serializable
{
	std::map<int, std::string> values;

	json::scheme& get_json_scheme() override
	{
		static auto scheme = json::scheme({});
		return scheme;
	}
};

json::object serialize(CustomFields& serializable)
{
	return {};
}

template <>
CustomFields from_json<CustomFields>(json::object& object)
{
	std::string prefix = "customfield_";

	CustomFields customFields;
	for (const auto& [key, value] : object.items())
	{
		if (value.is_string())
		{
			if (key.starts_with(prefix))
			{
				int index = std::atoi(key.substr(prefix.size()).c_str());
				customFields.values[index] = from_json<std::string>(value);
			}
		}
	}
	return customFields;
}

