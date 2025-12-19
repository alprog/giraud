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

template <>
CustomFields from_json<CustomFields>(json::object& object)
{
	std::string prefix = "customfield_";

	CustomFields customFields;
	for (const auto& [key, arrayValue] : object.items())
	{
		if (key.starts_with(prefix))
		{
			int index = std::atoi(key.substr(prefix.size()).c_str());
			if (arrayValue.is_array())
			{
				if (arrayValue.size() > 0 && arrayValue[0].is_object())
				{
					auto value = arrayValue[0]["value"];
					if (value.is_string())
					{
						customFields.values[index] = from_json<std::string>(value);
					}
				}
			}
		}
	}
	return customFields;
}

