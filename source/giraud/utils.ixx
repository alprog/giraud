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