export module utils;

import std;

export template <typename TValue>
TValue& access_or_create(std::vector<TValue*>& array, int id)
{
	for (auto ptr : array)
	{
		if (ptr->id == id)
		{
			return *ptr;
		}
	}

	auto ptr = new TValue(id);
	array.push_back(ptr);
	return *ptr;
}

export void trim(std::string& s)
{
	auto is_space = [](unsigned char c) { return std::isspace(c); };
	s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), is_space));
	s.erase(std::find_if_not(s.rbegin(), s.rend(), is_space).base(), s.end());
}

export void to_lower(std::string& s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
}

export void replace(std::string& string, std::string oldValue, std::string newValue)
{
    while (true)
    {
        auto index = string.find(oldValue.c_str());
        if (index == std::string::npos)
        {
            break;
        }
        string.replace(index, oldValue.size(), newValue);
    }
}

export std::string getReplaced(std::string string, std::string oldValue, std::string newValue)
{
    replace(string, oldValue, newValue);
    return string;
}