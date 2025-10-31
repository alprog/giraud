
#include <vector>
#include <cctype>
#include "field.h"
#include "serializable.h"

namespace json
{
	std::string base_field::snake_case_to_camel_case(std::string name)
	{
		for (int i = 0; i < name.size(); i++)
		{
			if (name[i] == '_')
			{
				name.erase(std::begin(name) + i);
				name[i] = toupper(name[i]);
			}
		}
		return name;
	}

	json::object serialize(json::serializable& serializable)
	{
		return serializable.to_json();
	}

	int serialize(int value) { return value; }
	float serialize(float value) { return value; }
	std::time_t serialize(std::time_t value) { return value; }
	std::string serialize(std::string value) { return value; }
}