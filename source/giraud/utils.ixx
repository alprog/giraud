export module utils;

import std;

export template <typename TKey, typename TValue>
TValue& access_or_create(std::unordered_map<TKey, std::shared_ptr<TValue>>& map, TKey key)
{
	auto it = map.find(key);
	if (it != std::end(map))
	{
		return *it->second;
	}
	else
	{
		std::shared_ptr<TValue> ptr = std::make_shared<TValue>(key);
		map[key] = ptr;
		return *ptr;
	}
}