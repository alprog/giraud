#pragma once

#include "serializable.h"

std::string readFileText(std::string path);
void writeTextToFile(std::string text, std::string path);

namespace json
{
	template <typename T>
	T loadFromFile(std::string path)
	{
		std::string text = readFileText(path);
		if (!text.empty())
		{
			nlohmann::json json = nlohmann::json::parse(text);
			return from_json<T>(json);
		}
		return {};
	}

	void saveToFile(json::serializable& object, std::string path);
}