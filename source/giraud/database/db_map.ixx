export module db_map;

import std;
import tree_item;

template <typename T>
concept SpecificTreeItem = std::is_base_of<TreeItem, T>::value;

export template<typename TKey, SpecificTreeItem TItem>
class DBMap
{
public:
	TItem* operator[](TKey key)
	{
		auto it = map.find(key);
		if (it != std::end(map))
		{
			return it->second;
		}

		auto item = new TItem(key);
		map[key] = item;
		return item;
	}

	auto begin() { return map.begin(); }
	auto end() { return map.end(); }
	auto empty() { return map.empty(); }

private:
	std::map<TKey, TItem*> map;
};