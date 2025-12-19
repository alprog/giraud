export module tree_item;

import std;
import utils;

export class TreeItem
{
public:
	TreeItem(std::string uid)
		: uid{ uid }
	{
	}

	std::string uid;

	bool opened = false;
	std::vector<TreeItem*> children;

	virtual std::string& getDisplayText() = 0;
};

export void sort_ascending(std::vector<TreeItem*>& array)
{
	sort(array, [](TreeItem* a, TreeItem* b)
		{ return a->getDisplayText() < b->getDisplayText(); }
	);
}