#include "utils.h"

#include <fstream>

std::string readFileText(std::string path)
{
	std::ifstream stream(path);
	std::ostringstream ss;
	ss << stream.rdbuf();
	return ss.str();
}

void writeTextToFile(std::string text, std::string path)
{
	std::ofstream stream(path);
	stream << text;
	stream.close();
}

void json::saveToFile(json::serializable& object, std::string path)
{
	nlohmann::json json = object.to_json();
	std::string text = json.dump();
	writeTextToFile(text, path);
}